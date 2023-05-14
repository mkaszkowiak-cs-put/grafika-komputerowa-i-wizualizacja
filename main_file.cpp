#define GLM_FORCE_RADIANS
#define GLM_FORCE_SWIZZLE

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <stdlib.h>
#include <stdio.h>
#include "constants.h"
#include "lodepng.h"
#include "shaderprogram.h"
#include "model.h"
#include "skybox.h"

bool w_pressed = false;
bool s_pressed = false;
bool a_pressed = false;
bool d_pressed = false;

glm::vec3 cameraPosition = glm::vec3(0, 0, 0);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
glm::vec3 cameraDirection = glm::vec3(0, 0, 1); // will be overwritten by yaw & pitch calculations

glm::vec3 lightPosition = glm::vec3(0, 0, -5);
glm::vec3 modelPosition = glm::vec3(0, 0, 20);

float yaw = 90.0f; // picture a kid running around a tree
float pitch = 0.0f;
float aspectRatio = 1;

int screenWidth = 500, screenHeight = 500;

float lastX = screenWidth/2, lastY = screenHeight/2;

ShaderProgram *sp;
ShaderProgram* skyboxShader;

Model* model;

GLuint tex; //Uchwyt – deklaracja globalna
GLuint texCube;


//Procedura obsługi błędów
void error_callback(int error, const char* description) {
	fputs(description, stderr);
}

void keyCallback(GLFWwindow* window,int key,int scancode,int action,int mods) {
    if (action==GLFW_PRESS) {
		if (key == GLFW_KEY_W) w_pressed = true;
		if (key == GLFW_KEY_S) s_pressed = true;
		if (key == GLFW_KEY_A) a_pressed = true;
		if (key == GLFW_KEY_D) d_pressed = true;
    }
    if (action==GLFW_RELEASE) {
		if (key == GLFW_KEY_W) w_pressed = false;
		if (key == GLFW_KEY_S) s_pressed = false;
		if (key == GLFW_KEY_A) a_pressed = false;
		if (key == GLFW_KEY_D) d_pressed = false;
    }
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos) {
	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos; // reversed since y-coordinates range from bottom to top
	lastX = xpos;
	lastY = ypos;

	const float sensitivity = 0.1f;
	xoffset *= sensitivity;
	yoffset *= sensitivity;

	yaw += xoffset;
	pitch += yoffset;

	if (pitch > 89.0f)
		pitch = 89.0f;
	if (pitch < -89.0f)
		pitch = -89.0f;
}

void windowResizeCallback(GLFWwindow* window,int width,int height) {
    if (height==0) return;
    aspectRatio=(float)width/(float)height;
    glViewport(0,0,width,height);
}

bool load_cube_map_side_png(GLuint texture, GLenum side_target, const char* file_name) {
	// Note: untested! I accidentally converted it to lodepng, leaving it in
	glBindTexture(GL_TEXTURE_CUBE_MAP, texture);

	unsigned x, y, n;
	int force_channels = 4;

	std::vector<unsigned char> image;
	string filename = string(file_name);
	unsigned error = lodepng::decode(image, x, y, filename);
	if (error) {
		std::cout << "decoder error " << error << ": " << lodepng_error_text(error) << std::endl;
		return false;
	}

	if ((x & (x - 1)) != 0 || (y & (y - 1)) != 0) {
		fprintf(stderr,
			"WARNING: image %s is not power-of-2 dimensions\n",
			file_name);
	}

	// copy image data into 'target' side of cube map
	glTexImage2D(side_target, 0, GL_RGBA, x, y, 0, GL_RGBA, GL_UNSIGNED_BYTE, (unsigned char*)image.data());
	image.clear();
	return true;
}

bool load_cube_map_side(GLuint texture, GLenum side_target, const char* file_name) {
	glBindTexture(GL_TEXTURE_CUBE_MAP, texture);

	int x, y, n;
	int force_channels = 4;
	unsigned char* image_data = stbi_load(file_name, &x, &y, &n, force_channels);
	if (!image_data) {
		fprintf(stderr, "ERROR: could not load %s\n", file_name);
		return false;
	}

	if ((x & (x - 1)) != 0 || (y & (y - 1)) != 0) {
		fprintf(stderr,
			"WARNING: image %s is not power-of-2 dimensions\n",
			file_name);
	}

	// copy image data into 'target' side of cube map
	glTexImage2D(side_target, 0, GL_RGBA, x, y, 0, GL_RGBA, GL_UNSIGNED_BYTE, image_data);
	free(image_data);
	return true;
}

void create_cube_map(
	const char* negz,
	const char* posz,
	const char* posy,
	const char* negy,
	const char* negx,
	const char* posx,
	GLuint* tex_cube) {
	// generate a cube-map texture to hold all the sides
	glActiveTexture(GL_TEXTURE0);
	glGenTextures(1, tex_cube);

	// load each image and copy into a side of the cube-map texture
	load_cube_map_side(*tex_cube, GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, negz);
	load_cube_map_side(*tex_cube, GL_TEXTURE_CUBE_MAP_POSITIVE_Z, posz);
	load_cube_map_side(*tex_cube, GL_TEXTURE_CUBE_MAP_POSITIVE_Y, posy);
	load_cube_map_side(*tex_cube, GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, negy);
	load_cube_map_side(*tex_cube, GL_TEXTURE_CUBE_MAP_NEGATIVE_X, negx);
	load_cube_map_side(*tex_cube, GL_TEXTURE_CUBE_MAP_POSITIVE_X, posx);
	// format cube map texture
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
}

//Procedura inicjująca
void initOpenGLProgram(GLFWwindow* window) {
	glClearColor(0,0,0,1);
	glEnable(GL_DEPTH_TEST);
	glDepthMask(GL_TRUE);
	glfwSetWindowSizeCallback(window,windowResizeCallback);
	glfwSetKeyCallback(window,keyCallback);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetCursorPosCallback(window, mouse_callback);
	create_cube_map(
		"textures/skybox/negz.jpg", // back - negz
		"textures/skybox/posz.jpg", // front - posz
		"textures/skybox/posy.jpg", // top - posy
		"textures/skybox/negy.jpg", // bottom - negy
		"textures/skybox/negx.jpg", // left - negx
		"textures/skybox/posx.jpg", // right - posx
		&texCube
	);

	sp=new ShaderProgram("v_simplest.glsl",NULL,"f_simplest.glsl");
	skyboxShader= new ShaderProgram("v_skybox.glsl", NULL, "f_skybox.glsl");
	model = new Model("models/Skull.obj");
}

//Zwolnienie zasobów zajętych przez program
void freeOpenGLProgram(GLFWwindow* window) {
    //************Tutaj umieszczaj kod, który należy wykonać po zakończeniu pętli głównej************

    delete sp;
	delete skyboxShader;
}

//Procedura rysująca zawartość sceny
void drawScene(GLFWwindow* window,float angle_x,float angle_y) {
	//************Tutaj umieszczaj kod rysujący obraz******************l
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glm::mat4 skyboxV = glm::lookAt(
		glm::vec3(0, 0, 0),
		cameraDirection,
		cameraUp
	);

	glm::mat4 V=glm::lookAt(
         cameraPosition,
         cameraPosition + cameraDirection,
         cameraUp
	); //Wylicz macierz widoku

    glm::mat4 P=glm::perspective(50.0f*PI/180.0f, aspectRatio, 0.01f, 5000.0f); //Wylicz macierz rzutowania

    glm::mat4 M=glm::mat4(1.0f);
	M=glm::rotate(M,angle_y,glm::vec3(1.0f,0.0f,0.0f)); //Wylicz macierz modelu
	M=glm::rotate(M,angle_x,glm::vec3(0.0f,1.0f,0.0f)); //Wylicz macierz modelu
	
	sp->use();//Aktywacja programu cieniującego
	
	glUniformMatrix4fv(sp->u("P"), 1, false, glm::value_ptr(P));
	glUniformMatrix4fv(sp->u("V"), 1, false, glm::value_ptr(V));
	glUniformMatrix4fv(sp->u("M"), 1, false, glm::value_ptr(M));

	glUniform3fv(sp->u("lightPos"), 1, glm::value_ptr(lightPosition));
	glUniform3fv(sp->u("viewPos"), 1, glm::value_ptr(cameraPosition));

	model->setPosition(modelPosition);
	model->Draw(*sp);
	
	glDepthFunc(GL_LEQUAL);
	skyboxShader->use();

	GLuint vbo;
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, 3 * 36 * sizeof(float), &skybox_points, GL_STATIC_DRAW);

	GLuint vao;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);

	glUniformMatrix4fv(skyboxShader->u("P"), 1, false, glm::value_ptr(P));
	glUniformMatrix4fv(skyboxShader->u("V"), 1, false, glm::value_ptr(skyboxV));
	glUniformMatrix4fv(skyboxShader->u("M"), 1, false, glm::value_ptr(M));

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, texCube);
	glBindVertexArray(vao);
	glDrawArrays(GL_TRIANGLES, 0, 36);

	glDepthFunc(GL_LESS);
	glBindVertexArray(0);

    glfwSwapBuffers(window); //Przerzuć tylny bufor na przedni
}


int main(void)
{
	GLFWwindow* window; //Wskaźnik na obiekt reprezentujący okno

	glfwSetErrorCallback(error_callback);//Zarejestruj procedurę obsługi błędów

	if (!glfwInit()) { //Zainicjuj bibliotekę GLFW
		fprintf(stderr, "Nie można zainicjować GLFW.\n");
		exit(EXIT_FAILURE);
	}

	window = glfwCreateWindow(500, 500, "OpenGL", NULL, NULL);  //Utwórz okno 500x500 o tytule "OpenGL" i kontekst OpenGL.

	if (!window) //Jeżeli okna nie udało się utworzyć, to zamknij program
	{
		fprintf(stderr, "Nie można utworzyć okna.\n");
		glfwTerminate();
		exit(EXIT_FAILURE);
	}

	glfwMakeContextCurrent(window); //Od tego momentu kontekst okna staje się aktywny i polecenia OpenGL będą dotyczyć właśnie jego.
	glfwSwapInterval(1); //Czekaj na 1 powrót plamki przed pokazaniem ukrytego bufora

	if (glewInit() != GLEW_OK) { //Zainicjuj bibliotekę GLEW
		fprintf(stderr, "Nie można zainicjować GLEW.\n");
		exit(EXIT_FAILURE);
	}

	initOpenGLProgram(window); //Operacje inicjujące

	//Główna pętla
	float angle_x=0; //Aktualny kąt obrotu obiektu
	float angle_y=0; //Aktualny kąt obrotu obiektu
	glfwSetTime(0); //Zeruj timer
	while (!glfwWindowShouldClose(window)) //Tak długo jak okno nie powinno zostać zamknięte
	{
		float delta_forward = 0;
		float delta_side = 0;
		if (w_pressed) {
			delta_forward += glfwGetTime();
		}
		if (s_pressed) {
			delta_forward -= glfwGetTime();
		}
		if (a_pressed) {
			delta_side += glfwGetTime();
		}
		if (d_pressed) {
			delta_side -= glfwGetTime();
		}

		delta_forward *= 20;
		delta_side *= 20;
		cameraDirection.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
		cameraDirection.y = sin(glm::radians(pitch));
		cameraDirection.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
		cameraDirection = glm::normalize(cameraDirection);

		glm::vec3 cameraLeft = glm::normalize(glm::cross(cameraUp, cameraDirection));

		glm::vec3 delta_pos = cameraDirection * delta_forward + cameraLeft * delta_side;

		// odkomentuj aby nie latac
		// delta_pos.y = 0;

		cameraPosition += delta_pos;

		/*printf("x:%.2f y:%.2f z:%.2f\n", cameraPosition.x, cameraPosition.y, cameraPosition.z);*/

        glfwSetTime(0); //Zeruj timer
		drawScene(window,angle_x,angle_y); //Wykonaj procedurę rysującą
		glfwPollEvents(); //Wykonaj procedury callback w zalezności od zdarzeń jakie zaszły.
	}

	freeOpenGLProgram(window);

	glfwDestroyWindow(window); //Usuń kontekst OpenGL i okno
	glfwTerminate(); //Zwolnij zasoby zajęte przez GLFW
	exit(EXIT_SUCCESS);
}

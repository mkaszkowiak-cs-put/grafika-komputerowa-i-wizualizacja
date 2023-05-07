/*
Niniejszy program jest wolnym oprogramowaniem; możesz go
rozprowadzać dalej i / lub modyfikować na warunkach Powszechnej
Licencji Publicznej GNU, wydanej przez Fundację Wolnego
Oprogramowania - według wersji 2 tej Licencji lub(według twojego
wyboru) którejś z późniejszych wersji.

Niniejszy program rozpowszechniany jest z nadzieją, iż będzie on
użyteczny - jednak BEZ JAKIEJKOLWIEK GWARANCJI, nawet domyślnej
gwarancji PRZYDATNOŚCI HANDLOWEJ albo PRZYDATNOŚCI DO OKREŚLONYCH
ZASTOSOWAŃ.W celu uzyskania bliższych informacji sięgnij do
Powszechnej Licencji Publicznej GNU.

Z pewnością wraz z niniejszym programem otrzymałeś też egzemplarz
Powszechnej Licencji Publicznej GNU(GNU General Public License);
jeśli nie - napisz do Free Software Foundation, Inc., 59 Temple
Place, Fifth Floor, Boston, MA  02110 - 1301  USA
*/

#define GLM_FORCE_RADIANS
#define GLM_FORCE_SWIZZLE

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
#include "myCube.h"
#include "myTeapot.h"

// Include a text rendering library for displaying X/Y coords in debug mode
// #define GLT_IMPLEMENTATION
// #include "gltext.h"
// Doesn't work fully - commenting it out

bool w_pressed = false;
bool s_pressed = false;
bool a_pressed = false;
bool d_pressed = false;

glm::vec3 cameraPosition = glm::vec3(0, 0, -5);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
glm::vec3 cameraDirection = glm::vec3(0, 0, 1); // will be overwritten by yaw & pitch calculations

float yaw = 90.0f; // picture a kid running around a tree
float pitch = 0.0f;
float aspectRatio = 1;

int screenWidth = 500, screenHeight = 500;

float lastX = screenWidth/2, lastY = screenHeight/2;

ShaderProgram *sp;

//Odkomentuj, żeby rysować kostkę
float* vertices = myCubeVertices;
float* normals = myCubeNormals;
float* texCoords = myCubeTexCoords;
float* colors = myCubeColors;
int vertexCount = myCubeVertexCount;


//Odkomentuj, żeby rysować czajnik
//float* vertices = myTeapotVertices;
//float* normals = myTeapotNormals;
//float* texCoords = myTeapotTexCoords;
//float* colors = myTeapotColors;
//int vertexCount = myTeapotVertexCount;



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

void initDebug() {
	// Stub in case I decide to try and get text rendering working properly
}

//Procedura inicjująca
void initOpenGLProgram(GLFWwindow* window) {
	initDebug();
	//************Tutaj umieszczaj kod, który należy wykonać raz, na początku programu************
	glClearColor(0,0,0,1);
	glEnable(GL_DEPTH_TEST);
	glfwSetWindowSizeCallback(window,windowResizeCallback);
	glfwSetKeyCallback(window,keyCallback);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetCursorPosCallback(window, mouse_callback);

	sp=new ShaderProgram("v_simplest.glsl",NULL,"f_simplest.glsl");
}

//Zwolnienie zasobów zajętych przez program
void freeOpenGLProgram(GLFWwindow* window) {
    //************Tutaj umieszczaj kod, który należy wykonać po zakończeniu pętli głównej************

    delete sp;
}




//Procedura rysująca zawartość sceny
void drawScene(GLFWwindow* window,float angle_x,float angle_y) {
	//************Tutaj umieszczaj kod rysujący obraz******************l
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	sp->use();//Aktywacja programu cieniującego

	glm::mat4 V=glm::lookAt(
         cameraPosition,
         cameraPosition + cameraDirection,
         cameraUp
	); //Wylicz macierz widoku

    glm::mat4 P=glm::perspective(50.0f*PI/180.0f, aspectRatio, 0.01f, 50.0f); //Wylicz macierz rzutowania

    glm::mat4 M=glm::mat4(1.0f);
	M=glm::rotate(M,angle_y,glm::vec3(1.0f,0.0f,0.0f)); //Wylicz macierz modelu
	M=glm::rotate(M,angle_x,glm::vec3(0.0f,1.0f,0.0f)); //Wylicz macierz modelu
  
    //Przeslij parametry programu cieniującego do karty graficznej
    glUniformMatrix4fv(sp->u("P"),1,false,glm::value_ptr(P));
    glUniformMatrix4fv(sp->u("V"),1,false,glm::value_ptr(V));
    glUniformMatrix4fv(sp->u("M"),1,false,glm::value_ptr(M));

    glEnableVertexAttribArray(sp->a("vertex"));  //Włącz przesyłanie danych do atrybutu vertex
    glVertexAttribPointer(sp->a("vertex"),4,GL_FLOAT,false,0,vertices); //Wskaż tablicę z danymi dla atrybutu vertex

    glDrawArrays(GL_TRIANGLES,0,vertexCount); //Narysuj obiekt

    glDisableVertexAttribArray(sp->a("vertex"));  //Wyłącz przesyłanie danych do atrybutu vertex

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
		cameraDirection.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
		cameraDirection.y = sin(glm::radians(pitch));
		cameraDirection.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
		cameraDirection = glm::normalize(cameraDirection);

		glm::vec3 cameraLeft = glm::normalize(glm::cross(cameraUp, cameraDirection));

		glm::vec3 delta_pos = cameraDirection * delta_forward + cameraLeft * delta_side;

		delta_pos.y = 0;

		cameraPosition += delta_pos;

		printf("x:%.2f y:%.2f z:%.2f\n", cameraPosition.x, cameraPosition.y, cameraPosition.z);

        glfwSetTime(0); //Zeruj timer
		drawScene(window,angle_x,angle_y); //Wykonaj procedurę rysującą
		glfwPollEvents(); //Wykonaj procedury callback w zalezności od zdarzeń jakie zaszły.
	}

	freeOpenGLProgram(window);

	glfwDestroyWindow(window); //Usuń kontekst OpenGL i okno
	glfwTerminate(); //Zwolnij zasoby zajęte przez GLFW
	exit(EXIT_SUCCESS);
}

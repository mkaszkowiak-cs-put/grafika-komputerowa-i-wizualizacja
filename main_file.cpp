#define GLM_FORCE_RADIANS
#define GLM_FORCE_SWIZZLE

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <stdlib.h>
#include <stdio.h>
#include "lodepng.h"

// ! kolejnosc importow ma znaczenie, bo dzialamy obecnie na dzikich zmiennych globalnych (extern) xD
#include "shaderprogram.h"
#include "model.h"
#include "input.h"
#include "skybox.h"
#include "drawable.h"
#include "terrain.h"
#include "engine.h"

glm::vec3 lightPosition = glm::vec3(0, 0, -5);

ShaderProgram* mainShader;
ShaderProgram* skyboxShader;

/*
Model* model;
Terrain* terrain;
Skybox* skybox;
*/
Engine* engine;

//Procedura obsługi błędów
void error_callback(int error, const char* description) {
	fputs(description, stderr);
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

	mainShader =new ShaderProgram("vertex_shader.glsl",NULL,"fragment_shader.glsl");
	skyboxShader= new ShaderProgram("v_skybox.glsl", NULL, "f_skybox.glsl");

	engine = new Engine();

	auto model = new Model("models/Skull.obj");
	model->init(mainShader);
	printf("init %d\n", model->getDrawablePriority());
	engine->add(model);

	auto terrain = new Terrain();
	printf("init %d\n", terrain->getDrawablePriority());
	terrain->init(mainShader);
	engine->add(model);

	auto skybox = new Skybox();
	printf("init %d\n", skybox->getDrawablePriority());
	skybox->init(skyboxShader);
	engine->add(model);
}


//Zwolnienie zasobów zajętych przez program
void freeOpenGLProgram(GLFWwindow* window) {
    //************Tutaj umieszczaj kod, który należy wykonać po zakończeniu pętli głównej************

    delete mainShader;
	delete skyboxShader;
}

//Procedura rysująca zawartość sceny
void drawScene(GLFWwindow* window) {
	//************Tutaj umieszczaj kod rysujący obraz******************l
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glm::mat4 M = glm::mat4(1.0f);

	glm::mat4 V=glm::lookAt(
         cameraPosition,
         cameraPosition + cameraDirection,
         cameraUp
	); //Wylicz macierz widoku

    glm::mat4 P=glm::perspective(glm::radians(50.0f), aspectRatio, 0.01f, 5000.0f); //Wylicz macierz rzutowania

	
	mainShader->use();//Aktywacja programu cieniującego
	
	glUniformMatrix4fv(mainShader->u("M"), 1, false, glm::value_ptr(M));
	glUniformMatrix4fv(mainShader->u("V"), 1, false, glm::value_ptr(V));
	glUniformMatrix4fv(mainShader->u("P"), 1, false, glm::value_ptr(P));

	glUniform3fv(mainShader->u("lightPos"), 1, glm::value_ptr(lightPosition));
	glUniform3fv(mainShader->u("viewPos"), 1, glm::value_ptr(cameraPosition));

	engine->draw();

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
	glfwSetTime(0); //Zeruj timer
	while (!glfwWindowShouldClose(window)) //Tak długo jak okno nie powinno zostać zamknięte
	{
		double time_elapsed = glfwGetTime();
		updateCameraPosition(time_elapsed);

		/*printf("x:%.2f y:%.2f z:%.2f\n", cameraPosition.x, cameraPosition.y, cameraPosition.z);*/

        glfwSetTime(0); //Zeruj timer
		drawScene(window); //Wykonaj procedurę rysującą
		glfwPollEvents(); //Wykonaj procedury callback w zalezności od zdarzeń jakie zaszły.
	}

	freeOpenGLProgram(window);

	glfwDestroyWindow(window); //Usuń kontekst OpenGL i okno
	glfwTerminate(); //Zwolnij zasoby zajęte przez GLFW
	exit(EXIT_SUCCESS);
}

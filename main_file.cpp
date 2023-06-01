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
#include <format>
#include "lodepng.h"

// ! kolejnosc importow ma znaczenie, bo dzialamy obecnie na dzikich zmiennych globalnych (extern) xD
#include "shaderprogram.h"
#include "model.h"

#include "drawable.h"
#include "terrain.h"
#include "bounding_box.h"

bool showBoundBoxes = false;

#include "engine.h"
Engine* engine;

#include "npc.h";
#include "input.h"
#include "skybox.h"

#include "skull.h"
#include "room.h"

glm::vec3 lightPosition = glm::vec3(150, 1500, 150); // roughly in the middle in the sky

ShaderProgram* mainShader;
ShaderProgram* skyboxShader;
ShaderProgram* glassShader;


class MockCameraObject : public Object {
public:
	MockCameraObject() {
		boundingBox = new BoundingBox({ -5, -50, -5 }, { 5, 0, 5 });
		position = cameraPosition; // global var... i'm sorry for this code... no time to refactor
	}

	void initObject() override {}
	void stepObject(Engine* engine, double timeDelta) override {}
};

auto cameraObject = new MockCameraObject();

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

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	mainShader = new ShaderProgram("vertex_shader.glsl",NULL,"fragment_shader.glsl");
	skyboxShader= new ShaderProgram("v_skybox.glsl", NULL, "f_skybox.glsl");
	glassShader = new ShaderProgram("v_glass.glsl", NULL, "f_glass.glsl");

	engine = new Engine();
	engine->add(cameraObject, mainShader);

	auto skybox = new SkyboxObject();
	engine->add(skybox, skyboxShader);

	auto npc = new Npc(glm::vec3(100.0f, -20.f, 0));
	engine->add(npc, mainShader);

	auto npc2 = new Npc(glm::vec3(0.f, -20.f, 250.f));
	engine->add(npc2, mainShader);

	auto terrain = new TerrainObject(
		glm::vec3(-700.0f, -41.0f, -700.0f),
		glm::vec3(1000.0f, -41.0f, 1000.0f),
		"dirt.png"
	);
	engine->add(terrain, mainShader);

	auto floor = new TerrainObject(
		glm::vec3(-155.0f, -40.0f, -155.0f),
		glm::vec3(455.0f, -40.0f, 455.0f),
		"tiles.png"
	);
	engine->add(floor, mainShader);

	auto ceiling = new TerrainObject(
		glm::vec3(-155.0f, 60.0f, -155.0f),
		glm::vec3(455.0f, 60.0f, 455.0f),
		"glass.png",
		// DrawablePriority::VERY_HIGH // Nie dziala
		DrawablePriority::NORMAL // dziala (widac teksture, ale nie jest przezroczysta)
	);
	engine->add(ceiling, glassShader);

	for (const auto& wallCoords : roomWallsCoords) {
		auto wall = new Wall(
			wallCoords.start,
			wallCoords.size,
			"bricks.png"
		);
		engine->add(wall, mainShader);
	}

	for (const auto& wallCoords : horizontalWallsCoords) {
		auto wall = new Wall(
			wallCoords.start,
			wallCoords.size,
			"bricks.png"
		);
		engine->add(wall, mainShader);
	}

	for (const auto& wallCoords : verticalWallsCoords) {
		auto wall = new Wall(
			wallCoords.start,
			wallCoords.size,
			"bricks.png"
		);
		engine->add(wall, mainShader);
	}

	for (const auto& painting : paintings) {
		auto wall = new Wall(
			painting.start,
			painting.size,
			painting.texture
		);
		engine->add(wall, mainShader);
	}
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

	for (int i = 0; i < paintings.size(); i++) {
		PaintingSpotLight spotLight = paintings.at(i).spotLight;

		glUniform3fv(mainShader->u(std::format("spotLights[{}].position", i).c_str()), 1, glm::value_ptr(spotLight.position));
		glUniform3fv(mainShader->u(std::format("spotLights[{}].direction", i).c_str()), 1, glm::value_ptr(spotLight.direction));
		glUniform3fv(mainShader->u(std::format("spotLights[{}].ambient", i).c_str()), 1, glm::value_ptr(glm::vec3(0.05f, 0.05f, 0.05f)));
		glUniform3fv(mainShader->u(std::format("spotLights[{}].diffuse", i).c_str()), 1, glm::value_ptr(glm::vec3(0.8f, 0.8f, 0.8f)));
		glUniform3fv(mainShader->u(std::format("spotLights[{}].specular", i).c_str()), 1, glm::value_ptr(glm::vec3(0.3f, 0.3f, 0.3f)));
		glUniform1f(mainShader->u(std::format("spotLights[{}].cutOff", i).c_str()), glm::cos(glm::radians(1.5f)));
		glUniform1f(mainShader->u(std::format("spotLights[{}].outerCutOff ", i).c_str()), glm::cos(glm::radians(6.5f)));
		glUniform1f(mainShader->u(std::format("spotLights[{}].constant", i).c_str()), 6.0f); // the higher, the weaker the light
		glUniform1f(mainShader->u(std::format("spotLights[{}].linear", i).c_str()), 0.001f);
		glUniform1f(mainShader->u(std::format("spotLights[{}].quadratic", i).c_str()), 0.0000001f);
	}

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
		updateCameraPosition(time_elapsed, cameraObject);
		engine->step(time_elapsed);

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

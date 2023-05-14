#ifndef INPUT_H
#define INPUT_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/gtc/type_ptr.hpp>
#include <stdlib.h>
#include <stdio.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "lodepng.h"
// stb_image zastapione przez lodepng
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <mesh.h>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <map>
#include <vector>

extern bool w_pressed = false, s_pressed = false, a_pressed = false, d_pressed = false;

float yaw = 90.0f; // picture a kid running around a tree
float pitch = 0.0f;
float aspectRatio = 1;

int screenWidth = 500, screenHeight = 500;

float lastX = screenWidth / 2, lastY = screenHeight / 2;

extern glm::vec3 cameraPosition = glm::vec3(0, 0, 0);
extern glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
extern glm::vec3 cameraDirection = glm::vec3(0, 0, 1); // will be overwritten by yaw & pitch calculations


void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
	if (action == GLFW_PRESS) {
		if (key == GLFW_KEY_W) w_pressed = true;
		if (key == GLFW_KEY_S) s_pressed = true;
		if (key == GLFW_KEY_A) a_pressed = true;
		if (key == GLFW_KEY_D) d_pressed = true;
	}
	if (action == GLFW_RELEASE) {
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

void windowResizeCallback(GLFWwindow* window, int width, int height) {
	if (height == 0) return;
	aspectRatio = (float)width / (float)height;
	glViewport(0, 0, width, height);
}

float getMovementDeltaForward(double time_elapsed) {
	float delta_forward = 0;
	if (w_pressed) {
		delta_forward += 20 * time_elapsed;
	}
	if (s_pressed) {
		delta_forward -= 20 * time_elapsed;
	}
	return delta_forward;
}

float getMovementDeltaSide(double time_elapsed) {
	float delta_side = 0;
	if (a_pressed) {
		delta_side += 20 * time_elapsed;
	}
	if (d_pressed) {
		delta_side -= 20 * time_elapsed;
	}
	return delta_side;
}

void updateCameraPosition(double time_elapsed) {
	cameraDirection.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	cameraDirection.y = sin(glm::radians(pitch));
	cameraDirection.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
	cameraDirection = glm::normalize(cameraDirection);

	glm::vec3 cameraLeft = glm::normalize(glm::cross(cameraUp, cameraDirection));
	float delta_forward = getMovementDeltaForward(time_elapsed);
	float delta_side = getMovementDeltaSide(time_elapsed);

	glm::vec3 delta_pos = cameraDirection * delta_forward + cameraLeft * delta_side;

	// odkomentuj aby nie latac
	// delta_pos.y = 0;

	cameraPosition += delta_pos;
}

#endif
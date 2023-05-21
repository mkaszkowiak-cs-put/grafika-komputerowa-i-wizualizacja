#ifndef BOUNDING_BOX_H_INCLUDED
#define BOUNDING_BOX_H_INCLUDED

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

class BoundingBox {
public: 
	BoundingBox(glm::vec3 minCorner, glm::vec3 maxCorner) {
		this->minCorner = minCorner;
		this->maxCorner = maxCorner;
		this->position = glm::vec3(0, 0, 0);
	}

	void moveTo(glm::vec3 newPosition) {
		glm::vec3 delta = newPosition - this->position;
		this->position = newPosition;
		this->minCorner += delta;
		this->maxCorner += delta;
	}

	bool intersects(glm::vec3 other) {
		return other.x >= minCorner.x
			&& other.x <= maxCorner.x
			&& other.y >= minCorner.y
			&& other.y <= maxCorner.y
			&& other.z >= minCorner.z
			&& other.z <= maxCorner.z;
	}

	bool intersects(BoundingBox other) {
		return other.maxCorner.x >= minCorner.x
			&& other.minCorner.x <= maxCorner.x
			&& other.maxCorner.y >= minCorner.y
			&& other.minCorner.y <= maxCorner.y
			&& other.maxCorner.z >= minCorner.z
			&& other.minCorner.z <= maxCorner.z;
	}

	glm::vec3 position;

	glm::vec3 minCorner;
	glm::vec3 maxCorner;
};

#endif

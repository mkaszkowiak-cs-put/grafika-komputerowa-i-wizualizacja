#ifndef OBJECT_H_INCLUDED
#define OBJECT_H_INCLUDED

#include "bounding_box.h"
#include "drawable.h"
#include "engine.fwd.h"

class Object {
public: 
	virtual ~Object() {}

	void moveTo(glm::vec3 newPosition) {
		position = newPosition;

		if (boundingBox) {
			boundingBox->moveTo(newPosition);
		}
		if (drawable) {
			drawable->setPosition(newPosition);
		}
	}

	void rotateTo(float newAngle) {
		angle_x = newAngle;

		drawable->setAngleX(newAngle);
	}

	void move(glm::vec3 delta) {
		auto newPosition = position + delta;
		this->moveTo(newPosition);
	}

	void rotate(float delta) {
		auto newAngle = angle_x + delta;
		this->rotateTo(newAngle);
	}

	virtual void initObject() = 0;
	virtual void stepObject(Engine* engine, double timeDelta) = 0;

	void draw() {
		if (drawable) {
			drawable->draw();
		}
	}

	void init(ShaderProgram* shader) {
		if (drawable) {
			this->drawable->init(shader);
		}
		this->initObject();
	}

	void step(Engine* engine, double timeDelta) {
		this->stepObject(engine, timeDelta);
	}


	glm::vec3 position = glm::vec3(0,0,0);
	float angle_x = 0;
	BoundingBox* boundingBox;
	Drawable* drawable;
};
#endif

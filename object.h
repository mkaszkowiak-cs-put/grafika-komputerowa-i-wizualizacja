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

	void move(glm::vec3 delta) {
		auto newPosition = position + delta;
		this->moveTo(newPosition);
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
	BoundingBox* boundingBox;
	Drawable* drawable;
};
#endif

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
		if (boundingBoxDrawable) {
			boundingBoxDrawable->setPosition(newPosition);
		}
	}

	void rotateTo(float newAngle) {
		angle_x = newAngle;

		if (drawable) {
			drawable->setAngleX(newAngle);
		}

		// we don't rotate boundingBoxDrawable, as the boundingBox doesn't rotate
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

	// Object could use a draw() function, but rn Drawables are passed into Engine
	// also Object could handle multiple Drawables, which would be pretty cool

	void init(ShaderProgram* shader) {
		if (drawable) {
			drawable->init(shader);
		}
		if (boundingBoxDrawable) {
			boundingBoxDrawable->init(shader);
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
	Drawable* boundingBoxDrawable;
};
#endif

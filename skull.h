#ifndef SKULL_H_INCLUDED
#define SKULL_H_INCLUDED

#include "engine.h"
#include "drawable.h"
#include "object.h"
#include "model.h"

class Skull : public Object {
public:
	Skull() {
		drawable = new Model("models/Skull.obj");
	}

	void initObject() override {
		this->moveTo(glm::vec3(-30, -30, -30));
	}

	void stepObject(Engine* engine, double timeDelta) override {
		float delta = timeDelta;
		this->move(delta * 2 * glm::vec3(1, 1, 1));
		if (engine->collides(this, position)) {
			printf("Kolizja!!!! %f %f %f\n", position.x, position.y, position.z);
		}
	}


};
#endif

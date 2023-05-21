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

	}

	void stepObject(Engine* engine, double timeDelta) override {
		float delta = timeDelta;
		this->move(delta * 5 * glm::vec3(1, 1, 1));
		if (engine->collides(position)) {
			printf("Kolizja!!!! %f %f %f\n", position.x, position.y, position.z);
		}
	}


};
#endif

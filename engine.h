#ifndef ENGINE_H_INCLUDED
#define ENGINE_H_INCLUDED

#include "drawable.h"
#include <vector>
#include "object.h"

class Engine {

public:
	void add(Object* object, ShaderProgram* shader) {
		auto drawable = object->drawable;
		if (drawable) {
			this->addDrawable(drawable);
		}

		auto boundingBoxDrawable = object->boundingBoxDrawable;
		if (boundingBoxDrawable) {
			this->addDrawable(boundingBoxDrawable);
		}

		object->init(shader);
		objects.push_back(object);
	}

	void step(double timeDelta) {
		for (auto obj : objects) {
			obj->step(this, timeDelta);
		}
	}

	Object* collides(Object* self, glm::vec3 position) {
		for (auto obj : objects) {
			if (!obj->boundingBox) {
				continue;
			}
			
			if (obj->boundingBox->intersects(position) && obj != self) {
				return obj;
			}
		}

		return nullptr;
	}
	
	Object* collides(Object* self, BoundingBox* boundingBox) {
		for (auto obj : objects) {
			if (!obj->boundingBox) {
				continue;
			}

			if (obj->boundingBox->intersects(boundingBox) && obj != self) {
				return obj;
			}
		}

		return nullptr;
	}


	void draw() {
		for (auto obj : very_low_priority) {
			obj->draw();
		}

		for (auto obj : low_priority) {
			obj->draw();
		}

		for (auto obj : normal_priority) {
			obj->draw();
		}

		for (auto obj : high_priority) {
			obj->draw();
		}

		for (auto obj : very_high_priority) {
			obj->draw();
		}
	}

protected:
	std::vector<Drawable*> very_low_priority;
	std::vector<Drawable*> low_priority;
	std::vector<Drawable*> normal_priority;
	std::vector<Drawable*> high_priority;
	std::vector<Drawable*> very_high_priority;

	std::vector<Object*> objects;

	void addDrawable(Drawable* drawable) {
		switch (drawable->getDrawablePriority())
		{
		case DrawablePriority::VERY_LOW:
			very_low_priority.push_back(drawable);
			break;
		case DrawablePriority::LOW:
			low_priority.push_back(drawable);
			break;
		case DrawablePriority::NORMAL:
			normal_priority.push_back(drawable);
			break;
		case DrawablePriority::HIGH:
			high_priority.push_back(drawable);
			break;
		case DrawablePriority::VERY_HIGH:
			very_high_priority.push_back(drawable);
			break;
		}
	}
};

#endif

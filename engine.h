#ifndef ENGINE_H_INCLUDED
#define ENGINE_H_INCLUDED

#include "drawable.h"
#include <vector>

class Engine {

public:
	void add(Drawable* drawable) {
		printf("add %d\n", drawable->getDrawablePriority());
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

	void draw() {
		for (auto obj : very_low_priority) {
			obj->draw();
		}

		for (auto obj : low_priority) {
			//printf("drawing low priority\n");
			obj->draw();
		}

		for (auto obj : normal_priority) {
			//printf("drawing normal priority\n");
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
};

#endif

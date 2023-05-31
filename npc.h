#pragma once
#ifndef NPC_H_INCLUDED
#define NPC_H_INCLUDED

#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <GL/gl.h>
#include <GL/glu.h>
#include "drawable.h"
#include "object.h"
#include "terrain.h"

class Npc : public Object {
public:
    Npc(glm::vec3 startPosition) : startPosition(startPosition) {
        drawable = new Model("models/skull.obj");
        boundingBox = new BoundingBox({ 0, 0, 0 }, { 50, 50, 50 });
        boundingBoxDrawable = new Cuboid(boundingBox->minCorner, boundingBox->maxCorner, "glass.png");

        state = State::Moving;
        timer = 0.0f;
        stopDuration = 5 + static_cast<float>(rand() % 5);
        walkDuration = 15 + static_cast<float>(rand() % 5);
        speed = 50.0f;
        direction = glm::vec3(speed, 0.0f, 0.0f);  // Initial direction 
    }

    void initObject() override {
        this->moveTo(startPosition);
    }

    void stepObject(Engine* engine, double timeDelta) override {
        float delta = static_cast<float>(timeDelta);

        switch (state) {
        case State::Moving:
            stepMoving(engine, delta);
            break;
        case State::Stopping:
            stepStopping(delta);
            break;
        }
    }

private:
    enum class State {
        Moving,
        Stopping
    };

    glm::vec3 direction;
    glm::vec3 startPosition;
    State state;
    float timer;
    float stopDuration;
    float walkDuration;
    float speed;

    void stepMoving(Engine* engine, float delta) {
        glm::vec3 move = delta * 2 * direction;

        boundingBox->moveTo(position + move);

        if (engine->collides(this, boundingBox)) {
            boundingBox->moveTo(position);
            // Collision detected, change direction
            float randomAngle = static_cast<float>(rand() % 360);
            float radians = glm::radians(randomAngle);
            direction = glm::vec3(cos(radians) * speed, 0.0f, sin(radians) * speed);

            // Calculate rotation angle based on the direction vector
            float rotationAngle = atan2(direction.z, direction.x);
            rotationAngle = glm::degrees(rotationAngle);

            // Rotate the model in the direction it is moving
            this->rotateTo(rotationAngle);
        }
        else {
            boundingBox->moveTo(position);
            // No collision, continue moving in the current direction
            this->move(move);
        }

        // Check if it's time to stop
        timer += delta;
        if (timer >= walkDuration) {
            state = State::Stopping;
            timer = 0.0f;

            walkDuration = 15 + static_cast<float>(rand() % 5);
        }
    }

    void stepStopping(float delta) {
        timer += delta;
        if (timer >= stopDuration) {
            state = State::Moving;
            timer = 0.0f;
 
            stopDuration = 5 + static_cast<float>(rand() % 5);
        }
    }
};

#endif // NPC_H_INCLUDED

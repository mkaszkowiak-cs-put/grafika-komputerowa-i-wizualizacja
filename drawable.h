#ifndef DRAWABLE_H_INCLUDED
#define DRAWABLE_H_INCLUDED

#include "shaderprogram.h"
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

// higher priority = will be drawn after other models
enum class DrawablePriority : int {
    VERY_LOW = 1,   
    LOW = 2,        // for regular models 
    NORMAL = 3,     // for skybox
    HIGH = 4,       // for translucent objects
    VERY_HIGH = 5
};

class Drawable
{
public:
    virtual ~Drawable() {}
    virtual void drawDrawable() = 0;
    virtual void afterSetPosition(glm::vec3 pos) {}
    virtual DrawablePriority getDrawablePriority() {
        return DrawablePriority::NORMAL;
    }

    void init(ShaderProgram* shader) {
        this->shader = shader;
        shader->use();
        this->initDrawable();
        this->calculateModelMatrix();
    }

    void draw() {
        glUniformMatrix4fv(shader->u("M"), 1, false, glm::value_ptr(M));
        this->drawDrawable();
    }

    void setPosition(glm::vec3 pos) {
        this->position = pos;
        this->calculateModelMatrix();
        this->afterSetPosition(pos);
    }

    void setAngleX(float angle) {
        this->angle_x = angle;
        this->calculateModelMatrix();
    }

    void calculateModelMatrix() {
        M = glm::translate(glm::mat4(1.0f), position);
        M = glm::rotate(M, angle_y, glm::vec3(1.0f, 0.0f, 0.0f)); //Wylicz macierz modelu
        M = glm::rotate(M, angle_x, glm::vec3(0.0f, 1.0f, 0.0f)); //Wylicz macierz modelu
    }
    

protected:
    ShaderProgram* shader;
    virtual void initDrawable() = 0;
    glm::vec3 position = glm::vec3(0, 0, 0);
    float angle_y = 0;
    float angle_x = 0;
    glm::mat4 M;

};

#endif
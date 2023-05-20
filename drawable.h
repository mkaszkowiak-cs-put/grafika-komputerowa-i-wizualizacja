#ifndef DRAWABLE_H_INCLUDED
#define DRAWABLE_H_INCLUDED

#include "shaderprogram.h"

enum class DrawablePriority : int {
    VERY_LOW = 1,   // for translucent objects
    LOW = 2,        // for skybox
    NORMAL = 3,     // for regular models
    HIGH = 4        // unused but I wanted to make default name look nicer
};

class Drawable
{
public:
    virtual ~Drawable() {}
    virtual void draw() = 0;

    virtual void init(ShaderProgram* shader) {
        this->shader = shader;
        shader->use();
        this->initDrawable();
    }

    DrawablePriority getDrawablePriority() {
        return DrawablePriority::NORMAL;
    }

protected:
    ShaderProgram* shader;
    virtual void initDrawable() = 0;
};

#endif
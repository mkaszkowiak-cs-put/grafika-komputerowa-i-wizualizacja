#ifndef DRAWABLE_H_INCLUDED
#define DRAWABLE_H_INCLUDED

#include "shaderprogram.h"

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
    virtual void draw() = 0;
    virtual DrawablePriority getDrawablePriority() = 0;

    void init(ShaderProgram* shader) {
        this->shader = shader;
        shader->use();
        this->initDrawable();
    }

    

protected:
    ShaderProgram* shader;
    virtual void initDrawable() = 0;
};

#endif
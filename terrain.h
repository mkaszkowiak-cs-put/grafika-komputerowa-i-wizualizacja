#ifndef TERRAIN_H_INCLUDED
#define TERRAIN_H_INCLUDED

#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <GL/gl.h>
#include <GL/glu.h>
#include "drawable.h"

class Terrain: public Drawable {
private:
    GLuint textureID;
    unsigned int VBO, VAO, EBO;

public:
    Terrain() {}


    void draw() {
        glBindTexture(GL_TEXTURE_2D, textureID);
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    }

    DrawablePriority getDrawablePriority() override {
        return DrawablePriority::NORMAL;
    }

protected:
    void initDrawable() {

        textureID = TextureFromFile("dirt.png", "textures");

        float vertices[] = {
            // 4 floats: position, 4 floats: normal vector, 2 floats: tex coords
             0.0f,   -5.0f,  0.0f,   1.0f,   0.0f, 1.0f, 0.0f, 0.0f,  1.0f, 1.0f,
             0.0f,   -5.0f, -100.0f, 1.0f,   0.0f, 1.0f, 0.0f, 0.0f,  1.0f, 0.0f,
            -100.0f, -5.0f, -100.0f, 1.0f,   0.0f, 1.0f, 0.0f, 0.0f,  0.0f, 0.0f,
            -100.0f, -5.0f,  0.0f,   1.0f,   0.0f, 1.0f, 0.0f, 0.0f,  0.0f, 1.0f
        };

        unsigned int indices[] = {
            0, 1, 3,
            1, 2, 3
        };

        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        glGenBuffers(1, &EBO);

        // aktywujemy obecne vertex array - nastepne calle do vertexattribpointer beda sie zapisywaly w tym
        glBindVertexArray(VAO);

        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);


        // position
        glVertexAttribPointer(shader->a("vertex"), 4, GL_FLOAT, GL_FALSE, 10 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(shader->a("vertex"));

        // normal vector
        glVertexAttribPointer(shader->a("normal"), 4, GL_FLOAT, GL_FALSE, 10 * sizeof(float), (void*)(4 * sizeof(float)));
        glEnableVertexAttribArray(shader->a("normal"));

        // texture coord
        glVertexAttribPointer(shader->a("texCoord"), 2, GL_FLOAT, GL_FALSE, 10 * sizeof(float), (void*)(8 * sizeof(float)));
        glEnableVertexAttribArray(shader->a("texCoord"));

    }
};

#endif // TERRAIN_H_INCLUDED

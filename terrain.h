#ifndef TERRAIN_H_INCLUDED
#define TERRAIN_H_INCLUDED

#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <GL/gl.h>
#include <GL/glu.h>
#include "drawable.h"
#include "object.h"

class Rectangle: public Drawable {
private:
    GLuint textureID;
    unsigned int VBO, VAO, EBO;
    unsigned int indices[6] = {
        0, 1, 3,
        1, 2, 3
    };

    glm::vec3 topRight;
    glm::vec3 topLeft;
    glm::vec3 bottomRight;
    glm::vec3 bottomLeft;

    const char* texturePath;

public:
    Rectangle(glm::vec3 topRight, glm::vec3 bottomRight, glm::vec3 bottomLeft, glm::vec3 topLeft, const char* texturePath, float repeatTexture = 1.0f) {
        this->topRight = topRight;
        this->topLeft = topLeft;
        this->bottomRight = bottomRight;
        this->bottomLeft = bottomLeft;
        
        this->texturePath = texturePath;
        this->repeatTexture = repeatTexture;
    }


    void drawDrawable() {
        glBindTexture(GL_TEXTURE_2D, textureID);
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    }

protected:
    float repeatTexture;

    void initDrawable() {
        textureID = TextureFromFile(this->texturePath, "textures");

        float vertices[]  = {
            // 4 floats: position, 4 floats: normal vector, 2 floats: tex coords
            topRight.x, topRight.y, topRight.z,  1.0f,   0.0f, 1.0f, 0.0f, 0.0f,            repeatTexture, repeatTexture,
            bottomRight.x, bottomRight.y, bottomRight.z, 1.0f,   0.0f, 1.0f, 0.0f, 0.0f,    repeatTexture, 0.0f,
            bottomLeft.x, bottomLeft.y, bottomLeft.z, 1.0f,   0.0f, 1.0f, 0.0f, 0.0f,       0.0f, 0.0f,
            topLeft.x, topLeft.y, topLeft.z, 1.0f,  0.0f, 1.0f, 0.0f, 0.0f,                 0.0f, repeatTexture
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

struct CuboidWalls {
    CuboidWalls(const char* left, const char* right, const char* front, const char* back, const char* floor, const char* ceiling) : left(left), right(right), front(front), back(back), floor(floor), ceiling(ceiling) {
        //
    };
    CuboidWalls(const char* all) : left(all), right(all), front(all), back(all), floor(all), ceiling(all) {
        //
    }

    const char* left;
    const char* right;
    const char* front;
    const char* back;
    const char* floor;
    const char* ceiling;
};

class Cuboid : public Drawable {
public:
    Cuboid(const glm::vec3 topFrontLeft, const glm::vec3 bottomBackRight, const CuboidWalls& textures, float repeatTexture = 1.0f) {
        auto a = topFrontLeft.x;
        auto b = topFrontLeft.y;
        auto c = topFrontLeft.z;
        auto d = bottomBackRight.x;
        auto e = bottomBackRight.y;
        auto f = bottomBackRight.z;

        drawablePriority = DrawablePriority::NORMAL;

        // pod³oga:
        walls[0] = new Rectangle(
            glm::vec3(d, b, f),
            glm::vec3(a, b, f),
            glm::vec3(a, b, c),
            glm::vec3(d, b, c),
            textures.floor,
            repeatTexture
        );

        // sufit:
        walls[1] = new Rectangle(
            glm::vec3(d, e, f),
            glm::vec3(a, e, f),
            glm::vec3(a, e, c),
            glm::vec3(d, e, c),
            textures.ceiling,
            repeatTexture
        );


        // przod:
        walls[2] = new Rectangle(
            glm::vec3(a, e, f),
            glm::vec3(a, b, f),
            glm::vec3(a, b, c),
            glm::vec3(a, e, c),
            textures.front,
            repeatTexture
        );

        // tyl:
        walls[3] = new Rectangle(
            glm::vec3(d, e, f),
            glm::vec3(d, b, f),
            glm::vec3(d, b, c),
            glm::vec3(d, e, c),
            textures.back,
            repeatTexture
        );

        // lewo:
        walls[4] = new Rectangle(
            glm::vec3(d, e, c),
            glm::vec3(d, b, c),
            glm::vec3(a, b, c),
            glm::vec3(a, e, c),
            textures.left,
            repeatTexture
        );

        // prawo:
        walls[5] = new Rectangle(
            glm::vec3(d, e, f),
            glm::vec3(d, b, f),
            glm::vec3(a, b, f),
            glm::vec3(a, e, f),
            textures.right,
            repeatTexture
        );

    }
    void drawDrawable() {
        for (int i = 0; i < 6; i++) {
            walls[i]->draw();
        }
    }

    void afterSetPosition(glm::vec3 pos) {
        for (int i = 0; i < 6; i++) {
            walls[i]->setPosition(pos);
        }
    }

    DrawablePriority getDrawablePriority() override {
        return drawablePriority;
    }

protected:
    void initDrawable() {
        for (int i = 0; i < 6; i++) {
            walls[i]->init(shader);
        }
    }

    Rectangle* walls[6];
    DrawablePriority drawablePriority;
};
class TerrainObject : public Object {
public:
    TerrainObject(const glm::vec3 topFrontLeft, const glm::vec3 bottomBackRight, const CuboidWalls& texture, float repeatTexture = 1.0f) {
        drawable = new Cuboid(
            topFrontLeft,
            bottomBackRight,
            texture,
            repeatTexture
        );
    }

    void initObject() override {

    }

    void stepObject(Engine* engine, double timeDelta) override {

    }
};

class Wall : public Object {
public:
    Wall(const glm::vec3 start, const glm::vec3 size, const CuboidWalls& textures) {
        drawable = new Cuboid(
            start,
            start + size,
            textures
        );
        boundingBox = new BoundingBox(
            start,
            start + size
        );
    }

    void initObject() override {

    }

    void stepObject(Engine* engine, double timeDelta) override {
    }
};

#endif // TERRAIN_H_INCLUDED

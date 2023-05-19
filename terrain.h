#ifndef TERRAIN_H_INCLUDED
#define TERRAIN_H_INCLUDED

#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <GL/gl.h>
#include <GL/glu.h>
#include "ShaderProgram.h"

class Terrain {
private:
    int size;
    float tileSize;
    GLuint textureID;
    std::vector<glm::vec3> vertices;
    std::vector<GLuint> indices;

public:
    Terrain(int size, float tileSize) : size(size), tileSize(tileSize) {}

    void loadTexture(const char* imagePath) {
        textureID = TextureFromFile("dirt.png", "textures");
    }

    void generateChunks() {
        vertices.clear();
        indices.clear();

        int numVerticesPerChunk = size + 1;

        for (int cz = -size; cz < size; cz++) {
            for (int cx = -size; cx < size; cx++) {
                int chunkX = cx * size;
                int chunkZ = cz * size;

                // Generate vertices for the chunk
                for (int j = 0; j <= size; j++) {
                    for (int i = 0; i <= size; i++) {
                        float x = chunkX + i * tileSize;
                        float z = chunkZ + j * tileSize;
                        float y = 0.0f;
                        vertices.push_back(glm::vec3(x, y, z));
                    }
                }

                // Generate indices for the chunk
                for (int j = 0; j < size; j++) {
                    for (int i = 0; i < size; i++) {
                        int index = j * numVerticesPerChunk + i;
                        indices.push_back(index);
                        indices.push_back(index + numVerticesPerChunk);
                        indices.push_back(index + numVerticesPerChunk + 1);
                        indices.push_back(index);
                        indices.push_back(index + numVerticesPerChunk + 1);
                        indices.push_back(index + 1);
                    }
                }
            }
        }
    }

    void draw(ShaderProgram& shader) {

        shader.use();

        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, textureID);

        glEnableClientState(GL_VERTEX_ARRAY);
        glVertexPointer(3, GL_FLOAT, 0, vertices.data());

        glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, indices.data());

        glDisableClientState(GL_VERTEX_ARRAY);

        glDisable(GL_TEXTURE_2D);
    }
};

#endif // TERRAIN_H_INCLUDED

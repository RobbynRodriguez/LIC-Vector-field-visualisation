#ifndef LEARNOPENGL_MESH_H
#define LEARNOPENGL_MESH_H

#include <glad/glad.h>
#include "../utilitaires/Structures.h"
#include "../utilitaires/Shader.h"

class Mesh {

public:
    Mesh();
    ~Mesh();
    void initMesh(vector<Vertex> vertices, vector<unsigned int> indices);

    void draw();

private:
    vector<Vertex>       vertices;
    vector<unsigned int> indices;
    unsigned int VAO, VBO, EBO;
    void setupMesh();
    void computeVectorField();
};

#endif //LEARNOPENGL_MESH_H

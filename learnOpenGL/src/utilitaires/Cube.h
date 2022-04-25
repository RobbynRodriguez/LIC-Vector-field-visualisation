#ifndef LEARNOPENGL_CUBE_H
#define LEARNOPENGL_CUBE_H

#include <glad/glad.h> // holds all OpenGL type declarations
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "Shader.h"
#include <string>
#include <vector>
using namespace std;


//Classe pour créer un cube
class Cube {

public:

    unsigned int VAO;

    //Constructeur
    Cube();

    ~Cube();

    //Pour dessiner le cube
    void Draw(Shader &shader);

private:

    unsigned int VBO;

    //Pour set up les buffers
    void setupMesh();

};

#endif //LEARNOPENGL_CUBE_H

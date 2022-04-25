#include <glad/glad.h>
#include <vector>
#include "Shader.h"
#include "Axes.h"
using namespace std;

float axeX[] = {
        0.0f,0.0f,0.0f, 1.0f,0.0f,0.0f,
        1.0f,0.0f,0.0f, 1.0f,0.0f,0.0f
};

float axeY[] = {
        0.0f, 0.0f, 0.0f,0.0f,1.0f,0.0f,
        0.0f, 1.0f, 0.0f,0.0f,1.0f,0.0f
};

float axeZ[] = {
        0.0f,0.0f,0.0f, 0.0f,0.0f,1.0f,
        0.0f,0.0f,1.0f,0.0f,0.0f,1.0f
};

Axes::Axes(int numAxe) {
    //Axe X
    if(numAxe==0){
        this->vertices = axeX;
    }
    //Axe Y
    if (numAxe==1){
        this->vertices = axeY;
    }
    //Axe Z
    if (numAxe==2){
        this->vertices = axeZ;
    }

    //Génération du VAO
    glGenVertexArrays(1,&VAO);
    //Gestion du mesh
    setUpAxe();
}

Axes::~Axes(){
    glDeleteVertexArrays(1,&VAO);
    glDeleteBuffers(1,&VBO);
    glDeleteBuffers(1, &EBO);
}

void Axes::DrawAxes(Shader &shader) {
    glBindVertexArray(VAO);
    glDrawElements(GL_LINE_STRIP,static_cast<unsigned int>(indices.size()),GL_UNSIGNED_INT,0);
    glBindVertexArray(0);
}

void Axes::setUpAxe() {
    //Création des buffers
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    //VBO
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, 12*sizeof(float), &vertices[0], GL_STATIC_DRAW);
    //EBO
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);


    //Set the vertex attribute pointers
    //positions
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6*sizeof(float), (void*)0);
    //couleurs
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6*sizeof(float), (void*)(3*sizeof(float)));

}

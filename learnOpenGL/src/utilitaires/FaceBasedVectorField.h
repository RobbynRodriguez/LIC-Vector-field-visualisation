#ifndef LEARNOPENGL_FACEBASEDVECTORFIELD_H
#define LEARNOPENGL_FACEBASEDVECTORFIELD_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include "Structures.h"
#include "Shader.h"
#include <vector>

using namespace std;

class FaceBasedVectorField{

public :

    unsigned int VAO;
    vector<Vertex> sommets;
    vector<unsigned int> indices;

    //Constructeur
    FaceBasedVectorField(vector<Vertex> sommets, vector<unsigned int> indices);

    //Dessine le mesh
    void draw(Shader &shader);

    //Libère les ressources
    void FreeResources();

private :

    unsigned int VBO,EBO;

    //set up les vertex array et buffer
    void setUpMesh();

    //Calcul le champ de vecteurs tangent à chaque face et l'associe à un sommet de la face
    void computeVectorField();

};


#endif //LEARNOPENGL_FACEBASEDVECTORFIELD_H

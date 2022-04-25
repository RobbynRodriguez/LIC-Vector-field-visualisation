#ifndef LEARNOPENGL_SPHERE_H
#define LEARNOPENGL_SPHERE_H


#include <glad/glad.h>
#include <glm/glm.hpp>
#include "../utilitaires/Structures.h"
#include "../utilitaires/Shader.h"
#include "../utilitaires/Mesh.h"

using namespace std;

class Sphere {

public :

    int LATTITUDE = 50;
    int LONGITUDE = 50;
    float RAYON = 1.0f;
    unsigned int VAO;
    vector<Vertex> sommets;
    vector<unsigned int> indices;
    Mesh sphereMesh;

    //Constructeur
    Sphere();

    vector<Vertex> getVertex(){
        return sommets;
    }

    vector<unsigned int> getIndices(){
        return indices;
    }

    //Dessiner la sphère
    void draw(Shader &shader);


private :

    unsigned int VBO,EBO;

    //set up les vertex array et buffer
//    void setUpMesh();

    //Créer le mesh de la sphère
    void createSphere();

    //Calculer les indices
    void computeIndices(unsigned int n);

    //Calcul le champ de vecteurs tangent à chaque face et l'associe à un sommet de la face
    void computeVectorField();

    //Calcul la normal d'un triangle
    glm::vec3 computeNormalTriangle(glm::vec3 a, glm::vec3 b, glm::vec3 c);

};

#endif //LEARNOPENGL_SPHERE_H

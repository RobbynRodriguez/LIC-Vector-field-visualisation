#ifndef LEARNOPENGL_STRUCTURES_H
#define LEARNOPENGL_STRUCTURES_H

#include <vector>
#include <glm/glm.hpp>
using namespace std;

//Fichier contenant des structures utilitaires

//Structure d'une face
struct Triangle{
    //Indice sommets
    unsigned int S1;
    unsigned int  S2;
    unsigned int  S3;
    //Normal
    glm::vec3 Normal;
};

//Structure d'un sommet
struct Vertex {
    // position
    glm::vec3 Position;
    // normal
    glm::vec3 Normal;
    // coordonnées uv
    glm::vec2 UV;
    // liste des faces de ce sommet
    vector<Triangle> faces;
    // Vecteur du champ de vecteur associé au vertex
    glm::vec3 champVect;
};

// Coordonnées d'un point de l'espace et de sa tangente à un maillage
struct PointTangent{
    // position
    glm::vec3 Position;
    // tangente
    glm::vec3 Tangente;
};


#endif //LEARNOPENGL_STRUCTURES_H

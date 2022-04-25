#ifndef LEARNOPENGL_BEZIERSURFACE_H
#define LEARNOPENGL_BEZIERSURFACE_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "../utilitaires/Shader.h"
#include <string>
#include <vector>
#include "../utilitaires/Structures.h"
#include "../utilitaires/Mesh.h"
using namespace std;

//Classe pour les surfaces de Bézier
class BezierSurface {

public :

    //Un VAO pour dessiner les points de controle
    unsigned int VAOControl;

    //Control Data
    vector<vector<glm::vec3>> pointsControl;
    //M=nombre de lignes, N=nombre de colonnes
    unsigned int M, N;
    vector <unsigned int> indicesPtsControl;

    //Bezier data
    vector<Vertex> pointsBezier;
    vector<unsigned int> indicesBezier;
    Mesh surfaceMesh;
    //Pour dessiner la surface en nombre uniforme de segment, ici avec un pas de 0.02
    float PAS_U = 0.02;
    float PAS_V = 0.02;


    //Constructeur
    BezierSurface(vector<vector<glm::vec3>> pointsControl,unsigned int M, unsigned int N);
    ~BezierSurface();

    //Pour dessiner les points de controle
    void drawPointsControl(Shader &shader);
    void drawBezierSurface (Shader &shader);



private:

    //Un VBO et un EBO pour les points de controle et les points de Bézier
    unsigned int VBOControl, EBOControl;

    //Set up des points de controle
    void setUpControl ();

    //Set up des points de Bézier
    void setUpBezier();

    //Gestion du mesh pour les points de controle
    void setupMesh(vector<Vertex> vertices, vector<unsigned int> indices, unsigned int VAO, unsigned int VBO, unsigned int EBO);

    //Calculer le point P(U,V)
    glm::vec3 computePointUV(float u, float v);

    //Algorithme de Casteljau pour un t et une liste de points de controle donnés
    glm::vec3 deCasteljau(float t, vector<glm::vec3> pointsControl);

    //Calcul les indices à partir du nombre de points de Bézier en colonne
    void computeIndices(unsigned int n);

    //Calculer la normale d'un triangle
    glm::vec3 computeNormalTriangle(glm::vec3 a, glm::vec3 b, glm::vec3 c);

    PointTangent getGeodesicPoint();


};

#endif //LEARNOPENGL_BEZIERSURFACE_H

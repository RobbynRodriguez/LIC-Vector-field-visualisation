#ifndef LEARNOPENGL_BEZIERCURVE_H
#define LEARNOPENGL_BEZIERCURVE_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "../utilitaires/Shader.h"
#include <string>
#include <vector>
#include "../utilitaires/Structures.h"
using namespace std;

//Classe pour les courbes de Bézier
class BezierCurve {

public:

    //Un VAO pour dessiner le polynome de controle
    unsigned int VAOControl;
    //Un VAO pour dessiner la courbe de Bézier
    unsigned int VAOBezier;

    //Control Data
    vector<Vertex>       pointsControl;
    vector<unsigned int> indicesPtsControl;
    unsigned int nbPointsControl;

    //Bezier Data
    //Pour le nombre de segments uniforme
    float pas;
    //Pour la taille de segment uniforme
    float tailleSegment;
    float marge; //Marge acceptable pour la taille du segment
    //data
    vector<Vertex> pointsBezier;
    vector<unsigned int> indicesBezier;

    //Constructeur pour uniforme en nombre de segments
    BezierCurve(glm::vec3* pointsControl, unsigned int nbPointsControl, unsigned int nbSegments) {

        pas = 1. / nbSegments;
        this->nbPointsControl = nbPointsControl;

        //On génère nos VAO
        glGenVertexArrays(1, &VAOControl);
        glGenVertexArrays(1, &VAOBezier);

        //On set up les points de controle
        setUpControl(pointsControl);

        //On set up les points de Bézier
        setUpBezierNbPoints();
    }

    //Constructeur pour uniforme en longueur de segments
    BezierCurve(glm::vec3* pointsControl, unsigned int nbPointsControl, float tailleSegment) {

        this->tailleSegment = tailleSegment;
        marge = tailleSegment*0.05;
        this->nbPointsControl = nbPointsControl;

        //On génère nos VAO
        glGenVertexArrays(1, &VAOControl);
        glGenVertexArrays(1, &VAOBezier);

        //On set up les points de controle
        setUpControl(pointsControl);

        //On set up les points de Bézier
        setUpBezierLenSegment();
    }

    //Pour dessiner les points de controle
    void DrawPointsControle(Shader &shader) {

        glBindVertexArray(VAOControl);
        glPointSize(5);
        glDrawElements(GL_POINTS, static_cast<unsigned int>(pointsControl.size()), GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);

    }

    //Pour dessiner les segments entre chaque point de controle
    void DrawSegments (Shader &shader){

        glBindVertexArray(VAOControl);
        glDrawElements(GL_LINE_STRIP, static_cast<unsigned int>(indicesPtsControl.size()), GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);

    }

    //Pour dessiner la courbe de Bézier
    void DrawBezierCurve(Shader &shader) {

        glBindVertexArray(VAOBezier);
        glDrawElements(GL_LINE_STRIP,static_cast<unsigned int>(indicesBezier.size()), GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);

    }

    void FreeResources(){
        glDeleteVertexArrays(1, &VAOControl);
        glDeleteVertexArrays(1, &VAOBezier);
        glDeleteBuffers(1, &VBO);
        glDeleteBuffers(1, &EBO);
    }


private:

    unsigned int VBO, EBO;


    //Set up des points de controle
    void setUpControl (glm::vec3* pointsControl){

        //On crée notre vecteur de sommets
        for (int i=0; i<nbPointsControl; i++){
            Vertex vertex;
            vertex.Position = pointsControl[i];
            this->pointsControl.push_back(vertex);
        }
        //On crée notre vecteur d'indice
        for (int i=0; i<nbPointsControl; i++){
            indicesPtsControl.push_back(i);
        }

        //On met à jour les buffer
        setupMesh(this->pointsControl, indicesPtsControl, VAOControl);

    }

    //Set up des points de Bézier pour un nombre de segment uniforme
    void setUpBezierNbPoints(){

        //On récupère tous les points de notre courbe de Bézier
        for (float t=0; t<1 ; t+=pas){

            Vertex vertex;

            //On récupère la position de notre point
            vertex.Position = algoDeCasteljau(t);;

            //On ajoute le point et l'indice
            pointsBezier.push_back(vertex);
            indicesBezier.push_back(pointsBezier.size()-1);
        }

        //On met à jour nos buffers
        setupMesh(pointsBezier,indicesBezier,VAOBezier);

    }

    //Set up des points de Bézier pour une longueur de segment uniforme
    void setUpBezierLenSegment(){

        //indice du dernier point de la courbe de Bezier
        int i = 0;
        //initialisation avec le premier point de controle
        pointsBezier.push_back(pointsControl[0]);
        indicesBezier.push_back(i);
        //distance entre le point calculé en cours et le dernier point de la courbe de Bezier
        float dist;
        //Bornes dichotomie
        float min = 0;
        float max = 1;
        //Le t de base est à la moitié de [0,1]
        float t = 0.5;
        //valeur par laquelle modifier t par dichotomie
        float n;

        //Tant qu'on est trop loin du dernier point de controle on ajoute des points
        while (computeDist(pointsBezier[i].Position,pointsControl[nbPointsControl-1].Position) > tailleSegment){

            //initialisation
            //point de la courbe de Bezier calculé
            Vertex vertex;
            glm::vec3 vertexPos;

            //Tant qu'on a pas une bonne distance entre le point calculé et le dernier point de controle
            do {

                //Calcul du point avec Casteljau
                vertexPos = algoDeCasteljau(t);

                dist = computeDist(vertexPos,pointsBezier[i].Position);
                //Si le point est trop pret on augmente t
                if (dist < tailleSegment){
                    //min = std::max(min,t);
                    min=t;
                    n = (max-min)/2;
                    t = min+n;
                }
                //S'il est trop loin on diminue t
                else {
                    max = t;
                    n = (max-min)/2;
                    t = max - n;
                }

            } while ( dist < tailleSegment-marge || dist > tailleSegment+marge);

            //On a trouvé le bon point
            vertex.Position = vertexPos;

            //On met à jour l'indice
            i++;

            //On ajoute le point et l'indice
            pointsBezier.push_back(vertex);
            indicesBezier.push_back(i);

            //On met à jour le min afin de ne pas se retrouver avec un point précédent le dernier
            min = t;
            //On reset le max
            max = 1;
            //On positionne t au milieu de min et max
            t = min + ((max - min) / 2);

        }

        //On ajoute le dernier point de la courbe = dernier point de controle
        pointsBezier.push_back(pointsControl[nbPointsControl-1]);
        indicesBezier.push_back(++i);

        //On met à jour nos buffers
        setupMesh(pointsBezier,indicesBezier,VAOBezier);
    }

    //Gestion du mesh
    void setupMesh(vector<Vertex> vertices, vector<unsigned int> indices, unsigned int VAO) {

        //Création des buffers
        glGenBuffers(1, &VBO);
        glGenBuffers(1, &EBO);

        glBindVertexArray(VAO);

        //VBO
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);
        //EBO
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

        //Set the vertex attribute pointers
        //positions
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);

    }

    //Algorithme de De Casteljau
    glm::vec3 algoDeCasteljau(float t){

        vector<glm::vec3> elem;
        //Initialisation des premiers éléments
        for (int i=0; i<nbPointsControl; i++){
            elem.push_back(pointsControl[i].Position);
        }

        //Itérations
        for (int r=1; r<nbPointsControl; r++){
            for (int i=0; i<nbPointsControl-r; i++){
                //A chaque itération on remplace l'élément i par l'interpolation des anciens éléments i et i+1
                elem[i] = (1 - t) * elem[i] + t * elem[i + 1];
            }
        }

        //On renvoie la position de notre point
        return elem[0];
    }

    //Calcul la distance entre 2 points
    float computeDist(glm::vec3 a, glm::vec3 b){
        return sqrt(pow(a.x - b.x, 2) + pow(a.y - b.y, 2) + pow(a.z - b.z, 2));
    }

};

#endif //LEARNOPENGL_BEZIERCURVE_H

#ifndef LEARNOPENGL_BEZIERSURFACE_H
#define LEARNOPENGL_BEZIERSURFACE_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "../utilitaires/Shader.h"
#include <string>
#include <vector>
#include "../utilitaires/Structures.h"
using namespace std;

//Classe pour les surfaces de Bézier
class BezierSurface {

public :

    //Un VAO pour dessiner les points de controle
    unsigned int VAOControl;
    //Un VAO pour dessiner la surface de Bézier
    unsigned  int VAOBezier;

    //Control Data
    vector<vector<glm::vec3>> pointsControl;
    //M=nombre de lignes, N=nombre de colonnes
    unsigned int M, N;
    vector <unsigned int> indicesPtsControl;

    //Bezier data
    vector<Vertex> pointsBezier;
    vector<unsigned int> indicesBezier;
    //Pour dessiner la surface en nombre uniforme de segment, ici avec un pas de 0.02
    float PAS_U = 0.02;
    float PAS_V = 0.02;


    //Constructeur
    BezierSurface(vector<vector<glm::vec3>> pointsControl,unsigned int M, unsigned int N){

        this->pointsControl = pointsControl;
        this->M=M;
        this->N=N;

        //On génère les VAO
        glGenVertexArrays(1, &VAOControl);
        glGenVertexArrays(1, &VAOBezier);

        //On set up les points de controle
        setUpControl();

        //On set up les points de Bézier
        setUpBezier();

    }

    //Pour dessiner les points de controle
    void drawPointsControl(Shader &shader){

        glBindVertexArray(VAOControl);
        glPointSize(5);
        glDrawElements(GL_POINTS, static_cast<unsigned int>(indicesPtsControl.size()), GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);

    }

    //Pour dessiner la surface de Bézier
    void drawBezierSurface (Shader &shader){

        glBindVertexArray(VAOBezier);
        glDrawElements(GL_TRIANGLES,static_cast<unsigned int>(indicesBezier.size()),GL_UNSIGNED_INT,0);
        glBindVertexArray(0);

    }

    void FreeResources(){
        glDeleteVertexArrays(1, &VAOControl);
        glDeleteVertexArrays(1, &VAOBezier);
        glDeleteBuffers(1, &VBOControl);
        glDeleteBuffers(1, &EBOControl);
        glDeleteBuffers(1, &VBOBezier);
        glDeleteBuffers(1, &EBOBezier);
    }


private:

    //Un VBO et un EBO pour les points de controle et les points de Bézier
    unsigned int VBOControl, EBOControl, VBOBezier, EBOBezier;


    //Set up des points de controle
    void setUpControl (){

        //On crée une liste de points de controle pour les avoir sur une seule dimension
        vector<Vertex> listePointsControl;
        for (int i=0; i<N; i++){
            for (int j=0; j<M; j++){
                Vertex vertex;
                vertex.Position = pointsControl[i][j];
                listePointsControl.push_back(vertex);
            }
        }

        //On crée notre vecteur d'indice
        for (int i=0; i<N*M; i++){
            indicesPtsControl.push_back(i);
        }

        //Génération des buffers
        glGenBuffers(1, &VBOControl);
        glGenBuffers(1, &EBOControl);

        //On met à jour les buffer
        setupMesh(listePointsControl, indicesPtsControl, VAOControl,VBOControl,EBOControl);

    }

    //Set up des points de Bézier
    void setUpBezier(){

        //Le nombre de points en colonne
        unsigned int n;

        //Calculer tous les points
        for (float u=0; u<1; u+=PAS_U){
            n=0;

            for (float v=0; v<1; v+=PAS_V){

                //On calcule le point
                glm::vec3 pos = computePointUV(u, v);

                //On l'ajoute
                Vertex vertex;
                vertex.Position = pos;
                vertex.Normal = glm::vec3(0.0f);
                vertex.UV = glm::vec2(u,v);
                pointsBezier.push_back(vertex);

                n++;
            }
        }

        //Calculer tous les indices
        computeIndices(n);

        //Calculer toutes les normales

        vector<Triangle> faces;

        //On calcule chaque normal de chaque face
        for (int i=0; i<indicesBezier.size(); i+=3){

            Triangle face;
            face.S1 = indicesBezier[i];
            face.S2 = indicesBezier[i+1];
            face.S3 = indicesBezier[i+2];
            cout << "S1 " << pointsBezier[face.S1].Position[0] << "," << pointsBezier[face.S1].Position[1] << "," << pointsBezier[face.S1].Position[2]  << endl;
            cout << "S2 " << pointsBezier[face.S2].Position[0] << "," << pointsBezier[face.S2].Position[1] << "," << pointsBezier[face.S2].Position[2]  << endl;
            cout << "S3 " << pointsBezier[face.S3].Position[0] << "," << pointsBezier[face.S3].Position[1] << "," << pointsBezier[face.S3].Position[2]  << endl;
            face.Normal = computeNormalTriangle(pointsBezier[face.S1].Position, pointsBezier[face.S2].Position,
                                                pointsBezier[face.S3].Position);


            //On ajouter la face
            faces.push_back(face);

            //On ajoute aux sommets concernés la face dans laquelle ils interviennent
            pointsBezier[face.S1].faces.push_back(face);
            pointsBezier[face.S2].faces.push_back(face);
            pointsBezier[face.S3].faces.push_back(face);
        }

        //On calcule les normales de chaque sommet
        for (int i=0; i<pointsBezier.size();i++){

            glm::vec3 normal = glm::vec3(0.0f);

            //On somme les normales de chaque face dans lesquelles le sommet intervient
            int x = pointsBezier[i].faces.size();
            if(x%2==1 && x != 1){
                x -= 1;
            }
            for (int j=0; j<x;j++){
                cout << " nroroemldeh "<<pointsBezier[i].faces[j].Normal[0] << "," << pointsBezier[i].faces[j].Normal[1] << "," << pointsBezier[i].faces[j].Normal[2] << endl;
                normal+=pointsBezier[i].faces[j].Normal;
            }
//            cout << normal[0] << "," << normal[1] << "," << normal[2]  << endl;
            //On lui passe la normal normalisée
            if(x == 1 && i == 0){
                pointsBezier[i].Normal = - glm::normalize(normal);
            }else {
                pointsBezier[i].Normal = glm::normalize(normal);
            }

//            if((i>=0 && i <50) || i%n==0){
//                pointsBezier[i].Normal = -pointsBezier[i].Normal;
//            }
//            pointsBezier[i].Normal = normal;
            cout << pointsBezier[i].Normal[0] << "," << pointsBezier[i].Normal[1] << "," << pointsBezier[i].Normal[2]  << endl;
        }

        //On génère les buffers
        glGenBuffers(1, &VBOBezier);
        glGenBuffers(1, &EBOBezier);

        //On met à jour les buffers
        setUpMeshSurface();

    }

    //Gestion du mesh pour les points de controle
    void setupMesh(vector<Vertex> vertices, vector<unsigned int> indices, unsigned int VAO, unsigned int VBO, unsigned int EBO) {

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

    //Gestion du mesh pour la surface
    void setUpMeshSurface (){

        glBindVertexArray(VAOBezier);

        //VBO
        glBindBuffer(GL_ARRAY_BUFFER, VBOBezier);
        glBufferData(GL_ARRAY_BUFFER, pointsBezier.size() * sizeof(Vertex), &pointsBezier[0], GL_STATIC_DRAW);
        //EBO
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBOBezier);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indicesBezier.size() * sizeof(unsigned int), &indicesBezier[0], GL_STATIC_DRAW);

        //Set the vertex attribute pointers
        // vertex positions
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
        // vertex normals
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Normal));
        // vertex texture coords
        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, UV));

        glBindVertexArray(0);

    }

    //Calculer le point P(U,V)
    glm::vec3 computePointUV(float u, float v){

        vector<glm::vec3> points;
        for (unsigned int i=0; i<M; i++){
            points.push_back(deCasteljau(v,pointsControl[i]));
        }

        glm::vec3 puv = deCasteljau(u,points);

        return puv;
    }

    //Algorithme de Casteljau pour un t et une liste de points de controle donnés
    glm::vec3 deCasteljau(float t, vector<glm::vec3> pointsControl){
        vector<glm::vec3> elem;

        //Initialisation des premiers éléments
        for (int i=0; i<pointsControl.size(); i++){
            elem.push_back(pointsControl[i]);
        }

        //Itération
        for (int r=1; r<pointsControl.size(); r++){
            for (int i=0; i<pointsControl.size()-r; i++){
                //A chaque itération on remplace l'élément i par l'interpolation des anciens éléments i et i+1
                elem[i] = (1 - t) * elem[i] + t * elem[i + 1];
            }
        }

        //On renvoie la position de notre point
        return elem[0];
    }

    //Calcul les indices à partir du nombre de points de Bézier en colonne
    void computeIndices(unsigned int n){

        //Tout sauf la derniere ligne
        for (int i=0; i<pointsBezier.size()-n; i++){

            //Tout sauf la derniere colonne
            if((i+1)%n!=0){

                //1er triangle
                indicesBezier.push_back(i);
                indicesBezier.push_back(i+1);
                indicesBezier.push_back(i+n);
                //2ème triangle
                indicesBezier.push_back(i+n);
                indicesBezier.push_back(i+n+1);
                indicesBezier.push_back(i+1);

            }
        }
    }

    //Calculer la normale d'un triangle
    glm::vec3 computeNormalTriangle(glm::vec3 a, glm::vec3 b, glm::vec3 c) {

        glm::vec3 u;
        glm::vec3 v;

        u = glm::normalize(a-b);
        v = glm::normalize(c-b);



        return  glm::cross(u,v);
    }

    PointTangent getGeodesicPoint(){
        PointTangent ptT;
        Vertex center = pointsBezier.at(pointsBezier.size()/2);
        ptT.Position = center.Position;
        ptT.Tangente.x = 1/(sqrt(1 + 1 + pow(center.Normal.x, 2)));
        ptT.Tangente.y = 1/(sqrt(1 + 1 + pow(center.Normal.x, 2)));
        ptT.Tangente.z = ptT.Position.x/(sqrt(1 + 1 + pow(center.Normal.x, 2)));
        return(ptT);
    }

};

#endif //LEARNOPENGL_BEZIERSURFACE_H

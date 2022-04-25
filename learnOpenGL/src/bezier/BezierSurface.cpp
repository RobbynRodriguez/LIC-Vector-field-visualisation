#include "BezierSurface.h"

BezierSurface::BezierSurface(vector<vector<glm::vec3>> pointsControl,unsigned int M, unsigned int N){
    this->pointsControl = pointsControl;
    this->M=M;
    this->N=N;

    //On génère les VAO
    glGenVertexArrays(1, &VAOControl);

    //On set up les points de controle
    setUpControl();

    //On set up les points de Bézier
    setUpBezier();
}

BezierSurface::~BezierSurface(){
//    glDeleteVertexArrays(1, &VAOControl);
//    glDeleteBuffers(1, &VBOControl);
//    glDeleteBuffers(1, &EBOControl);
    surfaceMesh.~Mesh();
}

//Pour dessiner les points de controle
void BezierSurface::drawPointsControl(Shader &shader){

    glBindVertexArray(VAOControl);
    glPointSize(5);
    glDrawElements(GL_POINTS, static_cast<unsigned int>(indicesPtsControl.size()), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);

}

//Pour dessiner la surface de Bézier
void BezierSurface::drawBezierSurface (Shader &shader){
    surfaceMesh.draw();
}


void BezierSurface::setUpControl (){

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
void BezierSurface::setUpBezier(){

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
            normal+=pointsBezier[i].faces[j].Normal;
        }
        //On lui passe la normal normalisée
        if(x == 1 && i == 0){
            pointsBezier[i].Normal = - glm::normalize(normal);
        }else {
            pointsBezier[i].Normal = glm::normalize(normal);
        }
    }

    //On met à jour les buffers
    surfaceMesh.initMesh(pointsBezier, indicesBezier);

}

//Gestion du mesh pour les points de controle
void BezierSurface::setupMesh(vector<Vertex> vertices, vector<unsigned int> indices, unsigned int VAO, unsigned int VBO, unsigned int EBO) {

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


//Calculer le point P(U,V)
glm::vec3 BezierSurface::computePointUV(float u, float v){

    vector<glm::vec3> points;
    for (unsigned int i=0; i<M; i++){
        points.push_back(deCasteljau(v,pointsControl[i]));
    }

    glm::vec3 puv = deCasteljau(u,points);

    return puv;
}

//Algorithme de Casteljau pour un t et une liste de points de controle donnés
glm::vec3 BezierSurface::deCasteljau(float t, vector<glm::vec3> pointsControl){
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
void BezierSurface::computeIndices(unsigned int n){

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
glm::vec3 BezierSurface::computeNormalTriangle(glm::vec3 a, glm::vec3 b, glm::vec3 c) {

    glm::vec3 u;
    glm::vec3 v;

    u = glm::normalize(a-b);
    v = glm::normalize(c-b);



    return  glm::cross(u,v);
}

PointTangent BezierSurface::getGeodesicPoint(){
    PointTangent ptT;
    Vertex center = pointsBezier.at(pointsBezier.size()/2);
    ptT.Position = center.Position;
    ptT.Tangente.x = 1/(sqrt(1 + 1 + pow(center.Normal.x, 2)));
    ptT.Tangente.y = 1/(sqrt(1 + 1 + pow(center.Normal.x, 2)));
    ptT.Tangente.z = ptT.Position.x/(sqrt(1 + 1 + pow(center.Normal.x, 2)));
    return(ptT);
}
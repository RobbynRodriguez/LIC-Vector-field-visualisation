#include "Sphere.h"

Sphere::Sphere() {
    createSphere();

    computeIndices(LONGITUDE);

    //On calcule chaque normal de chaque face
    for (int i=0; i<indices.size() - 2; i+=3){

        Triangle face;
        face.S1 = indices[i];
        face.S2 = indices[i+1];
        face.S3 = indices[i+2];
        face.Normal = computeNormalTriangle(sommets[face.S1].Position, sommets[face.S2].Position,
                                            sommets[face.S3].Position);

        //On ajoute aux sommets concernés la face dans laquelle ils interviennent
        sommets[face.S1].faces.push_back(face);
        sommets[face.S2].faces.push_back(face);
        sommets[face.S3].faces.push_back(face);
    }


    //On calcule les normales de chaque sommet
    for (int i=0; i<sommets.size();i++){

        glm::vec3 normal = glm::vec3(0.0f);

        //On somme les normales de chaque face dans lesquelles le sommet intervient
        for (int j=0; j<sommets[i].faces.size();j++){
            normal+=sommets[i].faces[j].Normal;
        }
        //On lui passe la normal normalisée
        sommets[i].Normal = - glm::normalize(normal);

    }
    sommets[sommets.size()-1].Normal = - glm::normalize(glm::vec3(0.0, 1.0, 0.0));

    sphereMesh.initMesh(sommets, indices);
}


void Sphere::draw(Shader &shader) {
    sphereMesh.draw();
}


void Sphere::createSphere() {
    Vertex vertex0;
    vertex0.Position = glm::vec3(0.0f, 1.0f, 0.0f);
    vertex0.UV = {0,0};
    sommets.push_back(vertex0);
    for(float m = 1.0; m < LONGITUDE - 1; m++){
        for(float n = 0.0; n < LATTITUDE; n++){
            Vertex vertex;
            float x = (float)glm::sin(M_PI * (m/LONGITUDE)) * (float)glm::cos(2 * M_PI * (n/LATTITUDE));
            float y = (float)glm::cos(M_PI * m/LONGITUDE);
            float z = (float)glm::sin(M_PI * (m/LONGITUDE)) * (float)glm::sin(2 * M_PI * (n/LATTITUDE));
            vertex.Position = glm::vec3(x, y, z);
            sommets.push_back(vertex);
        }
    }
    Vertex lastVertex;
    lastVertex.Position = glm::vec3(0.0f, -1.0f, 0.0f);
    lastVertex.UV = {1,1};
    sommets.push_back(lastVertex);
}

void Sphere::computeIndices(unsigned int n) {
    // On crée les premier indices indépendamment puisque ce sont des cas particulier, ils sont tous liés au pôle
    for(int i = 0; i < n -1; i++){
        //Premier triangle
        indices.push_back(0);
        indices.push_back(i+1);
        indices.push_back(i+2);
    }
    //On ferme la boucle
    indices.push_back(0);
    indices.push_back(n);
    indices.push_back(1);

    // Pour tout les triangles intemédiaires
    for(int i = 1; i < sommets.size() - n - 1; i++){
        if(i%n!=0) {
            //1er triangle
            indices.push_back(i);
            indices.push_back(i + 1);
            indices.push_back(i + n);
            //2ème triangle
            indices.push_back(i + n);
            indices.push_back(i + n + 1);
            indices.push_back(i + 1);
        }
        //On ferme la boucle
        else{
            //Avant dernier triangle de lattitude
            indices.push_back(i);
            indices.push_back(i - n + 1);
            indices.push_back(i + n);

            // Dernier triangle de lattitude
            indices.push_back(i + n);
            indices.push_back(i + 1);
            indices.push_back(i - n + 1);
        }
    }

    // Finalement pour la dernière longitude, particulière puisque tous liés au pôle
    for(int i = sommets.size() - n - 1; i < sommets.size()-2; i++){
        //Premier triangle
        indices.push_back(i+1);
        indices.push_back(i);
        indices.push_back(sommets.size() - 1);
    }
    //on ferme la boucle
    indices.push_back(sommets.size() - 1 - n);
    indices.push_back(sommets.size() - 2);
    indices.push_back(sommets.size() - 1);
}

//Calculer la normale d'un triangle
glm::vec3 Sphere::computeNormalTriangle(glm::vec3 a, glm::vec3 b, glm::vec3 c) {

    glm::vec3 u;
    glm::vec3 v;

    u = glm::normalize(a-b);
    v = glm::normalize(c-b);

    return  - glm::cross(u,v);
}

void Sphere::computeVectorField() {
    //Pour chaque point on associe le vecteur d'une face
    for (int i=0; i<indices.size(); i+=3){
        //Une des arrêtes de la face
        glm::vec3 e1 = sommets[indices[i+1]].Position - sommets[indices[i]].Position;
        //Le vecteur orthogonale à l'arrête et à la normale
        glm::vec3 e2 = glm::cross(e1, sommets[indices[i]].Normal);
        //Le vecteur du champ qui est donc tangent à notre face
        sommets[indices[i]].champVect = e2;
    }
}

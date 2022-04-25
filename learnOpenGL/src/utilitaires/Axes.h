#ifndef LEARNOPENGL_AXES_H
#define LEARNOPENGL_AXES_H

//Classe pour la création des axes X, Y et Z
class Axes {

public :
    std::vector<unsigned int> indices = {0,1,2,3,4,5,6};
    unsigned int VAO;
    float* vertices;

    //Constructeur avec numAxe le numéro de l'axe qu'on veut créer (0-x, 1-y, 2-z)
    Axes(int numAxe);

    ~Axes();

    //Dessiner l'axe
    void DrawAxes (Shader &shader);



private :

    unsigned int VBO,EBO;

    //Gestion du mesh
    void setUpAxe();

};



#endif //LEARNOPENGL_AXES_H

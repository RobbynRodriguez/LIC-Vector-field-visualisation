#version 330 core

uniform sampler2D tex;
uniform int STEPS;
uniform float STEPSIZE;
uniform float t;

in vec3 Normal;
in vec3 FragPos;
in vec2 TextCoords;

out vec4 FragColor;

//Fonction(s) du champ de vecteurs
vec2 champVectoriel(float x, float y){
    x = 2 * x -1;
    y = 2 * y -1;
    x = x*10;
    y = y*10;
//     return vec2(-x, -y);
//     return vec2(-y, x);
    return vec2(sin(x * t) + sin(y), sin(x) - sin(y * t));
}

//Verifier que les coordonnées ne sortent pas de la texture
bool verif (vec2 step_coords){
    return (step_coords[0] >= 0 && step_coords[0] <= 1 &&
            step_coords[1] >= 0 && step_coords[1] <= 1);
}

vec3 LIC(){

    vec3 valeur; //couleur du fragment
    vec2 vector_components; //coordonnees du champ de vecteurs
    vec2 init_coords = TextCoords; //Commencer au centre
    vec2 step_coords = init_coords; //coordonnes du parcours
    int nb_step_front = 0;
    int nb_step_back = 0;

    //Forward
    for (int i=0; i<STEPS; i++){
        //récupérer la valeur du champ vectoriel en ce point
        vector_components = champVectoriel(step_coords[0],step_coords[1]);
        //Approximation du prochain point selon la direction du champ de vecteurs
        step_coords += STEPSIZE * vector_components;
        //récupérer la valeur de la texture en ce point
        if (verif(step_coords)){
            valeur += vec3(texture(tex,step_coords));
            nb_step_front += 1;
        }
    }

    step_coords = init_coords;

    //Backward
    for (int i=0; i<STEPS; i++){
        //récupérer la valeur du champ vectoriel en ce point
        vector_components = champVectoriel(step_coords[0],step_coords[1]);
        //Approximation du prochain point selon la direction du champ de vecteurs
        step_coords -= STEPSIZE * vector_components;
        //récupérer la valeur de la texture en ce point
        if (verif(step_coords)){
            valeur += vec3(texture(tex,step_coords));
            nb_step_back += 1;
        }
    }

    int nb_steps = nb_step_back + nb_step_front;

    if(nb_steps!=0){
        valeur = valeur / float(nb_steps);
    }
    else{
        valeur = vec3(texture(tex,TextCoords));
    }

    return valeur;

}

void main(){
    vec3 result = LIC();
//    vec3 result = vec3(texture(tex,TextCoords));
    FragColor = vec4(result, 1.0);
}
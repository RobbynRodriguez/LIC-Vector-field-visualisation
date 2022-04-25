#ifndef LEARNOPENGL_CAMERA_H
#define LEARNOPENGL_CAMERA_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>

// Options possibles pour le mouvement de caméra
enum Camera_Movement {
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT,
    UP,
    DOWN
};

// Valeurs par défaut
const float YAW         = -90.0f;
const float PITCH       =  0.0f;
const float SPEED       =  2.5f;
const float SENSITIVITY =  0.1f;
const float ZOOM        =  45.0f;


//Classe gérant la caméra
class Camera {

public:

    /*-----Attributs-----*/

    //Caméra
    glm::vec3 Position;
    glm::vec3 Front;
    glm::vec3 Up;
    glm::vec3 Right;
    glm::vec3 WorldUp;
    //Angles d'Euler
    float Yaw;
    float Pitch;
    //Options de caméra
    float MovementSpeed;
    float MouseSensitivity;
    float Zoom;


    /*-----Constructeurs-----*/

    //Constructeur avec vecteur
    Camera (glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), float yaw = YAW, float pitch = PITCH);

    //Constructeur avec scalaire
    Camera(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch);


    /*-----Getter-----*/

    //Getter de la matrice view utilisant les angles d'euler et la matrice lookat
    glm::mat4 GetViewMatrix(){
        return glm::lookAt(Position, Position + Front, Up);
    }

    //Getter de la position de la caméra
    glm::vec3 GetPosition (){
        return Position;
    }


    /*-----Processing-----*/

    //Appelé par process input pour bouger la caméra avec le clavier
    void ProcessKeyboard(Camera_Movement direction, float deltaTime);

    //Appelé par process input pour bouger la caméra selon le mouvement de la souris
    void ProcessMouseMovement(float xoffset, float yoffset, GLboolean constrainPitch = true);

    //Appelé par process input pour zoomer avec le scroll
    void ProcessMouseScroll(float yoffset);

private:

    //update les vecteurs de la caméra
    void updateCameraVectors();

};

#endif

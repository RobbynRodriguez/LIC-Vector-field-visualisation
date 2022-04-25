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
    Camera (glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), float yaw = YAW, float pitch = PITCH) : Front(glm::vec3(0.0f, 0.0f, -1.0f)), MovementSpeed(SPEED), MouseSensitivity(SENSITIVITY), Zoom(ZOOM) {
        Position = position;
        WorldUp = up;
        Yaw = yaw;
        Pitch = pitch;
        updateCameraVectors();
    }

    //Constructeur avec scalaire
    Camera(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch) : Front(glm::vec3(0.0f, 0.0f, -1.0f)), MovementSpeed(SPEED), MouseSensitivity(SENSITIVITY), Zoom(ZOOM) {
        Position = glm::vec3(posX, posY, posZ);
        WorldUp = glm::vec3(upX, upY, upZ);
        Yaw = yaw;
        Pitch = pitch;
        updateCameraVectors();
    }


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
    void ProcessKeyboard(Camera_Movement direction, float deltaTime) {
        float velocity = MovementSpeed * deltaTime;
        switch(direction){
            case FORWARD :
                Position += Front * velocity;
                break;
            case BACKWARD :
                Position -= Front * velocity;
                break;
            case LEFT :
                Position -= Right * velocity;
                break;
            case RIGHT :
                Position += Right * velocity;
                break;
            case UP :
                Position += Up * velocity;
                break;
            case DOWN :
                Position -= Up * velocity;
                break;
        }

    }

    //Appelé par process input pour bouger la caméra selon le mouvement de la souris
    void ProcessMouseMovement(float xoffset, float yoffset, GLboolean constrainPitch = true) {
        xoffset *= MouseSensitivity;
        yoffset *= MouseSensitivity;

        Yaw   += xoffset;
        Pitch += yoffset;

        if (constrainPitch){
            if (Pitch > 89.0f)
                Pitch = 89.0f;
            if (Pitch < -89.0f)
                Pitch = -89.0f;
        }

        updateCameraVectors();
    }

    //Appelé par process input pour zoomer avec le scroll
    void ProcessMouseScroll(float yoffset) {
        Zoom -= (float)yoffset;
        if (Zoom < 1.0f)
            Zoom = 1.0f;
        if (Zoom > 45.0f)
            Zoom = 45.0f;
    }


private:

    //update les vecteurs de la caméra
    void updateCameraVectors() {
        //Front vector
        glm::vec3 front;
        front.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
        front.y = sin(glm::radians(Pitch));
        front.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));
        Front = glm::normalize(front);
        //Right vector
        Right = glm::normalize(glm::cross(Front, WorldUp));
        //Up vector
        Up    = glm::normalize(glm::cross(Right, Front));
    }

};

#endif

#ifndef LEARNOPENGL_TEXTURE_H
#define LEARNOPENGL_TEXTURE_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include "Shader.h"
#include "../librairies/stb_image.h"
#include "Camera.h"

//Type du fichier de la texture
enum Code_texture {
    RGB,
    RGBA,
    RED,
    GREEN,
    BLUE,
    ALPHA
};

//Encapsulation de la création des textures dans une classe
class Texture {

public :

    //Création des textures
    unsigned int createTexture(const char *filename, Code_texture codeTexture) {

        //Créer la texture
        unsigned int texture;
        glGenTextures(1, &texture);
        //bind
        glBindTexture(GL_TEXTURE_2D, texture);
        //Options
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        //Récupérer les données de la texture
        int width, height, nrChannels;
        stbi_set_flip_vertically_on_load(true);
        unsigned char *data = stbi_load(filename, &width, &height, &nrChannels, 0);

        //Génération
        if (data) {
            switch (codeTexture) {
                case RGB :
                    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
                    break;
                case RGBA :
                    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
                    break;
                case RED :
                    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RED, GL_UNSIGNED_BYTE, data);
                    break;
                case GREEN :
                    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_GREEN, GL_UNSIGNED_BYTE, data);
                    break;
                case BLUE :
                    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_BLUE, GL_UNSIGNED_BYTE, data);
                    break;
                case ALPHA :
                    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_ALPHA, GL_UNSIGNED_BYTE, data);
                    break;
            }
            glGenerateMipmap(GL_TEXTURE_2D);
        } else {
            std::cout << "Failed to load texture" << std::endl;
        }

        //Free
        stbi_image_free(data);

        return texture;

    };
};

#endif //LEARNOPENGL_TEXTURE_H
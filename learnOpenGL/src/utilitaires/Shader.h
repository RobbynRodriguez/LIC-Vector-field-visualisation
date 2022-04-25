#ifndef LEARNOPENGL_SHADER_H
#define LEARNOPENGL_SHADER_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

//Classe pour gérer les shader
class Shader {

public:

    //Identifiant du shader
    unsigned int ID;

    //constructeur
    Shader(const char* vertexPath, const char* fragmentPath, const char* geometryPath = nullptr);

    //Verifie les erreurs
    void checkCompileErrors(GLuint shader, std::string type);


    //Activate the shader
    void use(){
        glUseProgram(ID);
    }

    //Fonctions utilitaires pour modifier les uniformes

    void setBool(const std::string &name, bool value) const{
        glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
    }

    void setInt(const std::string &name, int value) const{
        glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
    }

    void setFloat(const std::string &name, float value) const{
        glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
    }

    void setVec3(const std::string &name, const glm::vec3 &value) const{
        glUniform3fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]);
    }
    void setMat4 (const std::string &name, glm::mat4 mat4) {
        glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, glm::value_ptr(mat4));
    }

};

#endif
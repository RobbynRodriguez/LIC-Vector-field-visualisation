#include <filesystem>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>
#include <vector>
#include "utilitaires/Shader.h"
#include "librairies/stb_image.h"
#include "utilitaires/Camera.h"
#include "utilitaires/Texture.h"
#include "bezier/BezierCurve.h"
#include "utilitaires/Axes.h"
#include "utilitaires/Cube.h"
#include "bezier/BezierSurface.h"
#include "utilitaires/meshLic.h"
#include "utilitaires/FaceBasedVectorField.h"
#include "utilitaires/Sphere.h"

using namespace std;

/*-----VARIABLES GLOBALES-----*/

//Fenetre
const unsigned int WIN_WIDTH = 1400;
const unsigned int WIN_HEIGHT = 1000;

//Temps
float deltaTime = 0.0f;	// Time between current frame and last frame
float lastFrame = 0.0f; // Time of last frame
float der_time;

//Camera
Camera camera(glm::vec3(1.0f,1.0f,10.0f));
float lastX = WIN_WIDTH / 2.0f;
float lastY = WIN_HEIGHT / 2.0f;
bool firstMouse = true;

//Lumiere
glm::vec3 lightPos = glm::vec3(3,3,3);
bool lumiere_bouge;
bool normales = false;


/*-----FONCTIONS ANNEXES-----*/

//Appelée à chaque changement de taille de fenetre
void framebuffer_size_callback(GLFWwindow* window, int width, int height);

//Gestion des input clavier
void processInput(GLFWwindow *window);

//Gestion des mouvements souris
void mouse_callback(GLFWwindow* window, double xposIn, double yposIn);

//Gestion du scroll
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);

//Initialisation de la fenetre
GLFWwindow* initialisationFenetre ();


unsigned int loadTexture(char const * path);



/*-----MAIN-----*/

int main() {

    /*-----INIT-----*/

    //Initialisation glfw
    GLFWwindow* window = initialisationFenetre();
    if (window == NULL)
        return -1;

    //initialisation glad
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }


    /*-----LIGHT-----*/

    filesystem::path path = filesystem::current_path();
    path = path.parent_path();

    Shader lightingShader((path / "src/shaders/lighting.vs").c_str(), (path / "src/shaders/lighting.fs").c_str());
    Shader lampShader ((path / "src/shaders/lamp.vs").c_str(),(path / "src/shaders/lamp.fs").c_str());
    Shader normalShader ((path / "src/shaders/normal.vs").c_str(),(path / "src/shaders/normal.fs").c_str(), (path / "src/shaders/normal.gs").c_str());

    //Le shader à utiliser pour les objets éclairés
//    Shader lightingShader("/home/maud/Documents/M1/IGAI/TP/src/shaders/lighting.vs","/home/maud/Documents/M1/IGAI/TP/learnOpenGL/shaders/lighting.fs");
    //Le shader à utiliser pour la lumière
//    Shader lampShader ("/home/maud/Documents/M1/IGAI/TP/learnOpenGL/shaders/lamp.vs","/home/maud/Documents/M1/IGAI/TP/learnOpenGL/shaders/lamp.fs");
    //Un cube pour montrer l'effet de la lumière
    glm::vec3 cubePosition = glm::vec3(-2,0,1);
    Cube cubeTestLumiere;
    //Un cube pour faire lampe
    Cube lamp;


    /*-----AXES-----*/

    Axes axeX(0);
    Axes axeY(1);
    Axes axeZ(2);
    Shader shaderAxe ((path / "src/shaders/shaderAxes.vs").c_str(),(path / "src/shaders/shaderAxes.fs").c_str());
//    Shader shaderAxe ("/home/maud/Documents/M1/IGAI/TP/learnOpenGL/shaders/shaderAxes.vs","/home/maud/Documents/M1/IGAI/TP/learnOpenGL/shaders/shaderAxes.fs");


    /*-----COURBES DE BEZIER-----*/

    /*-----Points de controle-----*/

    glm::vec3 pointsControle1[] = {
            //Positions
            glm::vec3(-1.5f, 2.0f, 2.0f),
            glm::vec3(-0.5f, 2.0f, 1.0),
            glm::vec3(1.0f, 0.5f, -1.0f),
            glm::vec3(2.0f, 0.5f, -2.0f),
    };

    glm::vec3 pointsControle2[] = {
            glm::vec3(2.0f, 0.5f, -2.0f),
            glm::vec3(2.3f, 0.5f, -1.0),
            glm::vec3(1.5f, -0.5f, 0.0f),
            glm::vec3(1.0f, -1.5f, 1.0f),
    };

    unsigned int nbPointsControl = 4;
    unsigned int nbSegments = 50;
    float tailleSegment = 0.1;

    /*-----Nombre de segments uniforme-----*/
    BezierCurve bezierCurve1(pointsControle1, nbPointsControl, nbSegments);
    //BezierCurve bezierCurve2(pointsControle2,nbPointsControl,nbSegments);
    /*-----Taille de segment uniforme-----*/
    //BezierCurve bezierCurve1(pointsControle1,nbPointsControl,tailleSegment);
    BezierCurve bezierCurve2(pointsControle2,nbPointsControl,tailleSegment);
    //shader
    Shader shaderBezier ((path / "src/shaders/shaderBezierCurve.vs").c_str(),(path / "src/shaders/shaderBezierCurve.fs").c_str());
//    Shader shaderBezier("/home/maud/Documents/M1/IGAI/TP/learnOpenGL/shaders/shaderBezierCurve.vs", "/home/maud/Documents/M1/IGAI/TP/learnOpenGL/shaders/shaderBezierCurve.fs");


    /*----SURFACES DE BEZIER----*/

    /*-----Points de controle-----*/

    vector<vector<glm::vec3>> pointsControl;
    vector<glm::vec3> ligne1 = {
            glm::vec3(0.5,0.0,0.0),
            glm::vec3(0.4,-0.3,0.5),
            glm::vec3(0.3,-0.6,1),
            glm::vec3(0.2,-1,1.5)
    };
    vector<glm::vec3> ligne2 = {
            glm::vec3(1,0.3,0.0),
            glm::vec3(1,0.0,0.5),
            glm::vec3(1,-0.3,1),
            glm::vec3(1,-0.3,1.5)
    };
    vector<glm::vec3> ligne3 = {
            glm::vec3(1.5,0.5,0.0),
            glm::vec3(1.5,0.3,0.5),
            glm::vec3(1.5,0.0,1),
            glm::vec3(1.5,0.0,1.5)
    };
    vector<glm::vec3> ligne4 = {
            glm::vec3(2,0.0,0),
            glm::vec3(2,0.0,0.5),
            glm::vec3(2,0.0,1),
            glm::vec3(2,0.0,1.5)
    };

    pointsControl.push_back(ligne1);
    pointsControl.push_back(ligne2);
    pointsControl.push_back(ligne3);
    pointsControl.push_back(ligne4);

    unsigned int M = 4; //Lignes
    unsigned int N = 4; //colonnes

    BezierSurface bezierSurface(pointsControl,M,N);

    /*-----SPHERE-----*/

    Sphere sphere1;


    /*-----LIC-----*/

    /*-----Shader-----*/
    Shader licShader((path / "src/shaders/shader.vs").c_str(), (path / "src/shaders/shader.fs").c_str());

    /*------- Texture de bruit couleur --------*/
    unsigned int coloredNoise = loadTexture((path / "src/textures/coloredNoise.png").c_str());
    licShader.setInt("tex", 0);
    MeshLic meshLic(bezierSurface.pointsBezier,bezierSurface.indicesBezier);


    /*-----GLYPHE FACE BASED VECTOR FIELD-----*/
    Shader glypheShader((path / "src/shaders/glyphe.vs").c_str(), (path / "src/shaders/glyphe.fs").c_str(),(path / "src/shaders/glyphe.gs").c_str() );
    FaceBasedVectorField faceBasedVectorField(bezierSurface.pointsBezier,bezierSurface.indicesBezier);


    /*-----INITIALISATION AVANT BOUCLE-----*/

    //Test de profondeur
    glEnable(GL_DEPTH_TEST);


    /*-----BOUCLE DE RENDU-----*/

    //Afficher jusqu'à ce que la fenêtre soit fermée
    while(!glfwWindowShouldClose(window)) {
        processInput(window);

        //rendering
        glClearColor(0.3f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        //delta time
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // view/projection/model transformations
        glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)WIN_WIDTH / (float)WIN_HEIGHT, 0.1f, 100.0f);
        glm::mat4 view = camera.GetViewMatrix();
        glm::mat4 model = glm::mat4(1.0f);

        //AXES
        shaderAxe.use();
        shaderAxe.setMat4("projection", projection);
        shaderAxe.setMat4("view", view);
        shaderAxe.setMat4("model", model);
        axeX.DrawAxes(shaderAxe);
        axeY.DrawAxes(shaderAxe);
        axeZ.DrawAxes(shaderAxe);

        //CUBE TEST LUMIERE
        lightingShader.use();
        lightingShader.setVec3("objectColor", glm::vec3(0.3f, 0.7f, 0.7f));
        lightingShader.setVec3("lightColor", glm::vec3(1.0f, 1.0f, 1.0f));
        lightingShader.setVec3("lightPos", lightPos);
        lightingShader.setMat4("projection", projection);
        lightingShader.setMat4("view", view);

        // world transformation
        model = glm::translate(model, cubePosition);
        lightingShader.setMat4("model", model);
        // render the cube
        cubeTestLumiere.Draw(lightingShader);

        //LAMPE
        lampShader.use();
        lampShader.setMat4("projection", projection);
        lampShader.setMat4("view", view);
        if(lumiere_bouge) {
            der_time = der_time + deltaTime;
            float lightX = 2.0f * 2 * sin(der_time);
            float lightY = 5.0f;
            float lightZ = 1.5f * 3 * cos(der_time);
            lightPos = glm::vec3(lightX, lightY, lightZ);
        }
        model = glm::mat4(1.0f);
        model = glm::translate(model, lightPos);
        model = glm::scale(model, glm::vec3(0.2f)); // a smaller cube
        model = glm::rotate(model, (float)glfwGetTime() * glm::radians(50.0f), glm::vec3(0.5f, 1.0f, 0.0f));
        lampShader.setMat4("model", model);
        //render la lampe
        lamp.Draw(lampShader);

        //COURBES DE BEZIER
        shaderBezier.use();
        shaderBezier.setMat4("view", view);
        shaderBezier.setMat4("projection", projection);
        model = glm::mat4(1.0f);
        shaderBezier.setMat4("model", model);

        //Dessin des polynomes de controle
        shaderBezier.setVec3("objectColor",glm::vec3(0.7,0.7,0.7));
        bezierCurve1.DrawPointsControle(shaderBezier);
        bezierCurve1.DrawSegments(shaderBezier);
        bezierCurve2.DrawPointsControle(shaderBezier);
        bezierCurve2.DrawSegments(shaderBezier);

        //Dessin des courbes
        shaderBezier.setVec3("objectColor",glm::vec3(0,0,0));
        bezierCurve1.DrawBezierCurve(shaderBezier);
        bezierCurve2.DrawBezierCurve(shaderBezier);

        //SURFACE DE BEZIER
        model = glm::translate(model, glm::vec3(3,0,0));
        shaderBezier.setMat4("model", model);

        //Dessin des points de controle
        shaderBezier.setVec3("objectColor",glm::vec3(0,0,0));
        bezierSurface.drawPointsControl(shaderBezier);

        //Dessin de la surface
        lightingShader.use();
        lightingShader.setMat4("model", model);
        lightingShader.setVec3("objectColor",glm::vec3(0.3f, 0.6f, 0.5f));
        bezierSurface.drawBezierSurface(shaderBezier);


        //LIC
        licShader.use();
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(3,3,0));
        licShader.setMat4("model",model);
        licShader.setMat4("view", view);
        licShader.setMat4("projection", projection);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, coloredNoise);

        licShader.setInt("STEPS",10);
        licShader.setFloat("STEPSIZE",0.001);
        meshLic.draw(licShader);

        if(normales){
            normalShader.use();
            normalShader.setMat4("projection", projection);
            normalShader.setMat4("view", view);
            normalShader.setMat4("model", model);
            meshLic.draw(normalShader);
        }

        //FACE BASED VECTOR FIELD
        lightingShader.use();
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(3.0,0.0,6.0));
        lightingShader.setMat4("model", model);
        faceBasedVectorField.draw(lightingShader);
        glypheShader.use();
        glypheShader.setMat4("model",model);
        glypheShader.setMat4("view", view);
        glypheShader.setMat4("projection",projection);
        faceBasedVectorField.draw(glypheShader);

        //Sphere
        lightingShader.use();
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(-2.0,4.0,0.0));
        lightingShader.setMat4("model", model);
        sphere1.draw(lightingShader);
        glypheShader.use();
        glypheShader.setMat4("model",model);
        glypheShader.setMat4("view", view);
        glypheShader.setMat4("projection",projection);
        sphere1.draw(glypheShader);

        // check and call events and swap the buffers
        glfwPollEvents();
        glfwSwapBuffers(window);
    }

    //Free les ressources
    bezierCurve1.FreeResources();
    bezierCurve2.FreeResources();
    bezierSurface.FreeResources();
    axeX.FreeResources();
    axeY.FreeResources();
    axeZ.FreeResources();
    cubeTestLumiere.FreeResources();
    lamp.FreeResources();
    meshLic.freeResources();
    sphere1.freeResources();

    glfwTerminate();

    return 0;
}


/*-----IMPLEMENTATION FONCTIONS ANNEXES-----*/


//Appelée à chaque changement de taille de fenetre
void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}


//Gestion des input clavier
void processInput(GLFWwindow *window) {

    //Si la touche échap est enfoncée on ferme la fenêtre
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    //Si la touche z est enfoncée on passe en mode wireframe
    if(glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS)
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    //Si la touche x est enfoncée on passe en fill mode
    if(glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS)
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    //Controle de la caméra avec les touches zqsdae
    const float cameraSpeed = 2.5f * deltaTime;
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.ProcessKeyboard(FORWARD,deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.ProcessKeyboard(BACKWARD,deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.ProcessKeyboard(LEFT,deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.ProcessKeyboard(RIGHT,deltaTime);
    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
        camera.ProcessKeyboard(DOWN,deltaTime);
    if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
        camera.ProcessKeyboard(UP,deltaTime);
    if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS)
        lumiere_bouge = true;
    if (glfwGetKey(window, GLFW_KEY_0) == GLFW_PRESS)
        lumiere_bouge = false;
    if (glfwGetKey(window, GLFW_KEY_N) == GLFW_PRESS)
        normales = true;
    if (glfwGetKey(window, GLFW_KEY_B) == GLFW_PRESS)
        normales = false;

    //Supprimer le contrôle via la souris en appuyant sur o
    if (glfwGetKey(window, GLFW_KEY_O) == GLFW_PRESS){
        glfwSetCursorPosCallback(window, NULL);
        glfwSetScrollCallback(window, NULL);
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    }

    //Mettre le contrôle via la souris en appuyant sur p
    if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS){
        glfwSetCursorPosCallback(window, mouse_callback);
        glfwSetScrollCallback(window, scroll_callback);
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    }

}


//Gestion des mouvements souris
void mouse_callback(GLFWwindow* window, double xposIn, double yposIn) {
    float xpos = static_cast<float>(xposIn);
    float ypos = static_cast<float>(yposIn);

    if (firstMouse) {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos;

    lastX = xpos;
    lastY = ypos;

    camera.ProcessMouseMovement(xoffset, yoffset);
}


//gestion du scroll
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset){
    camera.ProcessMouseScroll(static_cast<float>(yoffset));
}

//initialisation de la fenetre
GLFWwindow* initialisationFenetre (){

    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    //Création de la fenêtre
    GLFWwindow* window = glfwCreateWindow(WIN_WIDTH, WIN_HEIGHT, "My First Window", NULL, NULL);
    if (window == NULL) {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return NULL;
    }

    glfwMakeContextCurrent(window);
    //Appeler la fonction framebuffer_size_callback à chaque resize de la fenêtre
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    //Si on veut commencer en gérant la caméra avec la souris
    /*glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);*/

    return window;
}


//chargement des textures
unsigned int loadTexture(char const * path)
{
    unsigned int textureID;
    glGenTextures(1, &textureID);

    int width, height, nrComponents;
    unsigned char *data = stbi_load(path, &width, &height, &nrComponents, 0);
    if (data)
    {
        GLenum format;
        if (nrComponents == 1)
            format = GL_RED;
        else if (nrComponents == 3)
            format = GL_RGB;
        else if (nrComponents == 4)
            format = GL_RGBA;

        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        stbi_image_free(data);
    }
    else
    {
        std::cout << "Texture failed to load at path: " << path << std::endl;
        stbi_image_free(data);
    }

    return textureID;
}



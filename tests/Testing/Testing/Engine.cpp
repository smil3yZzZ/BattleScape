#include "Engine.hpp"

const int Engine::squareWidth = 64;
const int Engine::platformHeight = 64;
const int Engine::wallHeight = 32;

// Engine constructor
Engine::Engine(double framesPerSecond, int** map, int dimension, int bufferVertexSize, int verticesPerQuad)
{
    Engine::framesPerSecond = framesPerSecond;
    Engine::map = map;
    Engine::dimension = dimension;
    Engine::bufferVertexSize = bufferVertexSize;
    Engine::verticesPerQuad = verticesPerQuad;

    isRunning = false;

    frameDelay = 1/framesPerSecond;
    counter = 0;
}

int Engine::run() {

    isRunning = true;

    if (!glfwInit())
    {
        fprintf(stderr, "Failed to initialize GLFW\n");
        return -1;
    }

    // OpenGL version 3.3
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    
    GLFWwindow* window = glfwCreateWindow(800, 600, "Killer", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);

    /*
    double lastTime = glfwGetTime();
    double deltaTime = 0.0f;
    int nbFrames = 0;
    */

    float* vertices = new float[ * BUFFER_VERTEX_SIZE * VERTICES_PER_QUAD];

    while (!glfwWindowShouldClose(window)) {

        /*
        double currentTime = glfwGetTime();
        
        
        deltaTime += (currentTime - lastTime) * FPS;
        lastTime = currentTime;
        nbFrames++;
        

        while(deltaTime >= 1.0f) {
            printf("%f ms/frame\n", 1000.0 / double(nbFrames));
            nbFrames = 0;
            update();
            --deltaTime;
        }
        render();
        */

        double oldTime = glfwGetTime();

        update();
        render();

        double frameTime = glfwGetTime() - oldTime;

        if (frameTime < frameDelay) {
            Sleep(1000*(frameDelay - frameTime));
            std::cout << 1000 * (frameDelay - frameTime) << std::endl;
        }

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    return 1;
}

int Engine::render() {
    //std::cout << deltaTime << std::endl;
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    return 1;
}

int Engine::update() {
    std::cout << "Updating: " << (counter++) << std::endl;
    return 1;
}

int Engine::getIsRunning() {
    return isRunning;
}

void Engine::setIsRunning(int isRunning) {
    Engine::isRunning = isRunning;
}
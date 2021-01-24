#include "Engine.hpp"
#define STB_IMAGE_IMPLEMENTATION
#include "utils/stb_image.h"

const int Engine::squareWidth = 64;
const int Engine::platformHeight = 64;
const int Engine::wallHeight = 32;

void processKeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);

// Engine constructor
Engine::Engine(double framesPerSecond, int** map, int** wallMap, int dimension, int bufferVertexSize, int bufferVertexTexturesSize,
    int verticesPerQuad, int textureVerticesPerQuad, int indicesPerQuad, int textureIndicesPerQuad, int quadWidth, int quadHeight,
    int screenWidth, int screenHeight, float viewportWidth, float viewportHeight, float xOrigin, float yOrigin,
    int numberOfPlatforms, int numberOfRgbaChannels)
{
    Engine::framesPerSecond = framesPerSecond;
    Engine::map = map;
    Engine::wallMap = wallMap;
    Engine::dimension = dimension;

    Engine::bufferVertexSize = bufferVertexSize;
    Engine::bufferVertexTexturesSize = bufferVertexTexturesSize;
    Engine::verticesPerQuad = verticesPerQuad;
    Engine::textureVerticesPerQuad = textureVerticesPerQuad;
    Engine::indicesPerQuad = indicesPerQuad;
    Engine::textureIndicesPerQuad = textureIndicesPerQuad;

    Engine::quadWidth = quadWidth;
    Engine::quadHeight = quadHeight;

    Engine::input = Input();

    isRunning = false;

    frameDelay = 1/framesPerSecond;
    counter = 0;

    up = right = down = left = 0;

    Engine::platformVertices = new float[dimension * dimension * bufferVertexSize * verticesPerQuad];

    Engine::platformIndices = new unsigned int[dimension * dimension * indicesPerQuad];

    Engine::VAO = Engine::VBO = Engine::EBO = 0;

    Engine::wallVertices = new float[dimension * dimension * bufferVertexTexturesSize * textureVerticesPerQuad];

    Engine::wallIndices = new unsigned int[dimension * dimension * textureIndicesPerQuad];

    Engine::textureVAO = Engine::textureVBO = Engine::textureEBO;

    //Engine::wallShadowVertices = ;

    //Engine::wallShadowIndices = ;

    Engine::numOfWallTextureFiles = 0;

    Engine::screenWidth = screenWidth;
    Engine::screenHeight = screenHeight;

    Engine::viewportWidth = viewportWidth;
    Engine::viewportHeight = viewportHeight;

    Engine::xOrigin = xOrigin;
    Engine::yOrigin = yOrigin;

    Engine::numberOfPlatforms = numberOfPlatforms;
    Engine::numberOfRgbaChannels = numberOfRgbaChannels;
}

int Engine::run() {

    isRunning = true;

    char readBuffer[65536];

    FILE* fColors = fopen("resources/colors.json", "r");

    rapidjson::FileReadStream isColors(fColors, readBuffer, sizeof(readBuffer));

    rapidjson::Document colors;
    colors.ParseStream(isColors);

    fclose(fColors);

    FILE* fWalls = fopen("resources/walls.json", "r");

    rapidjson::FileReadStream isWalls(fWalls, readBuffer, sizeof(readBuffer));

    rapidjson::Document walls;
    walls.ParseStream(isWalls);

    fclose(fWalls);

    init(colors, walls);

    double oldTime = glfwGetTime();

    double frameTime = frameDelay;

    while (!glfwWindowShouldClose(window)) {

        if (frameTime >= frameDelay) {
            oldTime = glfwGetTime();

            update();

            clearScreen();

            render();

            checkCamera();

            glfwSwapBuffers(window);

            glfwPollEvents();
        }

        frameTime = glfwGetTime() - oldTime;

        /*
        double oldTime = glfwGetTime();

        update(colors);

        clearScreen();

        checkCamera();

        render();

        double frameTime = glfwGetTime() - oldTime;

        if (frameTime < frameDelay) {
            Sleep(1000*(frameDelay - frameTime));
        }
        */


    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);

    glDeleteVertexArrays(1, &textureVAO);
    glDeleteBuffers(1, &textureVBO);
    glDeleteBuffers(1, &textureEBO);

    delete[] platformVertices;
    delete[] wallVertices;
    //delete[] wallShadowVertices;
    delete[] platformIndices;
    delete[] wallIndices;
    //delete[] wallShadowIndices;


    glfwTerminate();

    return 1;
}



int Engine::init(const rapidjson::Document& colors, const rapidjson::Document& walls) {
    initGL();
    initTextures(colors);
    initMaze(colors, walls);
    initCamera();
    initShaders();
    generateBuffers();
    return 1;
}

int Engine::initGL() {
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

    window = glfwCreateWindow(screenWidth, screenHeight, "Killer", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);


    //glViewport(0.0f, 0.0f, screenWidth, screenHeight);

    glfwSetWindowUserPointer(window, this);
    glfwSetKeyCallback(window, processKeyCallback);
    glfwSetInputMode(window, GLFW_STICKY_KEYS, GLFW_TRUE);

    glewExperimental = GL_TRUE;

    glewInit();

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_SMOOTH);
    glDepthFunc(GL_LESS);

    return 1;
}

int Engine::initMaze(const rapidjson::Document& colors, const rapidjson::Document& walls) {

    for (int i = 0; i < dimension; i++) {
        for (int j = 0; j < dimension; j++) {
            int color = map[dimension - i - 1][j];
            // PLATFORMS //

            float top = ((float)color + 1.0f)/(float)numberOfPlatforms;
            float bottom = (float)color/(float)numberOfPlatforms;// - 1.0f/(float)quadHeight;

            platformVertices[i * dimension * bufferVertexSize * verticesPerQuad + bufferVertexSize * verticesPerQuad * j] = j * quadWidth;
            platformVertices[i * dimension * bufferVertexSize * verticesPerQuad + bufferVertexSize * verticesPerQuad * j + 1] = i * quadHeight;
            platformVertices[i * dimension * bufferVertexSize * verticesPerQuad + bufferVertexSize * verticesPerQuad * j + 2] = 5.0f;
            platformVertices[i * dimension * bufferVertexSize * verticesPerQuad + bufferVertexSize * verticesPerQuad * j + 3] = 0.0f;
            platformVertices[i * dimension * bufferVertexSize * verticesPerQuad + bufferVertexSize * verticesPerQuad * j + 4] = bottom;//(quadHeight*numberOfPlatforms*(colors[color_char]["codePrimary"]["r"].GetInt()/(quadHeight*numberOfPlatforms/quadWidth)) + quadWidth)/wallTextureWidth - (0.5f/wallTextureWidth);

            platformVertices[i * dimension * bufferVertexSize * verticesPerQuad + bufferVertexSize * verticesPerQuad * j + 5] = j * quadWidth;
            platformVertices[i * dimension * bufferVertexSize * verticesPerQuad + bufferVertexSize * verticesPerQuad * j + 6] = i * quadHeight + quadHeight;
            platformVertices[i * dimension * bufferVertexSize * verticesPerQuad + bufferVertexSize * verticesPerQuad * j + 7] = 5.0f;
            platformVertices[i * dimension * bufferVertexSize * verticesPerQuad + bufferVertexSize * verticesPerQuad * j + 8] = 0.0f;
            platformVertices[i * dimension * bufferVertexSize * verticesPerQuad + bufferVertexSize * verticesPerQuad * j + 9] = top;

            platformVertices[i * dimension * bufferVertexSize * verticesPerQuad + bufferVertexSize * verticesPerQuad * j + 10] = j * quadWidth + quadWidth;
            platformVertices[i * dimension * bufferVertexSize * verticesPerQuad + bufferVertexSize * verticesPerQuad * j + 11] = i * quadHeight;
            platformVertices[i * dimension * bufferVertexSize * verticesPerQuad + bufferVertexSize * verticesPerQuad * j + 12] = 5.0f;
            platformVertices[i * dimension * bufferVertexSize * verticesPerQuad + bufferVertexSize * verticesPerQuad * j + 13] = 1.0f;
            platformVertices[i * dimension * bufferVertexSize * verticesPerQuad + bufferVertexSize * verticesPerQuad * j + 14] = bottom;

            platformVertices[i * dimension * bufferVertexSize * verticesPerQuad + bufferVertexSize * verticesPerQuad * j + 15] = j * quadWidth + quadWidth;
            platformVertices[i * dimension * bufferVertexSize * verticesPerQuad + bufferVertexSize * verticesPerQuad * j + 16] = i * quadHeight + quadHeight;
            platformVertices[i * dimension * bufferVertexSize * verticesPerQuad + bufferVertexSize * verticesPerQuad * j + 17] = 5.0f;
            platformVertices[i * dimension * bufferVertexSize * verticesPerQuad + bufferVertexSize * verticesPerQuad * j + 18] = 1.0f;
            platformVertices[i * dimension * bufferVertexSize * verticesPerQuad + bufferVertexSize * verticesPerQuad * j + 19] = top;

            //Set indices here
            platformIndices[i * dimension * indicesPerQuad + indicesPerQuad * j] = i * dimension * verticesPerQuad + verticesPerQuad * j;
            platformIndices[i * dimension * indicesPerQuad + indicesPerQuad * j + 1] = i * dimension * verticesPerQuad + verticesPerQuad * j + 1;
            platformIndices[i * dimension * indicesPerQuad + indicesPerQuad * j + 2] = i * dimension * verticesPerQuad + verticesPerQuad * j + 2;
            platformIndices[i * dimension * indicesPerQuad + indicesPerQuad * j + 3] = i * dimension * verticesPerQuad + verticesPerQuad * j + 1;
            platformIndices[i * dimension * indicesPerQuad + indicesPerQuad * j + 4] = i * dimension * verticesPerQuad + verticesPerQuad * j + 2;
            platformIndices[i * dimension * indicesPerQuad + indicesPerQuad * j + 5] = i * dimension * verticesPerQuad + verticesPerQuad * j + 3;


            // WALLS //
            // Lower sprite

            if (wallMap[dimension - i - 1][j] != -1) {
                wallVertices[i * dimension * bufferVertexTexturesSize * textureVerticesPerQuad + bufferVertexTexturesSize * textureVerticesPerQuad * j] = j * quadWidth + quadWidth;
                wallVertices[i * dimension * bufferVertexTexturesSize * textureVerticesPerQuad + bufferVertexTexturesSize * textureVerticesPerQuad * j + 1] = i * quadHeight + (quadHeight / 2);
                wallVertices[i * dimension * bufferVertexTexturesSize * textureVerticesPerQuad + bufferVertexTexturesSize * textureVerticesPerQuad * j + 2] = 10.0f;
                wallVertices[i * dimension * bufferVertexTexturesSize * textureVerticesPerQuad + bufferVertexTexturesSize * textureVerticesPerQuad * j + 3] = 0.0f;
                wallVertices[i * dimension * bufferVertexTexturesSize * textureVerticesPerQuad + bufferVertexTexturesSize * textureVerticesPerQuad * j + 4] = 0.0f;
                wallVertices[i * dimension * bufferVertexTexturesSize * textureVerticesPerQuad + bufferVertexTexturesSize * textureVerticesPerQuad * j + 5] = 0.0f;
                wallVertices[i * dimension * bufferVertexTexturesSize * textureVerticesPerQuad + bufferVertexTexturesSize * textureVerticesPerQuad * j + 6] = (wallTextureWidth*(walls[wallMap[dimension - i - 1][j]]["lower"].GetInt()/(wallTextureWidth/quadWidth)) + quadWidth)/wallTextureWidth - (0.5f/wallTextureWidth);
                wallVertices[i * dimension * bufferVertexTexturesSize * textureVerticesPerQuad + bufferVertexTexturesSize * textureVerticesPerQuad * j + 7] = 1.0f;

                wallVertices[i * dimension * bufferVertexTexturesSize * textureVerticesPerQuad + bufferVertexTexturesSize * textureVerticesPerQuad * j + 8] = j * quadWidth + quadWidth;
                wallVertices[i * dimension * bufferVertexTexturesSize * textureVerticesPerQuad + bufferVertexTexturesSize * textureVerticesPerQuad * j + 9] = i * quadHeight;
                wallVertices[i * dimension * bufferVertexTexturesSize * textureVerticesPerQuad + bufferVertexTexturesSize * textureVerticesPerQuad * j + 10] = 10.0f;
                wallVertices[i * dimension * bufferVertexTexturesSize * textureVerticesPerQuad + bufferVertexTexturesSize * textureVerticesPerQuad * j + 11] = 0.0f;
                wallVertices[i * dimension * bufferVertexTexturesSize * textureVerticesPerQuad + bufferVertexTexturesSize * textureVerticesPerQuad * j + 12] = 0.0f;
                wallVertices[i * dimension * bufferVertexTexturesSize * textureVerticesPerQuad + bufferVertexTexturesSize * textureVerticesPerQuad * j + 13] = 0.0f;
                wallVertices[i * dimension * bufferVertexTexturesSize * textureVerticesPerQuad + bufferVertexTexturesSize * textureVerticesPerQuad * j + 14] = (wallTextureWidth*(walls[wallMap[dimension - i - 1][j]]["lower"].GetInt()/(wallTextureWidth/quadWidth)) + quadWidth)/wallTextureWidth - (0.5f/wallTextureWidth);
                wallVertices[i * dimension * bufferVertexTexturesSize * textureVerticesPerQuad + bufferVertexTexturesSize * textureVerticesPerQuad * j + 15] = 0.0f;

                wallVertices[i * dimension * bufferVertexTexturesSize * textureVerticesPerQuad + bufferVertexTexturesSize * textureVerticesPerQuad * j + 16] = j * quadWidth;
                wallVertices[i * dimension * bufferVertexTexturesSize * textureVerticesPerQuad + bufferVertexTexturesSize * textureVerticesPerQuad * j + 17] = i * quadHeight;
                wallVertices[i * dimension * bufferVertexTexturesSize * textureVerticesPerQuad + bufferVertexTexturesSize * textureVerticesPerQuad * j + 18] = 10.0f;
                wallVertices[i * dimension * bufferVertexTexturesSize * textureVerticesPerQuad + bufferVertexTexturesSize * textureVerticesPerQuad * j + 19] = 0.0f;
                wallVertices[i * dimension * bufferVertexTexturesSize * textureVerticesPerQuad + bufferVertexTexturesSize * textureVerticesPerQuad * j + 20] = 0.0f;
                wallVertices[i * dimension * bufferVertexTexturesSize * textureVerticesPerQuad + bufferVertexTexturesSize * textureVerticesPerQuad * j + 21] = 0.0f;
                wallVertices[i * dimension * bufferVertexTexturesSize * textureVerticesPerQuad + bufferVertexTexturesSize * textureVerticesPerQuad * j + 22] = wallTextureWidth*(walls[wallMap[dimension - i - 1][j]]["lower"].GetInt()/(wallTextureWidth/quadWidth))/wallTextureWidth + (0.5f/wallTextureWidth);
                wallVertices[i * dimension * bufferVertexTexturesSize * textureVerticesPerQuad + bufferVertexTexturesSize * textureVerticesPerQuad * j + 23] = 0.0f;

                wallVertices[i * dimension * bufferVertexTexturesSize * textureVerticesPerQuad + bufferVertexTexturesSize * textureVerticesPerQuad * j + 24] = j * quadWidth;
                wallVertices[i * dimension * bufferVertexTexturesSize * textureVerticesPerQuad + bufferVertexTexturesSize * textureVerticesPerQuad * j + 25] = i * quadHeight + (quadHeight / 2);
                wallVertices[i * dimension * bufferVertexTexturesSize * textureVerticesPerQuad + bufferVertexTexturesSize * textureVerticesPerQuad * j + 26] = 10.0f;
                wallVertices[i * dimension * bufferVertexTexturesSize * textureVerticesPerQuad + bufferVertexTexturesSize * textureVerticesPerQuad * j + 27] = 0.0f;
                wallVertices[i * dimension * bufferVertexTexturesSize * textureVerticesPerQuad + bufferVertexTexturesSize * textureVerticesPerQuad * j + 28] = 0.0f;
                wallVertices[i * dimension * bufferVertexTexturesSize * textureVerticesPerQuad + bufferVertexTexturesSize * textureVerticesPerQuad * j + 29] = 0.0f;
                wallVertices[i * dimension * bufferVertexTexturesSize * textureVerticesPerQuad + bufferVertexTexturesSize * textureVerticesPerQuad * j + 30] = wallTextureWidth*(walls[wallMap[dimension - i - 1][j]]["lower"].GetInt()/(wallTextureWidth/quadWidth))/wallTextureWidth + (0.5f/wallTextureWidth);
                wallVertices[i * dimension * bufferVertexTexturesSize * textureVerticesPerQuad + bufferVertexTexturesSize * textureVerticesPerQuad * j + 31] = 1.0f;

                wallIndices[i * dimension * textureIndicesPerQuad + textureIndicesPerQuad * j] = i * dimension * textureVerticesPerQuad + textureVerticesPerQuad * j;
                wallIndices[i * dimension * textureIndicesPerQuad + textureIndicesPerQuad * j + 1] = i * dimension * textureVerticesPerQuad + textureVerticesPerQuad * j + 1;
                wallIndices[i * dimension * textureIndicesPerQuad + textureIndicesPerQuad * j + 2] = i * dimension * textureVerticesPerQuad + textureVerticesPerQuad * j + 3;
                wallIndices[i * dimension * textureIndicesPerQuad + textureIndicesPerQuad * j + 3] = i * dimension * textureVerticesPerQuad + textureVerticesPerQuad * j + 1;
                wallIndices[i * dimension * textureIndicesPerQuad + textureIndicesPerQuad * j + 4] = i * dimension * textureVerticesPerQuad + textureVerticesPerQuad * j + 2;
                wallIndices[i * dimension * textureIndicesPerQuad + textureIndicesPerQuad * j + 5] = i * dimension * textureVerticesPerQuad + textureVerticesPerQuad * j + 3;

                // Upper sprite

                wallVertices[i * dimension * bufferVertexTexturesSize * textureVerticesPerQuad + bufferVertexTexturesSize * textureVerticesPerQuad * j + 32] = j * quadWidth + quadWidth;
                wallVertices[i * dimension * bufferVertexTexturesSize * textureVerticesPerQuad + bufferVertexTexturesSize * textureVerticesPerQuad * j + 33] = i * quadHeight + (quadHeight / 2) + (quadHeight / 2);
                wallVertices[i * dimension * bufferVertexTexturesSize * textureVerticesPerQuad + bufferVertexTexturesSize * textureVerticesPerQuad * j + 34] = 10.0f;
                wallVertices[i * dimension * bufferVertexTexturesSize * textureVerticesPerQuad + bufferVertexTexturesSize * textureVerticesPerQuad * j + 35] = 0.0f;
                wallVertices[i * dimension * bufferVertexTexturesSize * textureVerticesPerQuad + bufferVertexTexturesSize * textureVerticesPerQuad * j + 36] = 0.0f;
                wallVertices[i * dimension * bufferVertexTexturesSize * textureVerticesPerQuad + bufferVertexTexturesSize * textureVerticesPerQuad * j + 37] = 0.0f;
                wallVertices[i * dimension * bufferVertexTexturesSize * textureVerticesPerQuad + bufferVertexTexturesSize * textureVerticesPerQuad * j + 38] = (wallTextureWidth*(walls[wallMap[dimension - i - 1][j]]["upper"].GetInt()/(wallTextureWidth/quadWidth)) + quadWidth)/wallTextureWidth - (0.5f/wallTextureWidth);
                wallVertices[i * dimension * bufferVertexTexturesSize * textureVerticesPerQuad + bufferVertexTexturesSize * textureVerticesPerQuad * j + 39] = 1.0f;

                wallVertices[i * dimension * bufferVertexTexturesSize * textureVerticesPerQuad + bufferVertexTexturesSize * textureVerticesPerQuad * j + 40] = j * quadWidth + quadWidth;
                wallVertices[i * dimension * bufferVertexTexturesSize * textureVerticesPerQuad + bufferVertexTexturesSize * textureVerticesPerQuad * j + 41] = i * quadHeight + (quadHeight / 2);
                wallVertices[i * dimension * bufferVertexTexturesSize * textureVerticesPerQuad + bufferVertexTexturesSize * textureVerticesPerQuad * j + 42] = 10.0f;
                wallVertices[i * dimension * bufferVertexTexturesSize * textureVerticesPerQuad + bufferVertexTexturesSize * textureVerticesPerQuad * j + 43] = 0.0f;
                wallVertices[i * dimension * bufferVertexTexturesSize * textureVerticesPerQuad + bufferVertexTexturesSize * textureVerticesPerQuad * j + 44] = 0.0f;
                wallVertices[i * dimension * bufferVertexTexturesSize * textureVerticesPerQuad + bufferVertexTexturesSize * textureVerticesPerQuad * j + 45] = 0.0f;
                wallVertices[i * dimension * bufferVertexTexturesSize * textureVerticesPerQuad + bufferVertexTexturesSize * textureVerticesPerQuad * j + 46] = (wallTextureWidth*(walls[wallMap[dimension - i - 1][j]]["upper"].GetInt()/(wallTextureWidth/quadWidth)) + quadWidth)/wallTextureWidth - (0.5f/wallTextureWidth);
                wallVertices[i * dimension * bufferVertexTexturesSize * textureVerticesPerQuad + bufferVertexTexturesSize * textureVerticesPerQuad * j + 47] = 0.0f;

                wallVertices[i * dimension * bufferVertexTexturesSize * textureVerticesPerQuad + bufferVertexTexturesSize * textureVerticesPerQuad * j + 48] = j * quadWidth;
                wallVertices[i * dimension * bufferVertexTexturesSize * textureVerticesPerQuad + bufferVertexTexturesSize * textureVerticesPerQuad * j + 49] = i * quadHeight + (quadHeight / 2);
                wallVertices[i * dimension * bufferVertexTexturesSize * textureVerticesPerQuad + bufferVertexTexturesSize * textureVerticesPerQuad * j + 50] = 10.0f;
                wallVertices[i * dimension * bufferVertexTexturesSize * textureVerticesPerQuad + bufferVertexTexturesSize * textureVerticesPerQuad * j + 51] = 0.0f;
                wallVertices[i * dimension * bufferVertexTexturesSize * textureVerticesPerQuad + bufferVertexTexturesSize * textureVerticesPerQuad * j + 52] = 0.0f;
                wallVertices[i * dimension * bufferVertexTexturesSize * textureVerticesPerQuad + bufferVertexTexturesSize * textureVerticesPerQuad * j + 53] = 0.0f;
                wallVertices[i * dimension * bufferVertexTexturesSize * textureVerticesPerQuad + bufferVertexTexturesSize * textureVerticesPerQuad * j + 54] = wallTextureWidth*(walls[wallMap[dimension - i - 1][j]]["upper"].GetInt()/(wallTextureWidth/quadWidth))/wallTextureWidth + (0.5f/wallTextureWidth);
                wallVertices[i * dimension * bufferVertexTexturesSize * textureVerticesPerQuad + bufferVertexTexturesSize * textureVerticesPerQuad * j + 55] = 0.0f;

                wallVertices[i * dimension * bufferVertexTexturesSize * textureVerticesPerQuad + bufferVertexTexturesSize * textureVerticesPerQuad * j + 56] = j * quadWidth;
                wallVertices[i * dimension * bufferVertexTexturesSize * textureVerticesPerQuad + bufferVertexTexturesSize * textureVerticesPerQuad * j + 57] = i * quadHeight + (quadHeight / 2) + (quadHeight / 2);
                wallVertices[i * dimension * bufferVertexTexturesSize * textureVerticesPerQuad + bufferVertexTexturesSize * textureVerticesPerQuad * j + 58] = 10.0f;
                wallVertices[i * dimension * bufferVertexTexturesSize * textureVerticesPerQuad + bufferVertexTexturesSize * textureVerticesPerQuad * j + 59] = 0.0f;
                wallVertices[i * dimension * bufferVertexTexturesSize * textureVerticesPerQuad + bufferVertexTexturesSize * textureVerticesPerQuad * j + 60] = 0.0f;
                wallVertices[i * dimension * bufferVertexTexturesSize * textureVerticesPerQuad + bufferVertexTexturesSize * textureVerticesPerQuad * j + 61] = 0.0f;
                wallVertices[i * dimension * bufferVertexTexturesSize * textureVerticesPerQuad + bufferVertexTexturesSize * textureVerticesPerQuad * j + 62] = wallTextureWidth*(walls[wallMap[dimension - i - 1][j]]["upper"].GetInt()/(wallTextureWidth/quadWidth))/wallTextureWidth + (0.5f/wallTextureWidth);
                wallVertices[i * dimension * bufferVertexTexturesSize * textureVerticesPerQuad + bufferVertexTexturesSize * textureVerticesPerQuad * j + 63] = 1.0f;

                wallIndices[i * dimension * textureIndicesPerQuad + textureIndicesPerQuad * j + 6] = i * dimension * textureVerticesPerQuad + textureVerticesPerQuad * j + 4;
                wallIndices[i * dimension * textureIndicesPerQuad + textureIndicesPerQuad * j + 7] = i * dimension * textureVerticesPerQuad + textureVerticesPerQuad * j + 5;
                wallIndices[i * dimension * textureIndicesPerQuad + textureIndicesPerQuad * j + 8] = i * dimension * textureVerticesPerQuad + textureVerticesPerQuad * j + 7;
                wallIndices[i * dimension * textureIndicesPerQuad + textureIndicesPerQuad * j + 9] = i * dimension * textureVerticesPerQuad + textureVerticesPerQuad * j + 5;
                wallIndices[i * dimension * textureIndicesPerQuad + textureIndicesPerQuad * j + 10] = i * dimension * textureVerticesPerQuad + textureVerticesPerQuad * j + 6;
                wallIndices[i * dimension * textureIndicesPerQuad + textureIndicesPerQuad * j + 11] = i * dimension * textureVerticesPerQuad + textureVerticesPerQuad * j + 7;
            }


        }
    }

    return 1;
}

int Engine::initCamera() {
    //projection = glm::ortho(-272.0f, 800.0f, -72.0f, 600.0f, -100.0f, 100.0f);
    //projection = glm::ortho(-100.0f, 400.0f, -50.0f, 300.0f, -100.0f, 100.0f);
    projection = glm::ortho(xOrigin, viewportWidth, yOrigin, viewportHeight, -100.0f, 100.0f);
    model = glm::mat4(1.0f);

    cameraPos = glm::vec3(0.0f, 0.0f, 100.0f);
    cameraTarget = glm::vec3(0.0f, 0.0f, 0.0f);
    glm::vec3 cameraDirection = glm::normalize(cameraPos - cameraTarget);
    glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
    glm::vec3 cameraRight = glm::normalize(glm::cross(up, cameraDirection));
    cameraUp = glm::cross(cameraDirection, cameraRight);


    view = glm::lookAt(cameraPos, cameraTarget, cameraUp);
    return 1;
}

int Engine::initShaders() {
    Engine::colorShader = Shader("shaders/squareColors.vs", "shaders/squareColors.fs");
    Engine::wallShader = Shader("shaders/squareTextures.vs", "shaders/squareTextures.fs");
    Engine::platformShader = Shader("shaders/squarePlatformTextures.vs", "shaders/squarePlatformTextures.fs");
    return 1;
}

int Engine::generateBuffers() {
    //Platforms
    glGenVertexArrays(1, &VAO);

    glGenBuffers(1, &VBO);

    glGenBuffers(1, &EBO);

    //Walls

    glGenVertexArrays(1, &textureVAO);

    glGenBuffers(1, &textureVBO);

    glGenBuffers(1, &textureEBO);

    return 1;
}

int Engine::initTextures(const rapidjson::Document& colors) {

    //Colors
    unsigned char * pixels = new unsigned char[numberOfPlatforms * quadWidth * quadHeight * numberOfRgbaChannels];

    for (int n = 0; n < numberOfPlatforms; n++) {
        for (int i = 0; i < quadHeight; i++) {
            for (int j = 0; j < quadWidth; j++) {
                if (i < 2 || j < 2 || i > 61 || j > 61) {
                    pixels[n * quadHeight * quadWidth * numberOfRgbaChannels + i * quadWidth * numberOfRgbaChannels + j * numberOfRgbaChannels] = 157.0f;
                    pixels[n * quadHeight * quadWidth * numberOfRgbaChannels + i * quadWidth * numberOfRgbaChannels + j * numberOfRgbaChannels + 1] = 157.0f;
                    pixels[n * quadHeight * quadWidth * numberOfRgbaChannels + i * quadWidth * numberOfRgbaChannels + j * numberOfRgbaChannels + 2] = 157.0f;
                    pixels[n * quadHeight * quadWidth * numberOfRgbaChannels + i * quadWidth * numberOfRgbaChannels + j * numberOfRgbaChannels + 3] = 1.0f;
                }
                else {
                    float randomNumber = (float) rand()/RAND_MAX;
                    float red, green, blue, diff;
                    if (randomNumber > 0.9) {
                        pixels[n * quadHeight * quadWidth * numberOfRgbaChannels + i * quadWidth * numberOfRgbaChannels + j * numberOfRgbaChannels] = 0.0f;
                        pixels[n * quadHeight * quadWidth * numberOfRgbaChannels + i * quadWidth * numberOfRgbaChannels + j * numberOfRgbaChannels + 1] = 0.0f;
                        pixels[n * quadHeight * quadWidth * numberOfRgbaChannels + i * quadWidth * numberOfRgbaChannels + j * numberOfRgbaChannels + 2] = 0.0f;
                        pixels[n * quadHeight * quadWidth * numberOfRgbaChannels + i * quadWidth * numberOfRgbaChannels + j * numberOfRgbaChannels + 3] = 1.0f;
                    }
                    else {
                        std::string color_string = std::to_string(n);
                        const char* color_char = color_string.c_str();
                        if (colors[color_char]["codePrimary"]["r"].GetInt() != colors[color_char]["codeSecondary"]["r"].GetInt()) {
                            diff = (float)abs(colors[color_char]["codePrimary"]["r"].GetInt() - colors[color_char]["codeSecondary"]["r"].GetInt()) * randomNumber;
                            red = colors[color_char]["codePrimary"]["r"].GetInt() < colors[color_char]["codeSecondary"]["r"].GetInt() ? colors[color_char]["codePrimary"]["r"].GetInt() + diff : colors[color_char]["codeSecondary"]["r"].GetInt() + diff;
                        }
                        else {
                            red = colors[color_char]["codePrimary"]["r"].GetInt();
                        }
                        if (colors[color_char]["codePrimary"]["g"].GetInt() != colors[color_char]["codeSecondary"]["g"].GetInt()) {
                            diff = (float)abs(colors[color_char]["codePrimary"]["g"].GetInt() - colors[color_char]["codeSecondary"]["g"].GetInt()) * randomNumber;
                            green = colors[color_char]["codePrimary"]["g"].GetInt() < colors[color_char]["codeSecondary"]["g"].GetInt() ? colors[color_char]["codePrimary"]["g"].GetInt() + diff : colors[color_char]["codeSecondary"]["g"].GetInt() + diff;
                        }
                        else {
                            green = colors[color_char]["codePrimary"]["g"].GetInt();
                        }
                        if (colors[color_char]["codePrimary"]["b"].GetInt() != colors[color_char]["codeSecondary"]["b"].GetInt()) {
                            diff = (float)abs(colors[color_char]["codePrimary"]["b"].GetInt() - colors[color_char]["codeSecondary"]["b"].GetInt()) * randomNumber;
                            blue = colors[color_char]["codePrimary"]["b"].GetInt() < colors[color_char]["codeSecondary"]["b"].GetInt() ? colors[color_char]["codePrimary"]["b"].GetInt() + diff : colors[color_char]["codeSecondary"]["b"].GetInt() + diff;
                        }
                        else {
                            blue = colors[color_char]["codePrimary"]["b"].GetInt();
                        }
                        pixels[n * quadHeight * quadWidth * numberOfRgbaChannels + i * quadWidth * numberOfRgbaChannels + j * numberOfRgbaChannels] = red;
                        pixels[n * quadHeight * quadWidth * numberOfRgbaChannels + i * quadWidth * numberOfRgbaChannels + j * numberOfRgbaChannels + 1] = green;
                        pixels[n * quadHeight * quadWidth * numberOfRgbaChannels + i * quadWidth * numberOfRgbaChannels + j * numberOfRgbaChannels + 2] = blue;
                        pixels[n * quadHeight * quadWidth * numberOfRgbaChannels + i * quadWidth * numberOfRgbaChannels + j * numberOfRgbaChannels + 3] = 1.0f;
                    }
                }
            }
        }
    }

    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glGenTextures(1, &colorTexture);
    glBindTexture(GL_TEXTURE_2D, colorTexture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, quadWidth, quadHeight*numberOfPlatforms, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels);
    glGenerateMipmap(GL_TEXTURE_2D);

    //Corregir 03 y 24 en muros. Añadir sombras, ver si en el negro compensa meterlo más realista...
    //Al meter personaje, igualar Y a Z. (profundidad --> Ver cómo solucionarlo)


    //Walls

    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture); // all upcoming GL_TEXTURE_2D operations now have effect on this texture object
    // set the texture wrapping parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);	// set texture wrapping to GL_REPEAT (default wrapping method)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    // set texture filtering parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // load image, create texture and generate mipmaps
    int width, height, nrChannels;

    stbi_set_flip_vertically_on_load(true);
    unsigned char* data = stbi_load("resources/wall.png", &width, &height, &nrChannels, 0);

    wallTextureWidth = width;
    if (data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        if(stbi_failure_reason())
            std::cout << stbi_failure_reason() << std::endl;
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(data);


    return 1;
}

int Engine::clearScreen() {
    glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
    return 1;
}

int Engine::update() {
    updateBuffers();
    return 1;
}

int Engine::updateBuffers() {

    // Platforms

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    int vertices_size = sizeof(float) * dimension * dimension * verticesPerQuad * bufferVertexSize;
    int indices_size = sizeof(unsigned int) * dimension * dimension * indicesPerQuad;

    glBufferData(GL_ARRAY_BUFFER, vertices_size, platformVertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);

    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices_size, platformIndices, GL_STATIC_DRAW);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, bufferVertexSize * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // gradient attribute
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, bufferVertexSize * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // Walls

    int walls_vertices_size = sizeof(float) * dimension * dimension * textureVerticesPerQuad * bufferVertexTexturesSize;
    int walls_indices_size = sizeof(unsigned int) * dimension * dimension * textureIndicesPerQuad;

    glBindVertexArray(textureVAO);

    glBindBuffer(GL_ARRAY_BUFFER, textureVBO);

    glBufferData(GL_ARRAY_BUFFER, walls_vertices_size, wallVertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, textureEBO);

    glBufferData(GL_ELEMENT_ARRAY_BUFFER, walls_indices_size, wallIndices, GL_STATIC_DRAW);


    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // color attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    // texture coord attribute
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    return 1;

}

int Engine::render() {

    platformShader.use();
    platformShader.setFloatMatrix("projection", glm::value_ptr(projection));
    platformShader.setFloatMatrix("view", glm::value_ptr(view));
    platformShader.setFloatMatrix("model", glm::value_ptr(model));
    //colorShader.setFloat("randomValue", )

    glBindVertexArray(VAO);
    glBindTexture(GL_TEXTURE_2D, colorTexture);
    glDrawElements(GL_TRIANGLES, indicesPerQuad * dimension * dimension, GL_UNSIGNED_INT, 0);
    //glDrawElements(GL_TRIANGLES, 600, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);

    wallShader.use();

    wallShader.setFloatMatrix("projection", glm::value_ptr(projection));
    wallShader.setFloatMatrix("view", glm::value_ptr(view));
    wallShader.setFloatMatrix("model", glm::value_ptr(model));

    glBindVertexArray(textureVAO);
    glBindTexture(GL_TEXTURE_2D, texture);
    glDrawElements(GL_TRIANGLES, textureIndicesPerQuad * dimension * dimension, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);

    return 1;
}

int Engine::getIsRunning() {
    return isRunning;
}

void Engine::setIsRunning(int isRunning) {
    Engine::isRunning = isRunning;
}

void Engine::setView(glm::mat4 view) {
    Engine::view = view;
}

Shader Engine::getColorShader() {
    return colorShader;
}


void Engine::checkCamera() {
    float x = 0;
    float y = 0;

    //Añadir aqui frameTime;
    if (input.getKeyState(INPUT_UP)) {
        y += 4.0f;
        yOrigin += 4.0f;
    }
    if (input.getKeyState(INPUT_RIGHT)) {
        x += 4.0f;
        xOrigin += 4.0f;
    }
    if (input.getKeyState(INPUT_DOWN)) {
        y -= 4.0f;
        yOrigin -= 4.0f;
    }
    if (input.getKeyState(INPUT_LEFT)) {
        x -= 4.0f;
        xOrigin -= 4.0f;
    }


    cameraPos += glm::vec3(x, y, 0.0f);
    cameraTarget += glm::vec3(x, y, 0.0f);

    setView(glm::lookAt(cameraPos, cameraTarget, cameraUp));
}

void Engine::updateInput(int key, int action) {

    if (action == GLFW_PRESS){
        switch (key) {
            case GLFW_KEY_ESCAPE:
                //Will be changed to "pause"
                glfwSetWindowShouldClose(window, true);
            case GLFW_KEY_UP:
                input.setKeyState(INPUT_UP, 1);
                break;
            case GLFW_KEY_RIGHT:
                input.setKeyState(INPUT_RIGHT, 1);
                break;
            case GLFW_KEY_DOWN:
                input.setKeyState(INPUT_DOWN, 1);
                break;
            case GLFW_KEY_LEFT:
                input.setKeyState(INPUT_LEFT, 1);
                break;
        }
    }
    if (action == GLFW_RELEASE){
        switch (key) {
            case GLFW_KEY_UP:
                input.setKeyState(INPUT_UP, 0);
                break;
            case GLFW_KEY_RIGHT:
                input.setKeyState(INPUT_RIGHT, 0);
                break;
            case GLFW_KEY_DOWN:
                input.setKeyState(INPUT_DOWN, 0);
                break;
            case GLFW_KEY_LEFT:
                input.setKeyState(INPUT_LEFT, 0);
                break;
        }
    }
}


void processKeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    Engine* engine = (Engine*)glfwGetWindowUserPointer(window);

    /*
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }
    else {
        engine->updateInput(key, action);
    }
    */
    engine->updateInput(key, action);
}

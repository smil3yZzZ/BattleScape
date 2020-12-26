#include "Engine.hpp"
#define STB_IMAGE_IMPLEMENTATION
#include "utils/stb_image.h"

const int Engine::squareWidth = 64;
const int Engine::platformHeight = 64;
const int Engine::wallHeight = 32;

void processKeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);

// Engine constructor
Engine::Engine(double framesPerSecond, int** map, int** wallMap, int dimension, int bufferVertexSize, int bufferVertexTexturesSize,
    int verticesPerQuad, int textureVerticesPerQuad, int indicesPerQuad, int textureIndicesPerQuad, int quadWidth, int quadHeight)
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

    Engine::vertices = new float[dimension * dimension * bufferVertexSize * verticesPerQuad];

    Engine::indices = new unsigned int[dimension * dimension * indicesPerQuad];

    Engine::VAO = Engine::VBO = Engine::EBO = 0;

    Engine::textureVertices = new float[dimension * dimension * bufferVertexTexturesSize * textureVerticesPerQuad];

    Engine::textureIndices = new unsigned int[dimension * dimension * textureIndicesPerQuad];

    Engine::textureVAO = Engine::textureVBO = Engine::textureEBO;

    Engine::numOfWallTextureFiles = 0;
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

    delete[] vertices;
    delete[] textureVertices;
    delete[] indices;
    delete[] textureIndices;


    glfwTerminate();

    return 1;
}



int Engine::init(const rapidjson::Document& colors, const rapidjson::Document& walls) {
    initGL();
    initTextures();
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

    window = glfwCreateWindow(800, 600, "Killer", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);


    //glViewport(0, 0, 800, 600);

    glfwSetWindowUserPointer(window, this);
    glfwSetKeyCallback(window, processKeyCallback);
    glfwSetInputMode(window, GLFW_STICKY_KEYS, GLFW_TRUE);

    glewExperimental = GL_TRUE;

    glewInit();

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_ALWAYS);

    return 1;
}

int Engine::initMaze(const rapidjson::Document& colors, const rapidjson::Document& walls) {


    std::cout << (wallTextureWidth*(0/(wallTextureWidth/quadWidth)))/wallTextureWidth << std::endl;

    std::cout << (wallTextureWidth*(1/(wallTextureWidth/quadWidth)))/wallTextureWidth << std::endl;
    std::cout << (wallTextureWidth*(2/(wallTextureWidth/quadWidth)))/wallTextureWidth << std::endl;
    std::cout << (wallTextureWidth*(3/(wallTextureWidth/quadWidth)))/wallTextureWidth << std::endl;
    std::cout << (wallTextureWidth*(4/(wallTextureWidth/quadWidth)))/wallTextureWidth << std::endl;
    std::cout << (wallTextureWidth*(5/(wallTextureWidth/quadWidth)))/wallTextureWidth << std::endl;
    std::cout << (wallTextureWidth*(6/(wallTextureWidth/quadWidth)))/wallTextureWidth << std::endl;
    std::cout << (wallTextureWidth*(7/(wallTextureWidth/quadWidth)))/wallTextureWidth << std::endl;

    std::cout << (wallTextureWidth*(8/(wallTextureWidth/quadWidth)))/wallTextureWidth << std::endl;

    std::cout << (wallTextureWidth*(9/(wallTextureWidth/quadWidth)))/wallTextureWidth << std::endl;
    std::cout << (wallTextureWidth*(10/(wallTextureWidth/quadWidth)))/wallTextureWidth << std::endl;
    std::cout << (wallTextureWidth*(11/(wallTextureWidth/quadWidth)))/wallTextureWidth << std::endl;

    for (int i = 0; i < dimension; i++) {
        for (int j = 0; j < dimension; j++) {
            int color = map[dimension - i - 1][j];
            std::string color_string = std::to_string(color);
            const char* color_char = color_string.c_str();

            // PLATFORMS //
            vertices[i * dimension * bufferVertexSize * verticesPerQuad + bufferVertexSize * verticesPerQuad * j] = j * quadWidth;
            vertices[i * dimension * bufferVertexSize * verticesPerQuad + bufferVertexSize * verticesPerQuad * j + 1] = i * quadHeight;
            vertices[i * dimension * bufferVertexSize * verticesPerQuad + bufferVertexSize * verticesPerQuad * j + 2] = 0.0f;
            vertices[i * dimension * bufferVertexSize * verticesPerQuad + bufferVertexSize * verticesPerQuad * j + 3] = colors[color_char]["code"]["r"].GetInt();
            vertices[i * dimension * bufferVertexSize * verticesPerQuad + bufferVertexSize * verticesPerQuad * j + 4] = colors[color_char]["code"]["g"].GetInt();
            vertices[i * dimension * bufferVertexSize * verticesPerQuad + bufferVertexSize * verticesPerQuad * j + 5] = colors[color_char]["code"]["b"].GetInt();

            vertices[i * dimension * bufferVertexSize * verticesPerQuad + bufferVertexSize * verticesPerQuad * j + 6] = j * quadWidth;
            vertices[i * dimension * bufferVertexSize * verticesPerQuad + bufferVertexSize * verticesPerQuad * j + 7] = i * quadHeight + quadHeight;
            vertices[i * dimension * bufferVertexSize * verticesPerQuad + bufferVertexSize * verticesPerQuad * j + 8] = 0.0f;
            vertices[i * dimension * bufferVertexSize * verticesPerQuad + bufferVertexSize * verticesPerQuad * j + 9] = colors[color_char]["code"]["r"].GetInt();
            vertices[i * dimension * bufferVertexSize * verticesPerQuad + bufferVertexSize * verticesPerQuad * j + 10] = colors[color_char]["code"]["g"].GetInt();
            vertices[i * dimension * bufferVertexSize * verticesPerQuad + bufferVertexSize * verticesPerQuad * j + 11] = colors[color_char]["code"]["b"].GetInt();

            vertices[i * dimension * bufferVertexSize * verticesPerQuad + bufferVertexSize * verticesPerQuad * j + 12] = j * quadWidth + quadWidth;
            vertices[i * dimension * bufferVertexSize * verticesPerQuad + bufferVertexSize * verticesPerQuad * j + 13] = i * quadHeight;
            vertices[i * dimension * bufferVertexSize * verticesPerQuad + bufferVertexSize * verticesPerQuad * j + 14] = 0.0f;
            vertices[i * dimension * bufferVertexSize * verticesPerQuad + bufferVertexSize * verticesPerQuad * j + 15] = colors[color_char]["code"]["r"].GetInt();
            vertices[i * dimension * bufferVertexSize * verticesPerQuad + bufferVertexSize * verticesPerQuad * j + 16] = colors[color_char]["code"]["g"].GetInt();
            vertices[i * dimension * bufferVertexSize * verticesPerQuad + bufferVertexSize * verticesPerQuad * j + 17] = colors[color_char]["code"]["b"].GetInt();

            vertices[i * dimension * bufferVertexSize * verticesPerQuad + bufferVertexSize * verticesPerQuad * j + 18] = j * quadWidth + quadWidth;
            vertices[i * dimension * bufferVertexSize * verticesPerQuad + bufferVertexSize * verticesPerQuad * j + 19] = i * quadHeight + quadHeight;
            vertices[i * dimension * bufferVertexSize * verticesPerQuad + bufferVertexSize * verticesPerQuad * j + 20] = 0.0f;
            vertices[i * dimension * bufferVertexSize * verticesPerQuad + bufferVertexSize * verticesPerQuad * j + 21] = colors[color_char]["code"]["r"].GetInt();
            vertices[i * dimension * bufferVertexSize * verticesPerQuad + bufferVertexSize * verticesPerQuad * j + 22] = colors[color_char]["code"]["g"].GetInt();
            vertices[i * dimension * bufferVertexSize * verticesPerQuad + bufferVertexSize * verticesPerQuad * j + 23] = colors[color_char]["code"]["b"].GetInt();



                std::cout << "TESTING TEX COORDS:"<< std::endl;
                std::cout << (wallTextureWidth*(walls[wallMap[dimension - i - 1][j]]["lower"].GetInt()/(wallTextureWidth/quadWidth)) + quadWidth)/wallTextureWidth << std::endl;
                std::cout << (wallTextureWidth*(walls[wallMap[dimension - i - 1][j]]["upper"].GetInt()/(wallTextureWidth/quadWidth)) + quadWidth)/wallTextureWidth << std::endl;

                // Check -1 here!

            //Set indices here
            indices[i * dimension * indicesPerQuad + indicesPerQuad * j] = i * dimension * verticesPerQuad + verticesPerQuad * j;
            indices[i * dimension * indicesPerQuad + indicesPerQuad * j + 1] = i * dimension * verticesPerQuad + verticesPerQuad * j + 1;
            indices[i * dimension * indicesPerQuad + indicesPerQuad * j + 2] = i * dimension * verticesPerQuad + verticesPerQuad * j + 2;
            indices[i * dimension * indicesPerQuad + indicesPerQuad * j + 3] = i * dimension * verticesPerQuad + verticesPerQuad * j + 1;
            indices[i * dimension * indicesPerQuad + indicesPerQuad * j + 4] = i * dimension * verticesPerQuad + verticesPerQuad * j + 2;
            indices[i * dimension * indicesPerQuad + indicesPerQuad * j + 5] = i * dimension * verticesPerQuad + verticesPerQuad * j + 3;

            // WALLS //
            // Lower sprite

            textureVertices[i * dimension * bufferVertexTexturesSize * textureVerticesPerQuad + bufferVertexTexturesSize * textureVerticesPerQuad * j] = j * quadWidth + quadWidth;
            textureVertices[i * dimension * bufferVertexTexturesSize * textureVerticesPerQuad + bufferVertexTexturesSize * textureVerticesPerQuad * j + 1] = i * quadHeight + (quadHeight / 2);
            textureVertices[i * dimension * bufferVertexTexturesSize * textureVerticesPerQuad + bufferVertexTexturesSize * textureVerticesPerQuad * j + 2] = 0.0f;
            textureVertices[i * dimension * bufferVertexTexturesSize * textureVerticesPerQuad + bufferVertexTexturesSize * textureVerticesPerQuad * j + 3] = 0.0f;
            textureVertices[i * dimension * bufferVertexTexturesSize * textureVerticesPerQuad + bufferVertexTexturesSize * textureVerticesPerQuad * j + 4] = 0.0f;
            textureVertices[i * dimension * bufferVertexTexturesSize * textureVerticesPerQuad + bufferVertexTexturesSize * textureVerticesPerQuad * j + 5] = 0.0f;
            textureVertices[i * dimension * bufferVertexTexturesSize * textureVerticesPerQuad + bufferVertexTexturesSize * textureVerticesPerQuad * j + 6] = (wallTextureWidth*(walls[wallMap[dimension - i - 1][j]]["lower"].GetInt()/(wallTextureWidth/quadWidth)) + quadWidth)/wallTextureWidth;
            textureVertices[i * dimension * bufferVertexTexturesSize * textureVerticesPerQuad + bufferVertexTexturesSize * textureVerticesPerQuad * j + 7] = 1.0f;

            textureVertices[i * dimension * bufferVertexTexturesSize * textureVerticesPerQuad + bufferVertexTexturesSize * textureVerticesPerQuad * j + 8] = j * quadWidth + quadWidth;
            textureVertices[i * dimension * bufferVertexTexturesSize * textureVerticesPerQuad + bufferVertexTexturesSize * textureVerticesPerQuad * j + 9] = i * quadHeight;
            textureVertices[i * dimension * bufferVertexTexturesSize * textureVerticesPerQuad + bufferVertexTexturesSize * textureVerticesPerQuad * j + 10] = 0.0f;
            textureVertices[i * dimension * bufferVertexTexturesSize * textureVerticesPerQuad + bufferVertexTexturesSize * textureVerticesPerQuad * j + 11] = 0.0f;
            textureVertices[i * dimension * bufferVertexTexturesSize * textureVerticesPerQuad + bufferVertexTexturesSize * textureVerticesPerQuad * j + 12] = 0.0f;
            textureVertices[i * dimension * bufferVertexTexturesSize * textureVerticesPerQuad + bufferVertexTexturesSize * textureVerticesPerQuad * j + 13] = 0.0f;
            textureVertices[i * dimension * bufferVertexTexturesSize * textureVerticesPerQuad + bufferVertexTexturesSize * textureVerticesPerQuad * j + 14] = (wallTextureWidth*(walls[wallMap[dimension - i - 1][j]]["lower"].GetInt()/(wallTextureWidth/quadWidth)) + quadWidth)/wallTextureWidth;
            textureVertices[i * dimension * bufferVertexTexturesSize * textureVerticesPerQuad + bufferVertexTexturesSize * textureVerticesPerQuad * j + 15] = 0.0f;

            textureVertices[i * dimension * bufferVertexTexturesSize * textureVerticesPerQuad + bufferVertexTexturesSize * textureVerticesPerQuad * j + 16] = j * quadWidth;
            textureVertices[i * dimension * bufferVertexTexturesSize * textureVerticesPerQuad + bufferVertexTexturesSize * textureVerticesPerQuad * j + 17] = i * quadHeight;
            textureVertices[i * dimension * bufferVertexTexturesSize * textureVerticesPerQuad + bufferVertexTexturesSize * textureVerticesPerQuad * j + 18] = 0.0f;
            textureVertices[i * dimension * bufferVertexTexturesSize * textureVerticesPerQuad + bufferVertexTexturesSize * textureVerticesPerQuad * j + 19] = 0.0f;
            textureVertices[i * dimension * bufferVertexTexturesSize * textureVerticesPerQuad + bufferVertexTexturesSize * textureVerticesPerQuad * j + 20] = 0.0f;
            textureVertices[i * dimension * bufferVertexTexturesSize * textureVerticesPerQuad + bufferVertexTexturesSize * textureVerticesPerQuad * j + 21] = 0.0f;
            textureVertices[i * dimension * bufferVertexTexturesSize * textureVerticesPerQuad + bufferVertexTexturesSize * textureVerticesPerQuad * j + 22] = wallTextureWidth*(walls[wallMap[dimension - i - 1][j]]["lower"].GetInt()/(wallTextureWidth/quadWidth))/wallTextureWidth;
            textureVertices[i * dimension * bufferVertexTexturesSize * textureVerticesPerQuad + bufferVertexTexturesSize * textureVerticesPerQuad * j + 23] = 0.0f;

            textureVertices[i * dimension * bufferVertexTexturesSize * textureVerticesPerQuad + bufferVertexTexturesSize * textureVerticesPerQuad * j + 24] = j * quadWidth;
            textureVertices[i * dimension * bufferVertexTexturesSize * textureVerticesPerQuad + bufferVertexTexturesSize * textureVerticesPerQuad * j + 25] = i * quadHeight + (quadHeight / 2);
            textureVertices[i * dimension * bufferVertexTexturesSize * textureVerticesPerQuad + bufferVertexTexturesSize * textureVerticesPerQuad * j + 26] = 0.0f;
            textureVertices[i * dimension * bufferVertexTexturesSize * textureVerticesPerQuad + bufferVertexTexturesSize * textureVerticesPerQuad * j + 27] = 0.0f;
            textureVertices[i * dimension * bufferVertexTexturesSize * textureVerticesPerQuad + bufferVertexTexturesSize * textureVerticesPerQuad * j + 28] = 0.0f;
            textureVertices[i * dimension * bufferVertexTexturesSize * textureVerticesPerQuad + bufferVertexTexturesSize * textureVerticesPerQuad * j + 29] = 0.0f;
            textureVertices[i * dimension * bufferVertexTexturesSize * textureVerticesPerQuad + bufferVertexTexturesSize * textureVerticesPerQuad * j + 30] = wallTextureWidth*(walls[wallMap[dimension - i - 1][j]]["lower"].GetInt()/(wallTextureWidth/quadWidth))/wallTextureWidth;
            textureVertices[i * dimension * bufferVertexTexturesSize * textureVerticesPerQuad + bufferVertexTexturesSize * textureVerticesPerQuad * j + 31] = 1.0f;

            textureIndices[i * dimension * textureIndicesPerQuad + textureIndicesPerQuad * j] = i * dimension * textureVerticesPerQuad + textureVerticesPerQuad * j;
            textureIndices[i * dimension * textureIndicesPerQuad + textureIndicesPerQuad * j + 1] = i * dimension * textureVerticesPerQuad + textureVerticesPerQuad * j + 1;
            textureIndices[i * dimension * textureIndicesPerQuad + textureIndicesPerQuad * j + 2] = i * dimension * textureVerticesPerQuad + textureVerticesPerQuad * j + 3;
            textureIndices[i * dimension * textureIndicesPerQuad + textureIndicesPerQuad * j + 3] = i * dimension * textureVerticesPerQuad + textureVerticesPerQuad * j + 1;
            textureIndices[i * dimension * textureIndicesPerQuad + textureIndicesPerQuad * j + 4] = i * dimension * textureVerticesPerQuad + textureVerticesPerQuad * j + 2;
            textureIndices[i * dimension * textureIndicesPerQuad + textureIndicesPerQuad * j + 5] = i * dimension * textureVerticesPerQuad + textureVerticesPerQuad * j + 3;

            // Upper sprite

            textureVertices[i * dimension * bufferVertexTexturesSize * textureVerticesPerQuad + bufferVertexTexturesSize * textureVerticesPerQuad * j + 32] = j * quadWidth + quadWidth;
            textureVertices[i * dimension * bufferVertexTexturesSize * textureVerticesPerQuad + bufferVertexTexturesSize * textureVerticesPerQuad * j + 33] = i * quadHeight + (quadHeight / 2) + (quadHeight / 2);
            textureVertices[i * dimension * bufferVertexTexturesSize * textureVerticesPerQuad + bufferVertexTexturesSize * textureVerticesPerQuad * j + 34] = 0.0f;
            textureVertices[i * dimension * bufferVertexTexturesSize * textureVerticesPerQuad + bufferVertexTexturesSize * textureVerticesPerQuad * j + 35] = 0.0f;
            textureVertices[i * dimension * bufferVertexTexturesSize * textureVerticesPerQuad + bufferVertexTexturesSize * textureVerticesPerQuad * j + 36] = 0.0f;
            textureVertices[i * dimension * bufferVertexTexturesSize * textureVerticesPerQuad + bufferVertexTexturesSize * textureVerticesPerQuad * j + 37] = 0.0f;
            textureVertices[i * dimension * bufferVertexTexturesSize * textureVerticesPerQuad + bufferVertexTexturesSize * textureVerticesPerQuad * j + 38] = (wallTextureWidth*(walls[wallMap[dimension - i - 1][j]]["upper"].GetInt()/(wallTextureWidth/quadWidth)) + quadWidth)/wallTextureWidth;
            textureVertices[i * dimension * bufferVertexTexturesSize * textureVerticesPerQuad + bufferVertexTexturesSize * textureVerticesPerQuad * j + 39] = 1.0f;

            textureVertices[i * dimension * bufferVertexTexturesSize * textureVerticesPerQuad + bufferVertexTexturesSize * textureVerticesPerQuad * j + 40] = j * quadWidth + quadWidth;
            textureVertices[i * dimension * bufferVertexTexturesSize * textureVerticesPerQuad + bufferVertexTexturesSize * textureVerticesPerQuad * j + 41] = i * quadHeight + (quadHeight / 2);
            textureVertices[i * dimension * bufferVertexTexturesSize * textureVerticesPerQuad + bufferVertexTexturesSize * textureVerticesPerQuad * j + 42] = 0.0f;
            textureVertices[i * dimension * bufferVertexTexturesSize * textureVerticesPerQuad + bufferVertexTexturesSize * textureVerticesPerQuad * j + 43] = 0.0f;
            textureVertices[i * dimension * bufferVertexTexturesSize * textureVerticesPerQuad + bufferVertexTexturesSize * textureVerticesPerQuad * j + 44] = 0.0f;
            textureVertices[i * dimension * bufferVertexTexturesSize * textureVerticesPerQuad + bufferVertexTexturesSize * textureVerticesPerQuad * j + 45] = 0.0f;
            textureVertices[i * dimension * bufferVertexTexturesSize * textureVerticesPerQuad + bufferVertexTexturesSize * textureVerticesPerQuad * j + 46] = (wallTextureWidth*(walls[wallMap[dimension - i - 1][j]]["upper"].GetInt()/(wallTextureWidth/quadWidth)) + quadWidth)/wallTextureWidth;
            textureVertices[i * dimension * bufferVertexTexturesSize * textureVerticesPerQuad + bufferVertexTexturesSize * textureVerticesPerQuad * j + 47] = 0.0f;

            textureVertices[i * dimension * bufferVertexTexturesSize * textureVerticesPerQuad + bufferVertexTexturesSize * textureVerticesPerQuad * j + 48] = j * quadWidth;
            textureVertices[i * dimension * bufferVertexTexturesSize * textureVerticesPerQuad + bufferVertexTexturesSize * textureVerticesPerQuad * j + 49] = i * quadHeight + (quadHeight / 2);
            textureVertices[i * dimension * bufferVertexTexturesSize * textureVerticesPerQuad + bufferVertexTexturesSize * textureVerticesPerQuad * j + 50] = 0.0f;
            textureVertices[i * dimension * bufferVertexTexturesSize * textureVerticesPerQuad + bufferVertexTexturesSize * textureVerticesPerQuad * j + 51] = 0.0f;
            textureVertices[i * dimension * bufferVertexTexturesSize * textureVerticesPerQuad + bufferVertexTexturesSize * textureVerticesPerQuad * j + 52] = 0.0f;
            textureVertices[i * dimension * bufferVertexTexturesSize * textureVerticesPerQuad + bufferVertexTexturesSize * textureVerticesPerQuad * j + 53] = 0.0f;
            textureVertices[i * dimension * bufferVertexTexturesSize * textureVerticesPerQuad + bufferVertexTexturesSize * textureVerticesPerQuad * j + 54] = wallTextureWidth*(walls[wallMap[dimension - i - 1][j]]["upper"].GetInt()/(wallTextureWidth/quadWidth))/wallTextureWidth;
            textureVertices[i * dimension * bufferVertexTexturesSize * textureVerticesPerQuad + bufferVertexTexturesSize * textureVerticesPerQuad * j + 55] = 0.0f;

            textureVertices[i * dimension * bufferVertexTexturesSize * textureVerticesPerQuad + bufferVertexTexturesSize * textureVerticesPerQuad * j + 56] = j * quadWidth;
            textureVertices[i * dimension * bufferVertexTexturesSize * textureVerticesPerQuad + bufferVertexTexturesSize * textureVerticesPerQuad * j + 57] = i * quadHeight + (quadHeight / 2) + (quadHeight / 2);
            textureVertices[i * dimension * bufferVertexTexturesSize * textureVerticesPerQuad + bufferVertexTexturesSize * textureVerticesPerQuad * j + 58] = 0.0f;
            textureVertices[i * dimension * bufferVertexTexturesSize * textureVerticesPerQuad + bufferVertexTexturesSize * textureVerticesPerQuad * j + 59] = 0.0f;
            textureVertices[i * dimension * bufferVertexTexturesSize * textureVerticesPerQuad + bufferVertexTexturesSize * textureVerticesPerQuad * j + 60] = 0.0f;
            textureVertices[i * dimension * bufferVertexTexturesSize * textureVerticesPerQuad + bufferVertexTexturesSize * textureVerticesPerQuad * j + 61] = 0.0f;
            textureVertices[i * dimension * bufferVertexTexturesSize * textureVerticesPerQuad + bufferVertexTexturesSize * textureVerticesPerQuad * j + 62] = wallTextureWidth*(walls[wallMap[dimension - i - 1][j]]["upper"].GetInt()/(wallTextureWidth/quadWidth))/wallTextureWidth;
            textureVertices[i * dimension * bufferVertexTexturesSize * textureVerticesPerQuad + bufferVertexTexturesSize * textureVerticesPerQuad * j + 63] = 1.0f;

            textureIndices[i * dimension * textureIndicesPerQuad + textureIndicesPerQuad * j + 6] = i * dimension * textureVerticesPerQuad + textureVerticesPerQuad * j + 4;
            textureIndices[i * dimension * textureIndicesPerQuad + textureIndicesPerQuad * j + 7] = i * dimension * textureVerticesPerQuad + textureVerticesPerQuad * j + 5;
            textureIndices[i * dimension * textureIndicesPerQuad + textureIndicesPerQuad * j + 8] = i * dimension * textureVerticesPerQuad + textureVerticesPerQuad * j + 7;
            textureIndices[i * dimension * textureIndicesPerQuad + textureIndicesPerQuad * j + 9] = i * dimension * textureVerticesPerQuad + textureVerticesPerQuad * j + 5;
            textureIndices[i * dimension * textureIndicesPerQuad + textureIndicesPerQuad * j + 10] = i * dimension * textureVerticesPerQuad + textureVerticesPerQuad * j + 6;
            textureIndices[i * dimension * textureIndicesPerQuad + textureIndicesPerQuad * j + 11] = i * dimension * textureVerticesPerQuad + textureVerticesPerQuad * j + 7;
        }
    }

    return 1;
}

int Engine::initCamera() {
    projection = glm::ortho(-272.0f, 800.0f, -72.0f, 600.0f, -100.0f, 100.0f);

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

int Engine::initTextures() {

    /*

    FILE *proc = popen("/bin/ls resources/wall","r");
    char buf[1024];

    std::stringstream ss;

    std::list<std::string> wallFiles = {};

    while ( !feof(proc) && fgets(buf,sizeof(buf),proc) )
    {
        std::string fileName = std::string(buf);
        fileName.erase(std::remove(fileName.begin(), fileName.end(), '\n'), fileName.end());
        wallFiles.push_back(fileName);
        numOfWallTextureFiles++;
    }

    //GLuint wallTextures[numOfWallTextureFiles];
    Engine::wallTextures = new unsigned int[numOfWallTextureFiles];

    for (std::string wallFile : wallFiles) {
        ss.str(std::string());

        ss << "resources/wall/" << wallFile;
        std::string number;
        std::istringstream file(wallFile);

        getline(file, number, '_');


        // load and create a texture
        // -------------------------
        glGenTextures(1, &(wallTextures[stoi(number)]));
        glBindTexture(GL_TEXTURE_2D, wallTextures[stoi(number)]); // all upcoming GL_TEXTURE_2D operations now have effect on this texture object
        // set the texture wrapping parameters
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);	// set texture wrapping to GL_REPEAT (default wrapping method)
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        // set texture filtering parameters
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        // load image, create texture and generate mipmaps
        int width, height, nrChannels;

        stbi_set_flip_vertically_on_load(true);
        unsigned char* data = stbi_load(ss.str().c_str(), &width, &height, &nrChannels, 0);

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

    }
    */

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
    glClearColor(0.0f, 0.0f, 1.0f, 1.0f);
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

    glBufferData(GL_ARRAY_BUFFER, vertices_size, vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);

    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices_size, indices, GL_STATIC_DRAW);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, bufferVertexSize * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // gradient attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, bufferVertexSize * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // Walls

    int walls_vertices_size = sizeof(float) * dimension * dimension * textureVerticesPerQuad * bufferVertexTexturesSize;
    int walls_indices_size = sizeof(unsigned int) * dimension * dimension * textureIndicesPerQuad;

    glBindVertexArray(textureVAO);

    glBindBuffer(GL_ARRAY_BUFFER, textureVBO);

    glBufferData(GL_ARRAY_BUFFER, walls_vertices_size, textureVertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, textureEBO);

    glBufferData(GL_ELEMENT_ARRAY_BUFFER, walls_indices_size, textureIndices, GL_STATIC_DRAW);


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

    colorShader.use();
    colorShader.setFloatMatrix("projection", glm::value_ptr(projection));
    colorShader.setFloatMatrix("view", glm::value_ptr(view));
    colorShader.setFloatMatrix("model", glm::value_ptr(model));

    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, indicesPerQuad * dimension * dimension, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);

    wallShader.use();

    wallShader.setFloatMatrix("projection", glm::value_ptr(projection));
    wallShader.setFloatMatrix("view", glm::value_ptr(view));
    wallShader.setFloatMatrix("model", glm::value_ptr(model));

    glBindVertexArray(textureVAO);
    glBindTexture(GL_TEXTURE_2D, texture);
    glDrawElements(GL_TRIANGLES, 12, GL_UNSIGNED_INT, 0);

/*
    glBindVertexArray(textureVAO);

    for (int i = 0; i < dimension; i++) {
        for (int j = 0; j < dimension; j++) {
            if (wallMap[dimension - i - 1][j] != -1) {
                glBindTexture(GL_TEXTURE_2D, wallTextures[walls[wallMap[dimension - i - 1][j]]["lower"].GetInt()]);
                glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void*)((textureIndicesPerQuad * i * dimension + textureIndicesPerQuad * j) * sizeof(GLuint)));
                glBindTexture(GL_TEXTURE_2D, wallTextures[walls[wallMap[dimension - i - 1][j]]["upper"].GetInt()]);
                glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void*)((textureIndicesPerQuad * i * dimension + textureIndicesPerQuad * j + (textureIndicesPerQuad / 2)) * sizeof(GLuint)));
            }
        }
    }
*/
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
    int x = 0;
    int y = 0;

    if (input.getKeyState(INPUT_UP)) {
        y += 8.0f;
    }
    if (input.getKeyState(INPUT_RIGHT)) {
        x += 8.0f;
    }
    if (input.getKeyState(INPUT_DOWN)) {
        y -= 8.0f;
    }
    if (input.getKeyState(INPUT_LEFT)) {
        x -= 8.0f;
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

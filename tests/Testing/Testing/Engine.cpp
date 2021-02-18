#include "Engine.hpp"
//#define STB_IMAGE_IMPLEMENTATION
//#include "utils/stb_image.h"

void processKeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);

// Engine constructor
Engine::Engine(
    double framesPerSecond,
    int** map,
    int** wallMap,
    int dimension,
    float xOrigin,
    float yOrigin)
{
    Engine::framesPerSecond = framesPerSecond;
    Engine::map = map;
    Engine::wallMap = wallMap;
    Engine::dimension = dimension;

    Engine::xOrigin = xOrigin;
    Engine::yOrigin = yOrigin;

    Engine::input = Input();

    isRunning = false;

    frameDelay = 1/framesPerSecond;
    counter = 0;

    up = right = down = left = 0;

    Engine::platformVertices = new float[dimension * dimension * PLATFORM_BUFFER_VERTEX_SIZE * PLATFORM_VERTICES_PER_QUAD];

    Engine::platformIndices = new unsigned int[dimension * dimension * PLATFORM_INDICES_PER_QUAD];

    Engine::VAO = Engine::VBO = Engine::EBO = 0;

    Engine::wallVertices = new float[dimension * dimension * WALL_BUFFER_VERTEX_SIZE * WALL_VERTICES_PER_QUAD];

    Engine::wallIndices = new unsigned int[dimension * dimension * WALL_INDICES_PER_QUAD];

    Engine::textureVAO = Engine::textureVBO = Engine::textureEBO;

    //Engine::wallShadowVertices = ;

    //Engine::wallShadowIndices = ;
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

    window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Killer", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);


    //glViewport(0.0f, 0.0f, SCREEN_WIDTH, SCREEN_HEIGHT);

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

int Engine::initTextures(const rapidjson::Document& colors) {

    //Colors
    unsigned char* colorPlatformsData = createColorPlatforms(colors);

    TextureAsset* platformsTexture = new TextureAsset(QUAD_WIDTH, QUAD_HEIGHT*PLATFORM_TEXTURE_ROWS, QUAD_WIDTH,
            QUAD_HEIGHT, PLATFORM_TEXTURE_ROWS, PLATFORM_TEXTURE_COLS, NUMBER_OF_RGBA_CHANNELS,
            PLATFORM_BUFFER_VERTEX_SIZE, PLATFORM_VERTICES_PER_QUAD, PLATFORM_INDICES_PER_QUAD, colorPlatformsData);

    DrawingObject platforms = DrawingObject(dimension, platformsTexture, PLATFORMS_Z, &TextureUtils::initPlatformVerticesAndIndices,
                                SQUARE_VERTEX_SHADER_PATH, SQUARE_FRAGMENT_SHADER_PATH);

    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glGenTextures(1, &colorTexture);
    glBindTexture(GL_TEXTURE_2D, colorTexture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, QUAD_WIDTH, QUAD_HEIGHT*PLATFORM_TEXTURE_ROWS, 0, GL_RGBA, GL_UNSIGNED_BYTE, colorPlatformsData);
    glGenerateMipmap(GL_TEXTURE_2D);

    //Corregir 03 y 24 en muros. Añadir sombras, ver si en el negro compensa meterlo más realista...
    //Al meter personaje, igualar Y a Z. (profundidad --> Ver cómo solucionarlo)


    //Walls
    /*
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
    */

    return 1;
}

int Engine::initMaze(const rapidjson::Document& colors, const rapidjson::Document& walls) {

    for (int i = 0; i < dimension; i++) {
        for (int j = 0; j < dimension; j++) {
            int color = map[dimension - i - 1][j];
            // PLATFORMS //

            float top = ((float)color + 1.0f)/(float)PLATFORM_TEXTURE_ROWS;
            float bottom = (float)color/(float)PLATFORM_TEXTURE_ROWS;// - 1.0f/(float)QUAD_HEIGHT;

            platformVertices[i * dimension * PLATFORM_BUFFER_VERTEX_SIZE * PLATFORM_VERTICES_PER_QUAD + PLATFORM_BUFFER_VERTEX_SIZE * PLATFORM_VERTICES_PER_QUAD * j] = j * QUAD_WIDTH;
            platformVertices[i * dimension * PLATFORM_BUFFER_VERTEX_SIZE * PLATFORM_VERTICES_PER_QUAD + PLATFORM_BUFFER_VERTEX_SIZE * PLATFORM_VERTICES_PER_QUAD * j + 1] = i * QUAD_HEIGHT;
            platformVertices[i * dimension * PLATFORM_BUFFER_VERTEX_SIZE * PLATFORM_VERTICES_PER_QUAD + PLATFORM_BUFFER_VERTEX_SIZE * PLATFORM_VERTICES_PER_QUAD * j + 2] = 5.0f;
            platformVertices[i * dimension * PLATFORM_BUFFER_VERTEX_SIZE * PLATFORM_VERTICES_PER_QUAD + PLATFORM_BUFFER_VERTEX_SIZE * PLATFORM_VERTICES_PER_QUAD * j + 3] = 0.0f;
            platformVertices[i * dimension * PLATFORM_BUFFER_VERTEX_SIZE * PLATFORM_VERTICES_PER_QUAD + PLATFORM_BUFFER_VERTEX_SIZE * PLATFORM_VERTICES_PER_QUAD * j + 4] = bottom;//(QUAD_HEIGHT*PLATFORM_TEXTURE_ROWS*(colors[color_char]["codePrimary"]["r"].GetInt()/(QUAD_HEIGHT*PLATFORM_TEXTURE_ROWS/QUAD_WIDTH)) + QUAD_WIDTH)/wallTextureWidth - (0.5f/wallTextureWidth);

            platformVertices[i * dimension * PLATFORM_BUFFER_VERTEX_SIZE * PLATFORM_VERTICES_PER_QUAD + PLATFORM_BUFFER_VERTEX_SIZE * PLATFORM_VERTICES_PER_QUAD * j + 5] = j * QUAD_WIDTH;
            platformVertices[i * dimension * PLATFORM_BUFFER_VERTEX_SIZE * PLATFORM_VERTICES_PER_QUAD + PLATFORM_BUFFER_VERTEX_SIZE * PLATFORM_VERTICES_PER_QUAD * j + 6] = i * QUAD_HEIGHT + QUAD_HEIGHT;
            platformVertices[i * dimension * PLATFORM_BUFFER_VERTEX_SIZE * PLATFORM_VERTICES_PER_QUAD + PLATFORM_BUFFER_VERTEX_SIZE * PLATFORM_VERTICES_PER_QUAD * j + 7] = 5.0f;
            platformVertices[i * dimension * PLATFORM_BUFFER_VERTEX_SIZE * PLATFORM_VERTICES_PER_QUAD + PLATFORM_BUFFER_VERTEX_SIZE * PLATFORM_VERTICES_PER_QUAD * j + 8] = 0.0f;
            platformVertices[i * dimension * PLATFORM_BUFFER_VERTEX_SIZE * PLATFORM_VERTICES_PER_QUAD + PLATFORM_BUFFER_VERTEX_SIZE * PLATFORM_VERTICES_PER_QUAD * j + 9] = top;

            platformVertices[i * dimension * PLATFORM_BUFFER_VERTEX_SIZE * PLATFORM_VERTICES_PER_QUAD + PLATFORM_BUFFER_VERTEX_SIZE * PLATFORM_VERTICES_PER_QUAD * j + 10] = j * QUAD_WIDTH + QUAD_WIDTH;
            platformVertices[i * dimension * PLATFORM_BUFFER_VERTEX_SIZE * PLATFORM_VERTICES_PER_QUAD + PLATFORM_BUFFER_VERTEX_SIZE * PLATFORM_VERTICES_PER_QUAD * j + 11] = i * QUAD_HEIGHT;
            platformVertices[i * dimension * PLATFORM_BUFFER_VERTEX_SIZE * PLATFORM_VERTICES_PER_QUAD + PLATFORM_BUFFER_VERTEX_SIZE * PLATFORM_VERTICES_PER_QUAD * j + 12] = 5.0f;
            platformVertices[i * dimension * PLATFORM_BUFFER_VERTEX_SIZE * PLATFORM_VERTICES_PER_QUAD + PLATFORM_BUFFER_VERTEX_SIZE * PLATFORM_VERTICES_PER_QUAD * j + 13] = 1.0f;
            platformVertices[i * dimension * PLATFORM_BUFFER_VERTEX_SIZE * PLATFORM_VERTICES_PER_QUAD + PLATFORM_BUFFER_VERTEX_SIZE * PLATFORM_VERTICES_PER_QUAD * j + 14] = bottom;

            platformVertices[i * dimension * PLATFORM_BUFFER_VERTEX_SIZE * PLATFORM_VERTICES_PER_QUAD + PLATFORM_BUFFER_VERTEX_SIZE * PLATFORM_VERTICES_PER_QUAD * j + 15] = j * QUAD_WIDTH + QUAD_WIDTH;
            platformVertices[i * dimension * PLATFORM_BUFFER_VERTEX_SIZE * PLATFORM_VERTICES_PER_QUAD + PLATFORM_BUFFER_VERTEX_SIZE * PLATFORM_VERTICES_PER_QUAD * j + 16] = i * QUAD_HEIGHT + QUAD_HEIGHT;
            platformVertices[i * dimension * PLATFORM_BUFFER_VERTEX_SIZE * PLATFORM_VERTICES_PER_QUAD + PLATFORM_BUFFER_VERTEX_SIZE * PLATFORM_VERTICES_PER_QUAD * j + 17] = 5.0f;
            platformVertices[i * dimension * PLATFORM_BUFFER_VERTEX_SIZE * PLATFORM_VERTICES_PER_QUAD + PLATFORM_BUFFER_VERTEX_SIZE * PLATFORM_VERTICES_PER_QUAD * j + 18] = 1.0f;
            platformVertices[i * dimension * PLATFORM_BUFFER_VERTEX_SIZE * PLATFORM_VERTICES_PER_QUAD + PLATFORM_BUFFER_VERTEX_SIZE * PLATFORM_VERTICES_PER_QUAD * j + 19] = top;

            //Set indices here
            platformIndices[i * dimension * PLATFORM_INDICES_PER_QUAD + PLATFORM_INDICES_PER_QUAD * j] = i * dimension * PLATFORM_VERTICES_PER_QUAD + PLATFORM_VERTICES_PER_QUAD * j;
            platformIndices[i * dimension * PLATFORM_INDICES_PER_QUAD + PLATFORM_INDICES_PER_QUAD * j + 1] = i * dimension * PLATFORM_VERTICES_PER_QUAD + PLATFORM_VERTICES_PER_QUAD * j + 1;
            platformIndices[i * dimension * PLATFORM_INDICES_PER_QUAD + PLATFORM_INDICES_PER_QUAD * j + 2] = i * dimension * PLATFORM_VERTICES_PER_QUAD + PLATFORM_VERTICES_PER_QUAD * j + 2;
            platformIndices[i * dimension * PLATFORM_INDICES_PER_QUAD + PLATFORM_INDICES_PER_QUAD * j + 3] = i * dimension * PLATFORM_VERTICES_PER_QUAD + PLATFORM_VERTICES_PER_QUAD * j + 1;
            platformIndices[i * dimension * PLATFORM_INDICES_PER_QUAD + PLATFORM_INDICES_PER_QUAD * j + 4] = i * dimension * PLATFORM_VERTICES_PER_QUAD + PLATFORM_VERTICES_PER_QUAD * j + 2;
            platformIndices[i * dimension * PLATFORM_INDICES_PER_QUAD + PLATFORM_INDICES_PER_QUAD * j + 5] = i * dimension * PLATFORM_VERTICES_PER_QUAD + PLATFORM_VERTICES_PER_QUAD * j + 3;


            // WALLS //
            // Lower sprite

            if (wallMap[dimension - i - 1][j] != -1) {
                wallVertices[i * dimension * WALL_BUFFER_VERTEX_SIZE * WALL_VERTICES_PER_QUAD + WALL_BUFFER_VERTEX_SIZE * WALL_VERTICES_PER_QUAD * j] = j * QUAD_WIDTH + QUAD_WIDTH;
                wallVertices[i * dimension * WALL_BUFFER_VERTEX_SIZE * WALL_VERTICES_PER_QUAD + WALL_BUFFER_VERTEX_SIZE * WALL_VERTICES_PER_QUAD * j + 1] = i * QUAD_HEIGHT + (QUAD_HEIGHT / 2);
                wallVertices[i * dimension * WALL_BUFFER_VERTEX_SIZE * WALL_VERTICES_PER_QUAD + WALL_BUFFER_VERTEX_SIZE * WALL_VERTICES_PER_QUAD * j + 2] = 10.0f;
                wallVertices[i * dimension * WALL_BUFFER_VERTEX_SIZE * WALL_VERTICES_PER_QUAD + WALL_BUFFER_VERTEX_SIZE * WALL_VERTICES_PER_QUAD * j + 3] = 0.0f;
                wallVertices[i * dimension * WALL_BUFFER_VERTEX_SIZE * WALL_VERTICES_PER_QUAD + WALL_BUFFER_VERTEX_SIZE * WALL_VERTICES_PER_QUAD * j + 4] = 0.0f;
                wallVertices[i * dimension * WALL_BUFFER_VERTEX_SIZE * WALL_VERTICES_PER_QUAD + WALL_BUFFER_VERTEX_SIZE * WALL_VERTICES_PER_QUAD * j + 5] = 0.0f;
                wallVertices[i * dimension * WALL_BUFFER_VERTEX_SIZE * WALL_VERTICES_PER_QUAD + WALL_BUFFER_VERTEX_SIZE * WALL_VERTICES_PER_QUAD * j + 6] = (wallTextureWidth*(walls[wallMap[dimension - i - 1][j]]["lower"].GetInt()/(wallTextureWidth/QUAD_WIDTH)) + QUAD_WIDTH)/wallTextureWidth - (0.5f/wallTextureWidth);
                wallVertices[i * dimension * WALL_BUFFER_VERTEX_SIZE * WALL_VERTICES_PER_QUAD + WALL_BUFFER_VERTEX_SIZE * WALL_VERTICES_PER_QUAD * j + 7] = 1.0f;

                wallVertices[i * dimension * WALL_BUFFER_VERTEX_SIZE * WALL_VERTICES_PER_QUAD + WALL_BUFFER_VERTEX_SIZE * WALL_VERTICES_PER_QUAD * j + 8] = j * QUAD_WIDTH + QUAD_WIDTH;
                wallVertices[i * dimension * WALL_BUFFER_VERTEX_SIZE * WALL_VERTICES_PER_QUAD + WALL_BUFFER_VERTEX_SIZE * WALL_VERTICES_PER_QUAD * j + 9] = i * QUAD_HEIGHT;
                wallVertices[i * dimension * WALL_BUFFER_VERTEX_SIZE * WALL_VERTICES_PER_QUAD + WALL_BUFFER_VERTEX_SIZE * WALL_VERTICES_PER_QUAD * j + 10] = 10.0f;
                wallVertices[i * dimension * WALL_BUFFER_VERTEX_SIZE * WALL_VERTICES_PER_QUAD + WALL_BUFFER_VERTEX_SIZE * WALL_VERTICES_PER_QUAD * j + 11] = 0.0f;
                wallVertices[i * dimension * WALL_BUFFER_VERTEX_SIZE * WALL_VERTICES_PER_QUAD + WALL_BUFFER_VERTEX_SIZE * WALL_VERTICES_PER_QUAD * j + 12] = 0.0f;
                wallVertices[i * dimension * WALL_BUFFER_VERTEX_SIZE * WALL_VERTICES_PER_QUAD + WALL_BUFFER_VERTEX_SIZE * WALL_VERTICES_PER_QUAD * j + 13] = 0.0f;
                wallVertices[i * dimension * WALL_BUFFER_VERTEX_SIZE * WALL_VERTICES_PER_QUAD + WALL_BUFFER_VERTEX_SIZE * WALL_VERTICES_PER_QUAD * j + 14] = (wallTextureWidth*(walls[wallMap[dimension - i - 1][j]]["lower"].GetInt()/(wallTextureWidth/QUAD_WIDTH)) + QUAD_WIDTH)/wallTextureWidth - (0.5f/wallTextureWidth);
                wallVertices[i * dimension * WALL_BUFFER_VERTEX_SIZE * WALL_VERTICES_PER_QUAD + WALL_BUFFER_VERTEX_SIZE * WALL_VERTICES_PER_QUAD * j + 15] = 0.0f;

                wallVertices[i * dimension * WALL_BUFFER_VERTEX_SIZE * WALL_VERTICES_PER_QUAD + WALL_BUFFER_VERTEX_SIZE * WALL_VERTICES_PER_QUAD * j + 16] = j * QUAD_WIDTH;
                wallVertices[i * dimension * WALL_BUFFER_VERTEX_SIZE * WALL_VERTICES_PER_QUAD + WALL_BUFFER_VERTEX_SIZE * WALL_VERTICES_PER_QUAD * j + 17] = i * QUAD_HEIGHT;
                wallVertices[i * dimension * WALL_BUFFER_VERTEX_SIZE * WALL_VERTICES_PER_QUAD + WALL_BUFFER_VERTEX_SIZE * WALL_VERTICES_PER_QUAD * j + 18] = 10.0f;
                wallVertices[i * dimension * WALL_BUFFER_VERTEX_SIZE * WALL_VERTICES_PER_QUAD + WALL_BUFFER_VERTEX_SIZE * WALL_VERTICES_PER_QUAD * j + 19] = 0.0f;
                wallVertices[i * dimension * WALL_BUFFER_VERTEX_SIZE * WALL_VERTICES_PER_QUAD + WALL_BUFFER_VERTEX_SIZE * WALL_VERTICES_PER_QUAD * j + 20] = 0.0f;
                wallVertices[i * dimension * WALL_BUFFER_VERTEX_SIZE * WALL_VERTICES_PER_QUAD + WALL_BUFFER_VERTEX_SIZE * WALL_VERTICES_PER_QUAD * j + 21] = 0.0f;
                wallVertices[i * dimension * WALL_BUFFER_VERTEX_SIZE * WALL_VERTICES_PER_QUAD + WALL_BUFFER_VERTEX_SIZE * WALL_VERTICES_PER_QUAD * j + 22] = wallTextureWidth*(walls[wallMap[dimension - i - 1][j]]["lower"].GetInt()/(wallTextureWidth/QUAD_WIDTH))/wallTextureWidth + (0.5f/wallTextureWidth);
                wallVertices[i * dimension * WALL_BUFFER_VERTEX_SIZE * WALL_VERTICES_PER_QUAD + WALL_BUFFER_VERTEX_SIZE * WALL_VERTICES_PER_QUAD * j + 23] = 0.0f;

                wallVertices[i * dimension * WALL_BUFFER_VERTEX_SIZE * WALL_VERTICES_PER_QUAD + WALL_BUFFER_VERTEX_SIZE * WALL_VERTICES_PER_QUAD * j + 24] = j * QUAD_WIDTH;
                wallVertices[i * dimension * WALL_BUFFER_VERTEX_SIZE * WALL_VERTICES_PER_QUAD + WALL_BUFFER_VERTEX_SIZE * WALL_VERTICES_PER_QUAD * j + 25] = i * QUAD_HEIGHT + (QUAD_HEIGHT / 2);
                wallVertices[i * dimension * WALL_BUFFER_VERTEX_SIZE * WALL_VERTICES_PER_QUAD + WALL_BUFFER_VERTEX_SIZE * WALL_VERTICES_PER_QUAD * j + 26] = 10.0f;
                wallVertices[i * dimension * WALL_BUFFER_VERTEX_SIZE * WALL_VERTICES_PER_QUAD + WALL_BUFFER_VERTEX_SIZE * WALL_VERTICES_PER_QUAD * j + 27] = 0.0f;
                wallVertices[i * dimension * WALL_BUFFER_VERTEX_SIZE * WALL_VERTICES_PER_QUAD + WALL_BUFFER_VERTEX_SIZE * WALL_VERTICES_PER_QUAD * j + 28] = 0.0f;
                wallVertices[i * dimension * WALL_BUFFER_VERTEX_SIZE * WALL_VERTICES_PER_QUAD + WALL_BUFFER_VERTEX_SIZE * WALL_VERTICES_PER_QUAD * j + 29] = 0.0f;
                wallVertices[i * dimension * WALL_BUFFER_VERTEX_SIZE * WALL_VERTICES_PER_QUAD + WALL_BUFFER_VERTEX_SIZE * WALL_VERTICES_PER_QUAD * j + 30] = wallTextureWidth*(walls[wallMap[dimension - i - 1][j]]["lower"].GetInt()/(wallTextureWidth/QUAD_WIDTH))/wallTextureWidth + (0.5f/wallTextureWidth);
                wallVertices[i * dimension * WALL_BUFFER_VERTEX_SIZE * WALL_VERTICES_PER_QUAD + WALL_BUFFER_VERTEX_SIZE * WALL_VERTICES_PER_QUAD * j + 31] = 1.0f;

                wallIndices[i * dimension * WALL_INDICES_PER_QUAD + WALL_INDICES_PER_QUAD * j] = i * dimension * WALL_VERTICES_PER_QUAD + WALL_VERTICES_PER_QUAD * j;
                wallIndices[i * dimension * WALL_INDICES_PER_QUAD + WALL_INDICES_PER_QUAD * j + 1] = i * dimension * WALL_VERTICES_PER_QUAD + WALL_VERTICES_PER_QUAD * j + 1;
                wallIndices[i * dimension * WALL_INDICES_PER_QUAD + WALL_INDICES_PER_QUAD * j + 2] = i * dimension * WALL_VERTICES_PER_QUAD + WALL_VERTICES_PER_QUAD * j + 3;
                wallIndices[i * dimension * WALL_INDICES_PER_QUAD + WALL_INDICES_PER_QUAD * j + 3] = i * dimension * WALL_VERTICES_PER_QUAD + WALL_VERTICES_PER_QUAD * j + 1;
                wallIndices[i * dimension * WALL_INDICES_PER_QUAD + WALL_INDICES_PER_QUAD * j + 4] = i * dimension * WALL_VERTICES_PER_QUAD + WALL_VERTICES_PER_QUAD * j + 2;
                wallIndices[i * dimension * WALL_INDICES_PER_QUAD + WALL_INDICES_PER_QUAD * j + 5] = i * dimension * WALL_VERTICES_PER_QUAD + WALL_VERTICES_PER_QUAD * j + 3;

                // Upper sprite

                wallVertices[i * dimension * WALL_BUFFER_VERTEX_SIZE * WALL_VERTICES_PER_QUAD + WALL_BUFFER_VERTEX_SIZE * WALL_VERTICES_PER_QUAD * j + 32] = j * QUAD_WIDTH + QUAD_WIDTH;
                wallVertices[i * dimension * WALL_BUFFER_VERTEX_SIZE * WALL_VERTICES_PER_QUAD + WALL_BUFFER_VERTEX_SIZE * WALL_VERTICES_PER_QUAD * j + 33] = i * QUAD_HEIGHT + (QUAD_HEIGHT / 2) + (QUAD_HEIGHT / 2);
                wallVertices[i * dimension * WALL_BUFFER_VERTEX_SIZE * WALL_VERTICES_PER_QUAD + WALL_BUFFER_VERTEX_SIZE * WALL_VERTICES_PER_QUAD * j + 34] = 10.0f;
                wallVertices[i * dimension * WALL_BUFFER_VERTEX_SIZE * WALL_VERTICES_PER_QUAD + WALL_BUFFER_VERTEX_SIZE * WALL_VERTICES_PER_QUAD * j + 35] = 0.0f;
                wallVertices[i * dimension * WALL_BUFFER_VERTEX_SIZE * WALL_VERTICES_PER_QUAD + WALL_BUFFER_VERTEX_SIZE * WALL_VERTICES_PER_QUAD * j + 36] = 0.0f;
                wallVertices[i * dimension * WALL_BUFFER_VERTEX_SIZE * WALL_VERTICES_PER_QUAD + WALL_BUFFER_VERTEX_SIZE * WALL_VERTICES_PER_QUAD * j + 37] = 0.0f;
                wallVertices[i * dimension * WALL_BUFFER_VERTEX_SIZE * WALL_VERTICES_PER_QUAD + WALL_BUFFER_VERTEX_SIZE * WALL_VERTICES_PER_QUAD * j + 38] = (wallTextureWidth*(walls[wallMap[dimension - i - 1][j]]["upper"].GetInt()/(wallTextureWidth/QUAD_WIDTH)) + QUAD_WIDTH)/wallTextureWidth - (0.5f/wallTextureWidth);
                wallVertices[i * dimension * WALL_BUFFER_VERTEX_SIZE * WALL_VERTICES_PER_QUAD + WALL_BUFFER_VERTEX_SIZE * WALL_VERTICES_PER_QUAD * j + 39] = 1.0f;

                wallVertices[i * dimension * WALL_BUFFER_VERTEX_SIZE * WALL_VERTICES_PER_QUAD + WALL_BUFFER_VERTEX_SIZE * WALL_VERTICES_PER_QUAD * j + 40] = j * QUAD_WIDTH + QUAD_WIDTH;
                wallVertices[i * dimension * WALL_BUFFER_VERTEX_SIZE * WALL_VERTICES_PER_QUAD + WALL_BUFFER_VERTEX_SIZE * WALL_VERTICES_PER_QUAD * j + 41] = i * QUAD_HEIGHT + (QUAD_HEIGHT / 2);
                wallVertices[i * dimension * WALL_BUFFER_VERTEX_SIZE * WALL_VERTICES_PER_QUAD + WALL_BUFFER_VERTEX_SIZE * WALL_VERTICES_PER_QUAD * j + 42] = 10.0f;
                wallVertices[i * dimension * WALL_BUFFER_VERTEX_SIZE * WALL_VERTICES_PER_QUAD + WALL_BUFFER_VERTEX_SIZE * WALL_VERTICES_PER_QUAD * j + 43] = 0.0f;
                wallVertices[i * dimension * WALL_BUFFER_VERTEX_SIZE * WALL_VERTICES_PER_QUAD + WALL_BUFFER_VERTEX_SIZE * WALL_VERTICES_PER_QUAD * j + 44] = 0.0f;
                wallVertices[i * dimension * WALL_BUFFER_VERTEX_SIZE * WALL_VERTICES_PER_QUAD + WALL_BUFFER_VERTEX_SIZE * WALL_VERTICES_PER_QUAD * j + 45] = 0.0f;
                wallVertices[i * dimension * WALL_BUFFER_VERTEX_SIZE * WALL_VERTICES_PER_QUAD + WALL_BUFFER_VERTEX_SIZE * WALL_VERTICES_PER_QUAD * j + 46] = (wallTextureWidth*(walls[wallMap[dimension - i - 1][j]]["upper"].GetInt()/(wallTextureWidth/QUAD_WIDTH)) + QUAD_WIDTH)/wallTextureWidth - (0.5f/wallTextureWidth);
                wallVertices[i * dimension * WALL_BUFFER_VERTEX_SIZE * WALL_VERTICES_PER_QUAD + WALL_BUFFER_VERTEX_SIZE * WALL_VERTICES_PER_QUAD * j + 47] = 0.0f;

                wallVertices[i * dimension * WALL_BUFFER_VERTEX_SIZE * WALL_VERTICES_PER_QUAD + WALL_BUFFER_VERTEX_SIZE * WALL_VERTICES_PER_QUAD * j + 48] = j * QUAD_WIDTH;
                wallVertices[i * dimension * WALL_BUFFER_VERTEX_SIZE * WALL_VERTICES_PER_QUAD + WALL_BUFFER_VERTEX_SIZE * WALL_VERTICES_PER_QUAD * j + 49] = i * QUAD_HEIGHT + (QUAD_HEIGHT / 2);
                wallVertices[i * dimension * WALL_BUFFER_VERTEX_SIZE * WALL_VERTICES_PER_QUAD + WALL_BUFFER_VERTEX_SIZE * WALL_VERTICES_PER_QUAD * j + 50] = 10.0f;
                wallVertices[i * dimension * WALL_BUFFER_VERTEX_SIZE * WALL_VERTICES_PER_QUAD + WALL_BUFFER_VERTEX_SIZE * WALL_VERTICES_PER_QUAD * j + 51] = 0.0f;
                wallVertices[i * dimension * WALL_BUFFER_VERTEX_SIZE * WALL_VERTICES_PER_QUAD + WALL_BUFFER_VERTEX_SIZE * WALL_VERTICES_PER_QUAD * j + 52] = 0.0f;
                wallVertices[i * dimension * WALL_BUFFER_VERTEX_SIZE * WALL_VERTICES_PER_QUAD + WALL_BUFFER_VERTEX_SIZE * WALL_VERTICES_PER_QUAD * j + 53] = 0.0f;
                wallVertices[i * dimension * WALL_BUFFER_VERTEX_SIZE * WALL_VERTICES_PER_QUAD + WALL_BUFFER_VERTEX_SIZE * WALL_VERTICES_PER_QUAD * j + 54] = wallTextureWidth*(walls[wallMap[dimension - i - 1][j]]["upper"].GetInt()/(wallTextureWidth/QUAD_WIDTH))/wallTextureWidth + (0.5f/wallTextureWidth);
                wallVertices[i * dimension * WALL_BUFFER_VERTEX_SIZE * WALL_VERTICES_PER_QUAD + WALL_BUFFER_VERTEX_SIZE * WALL_VERTICES_PER_QUAD * j + 55] = 0.0f;

                wallVertices[i * dimension * WALL_BUFFER_VERTEX_SIZE * WALL_VERTICES_PER_QUAD + WALL_BUFFER_VERTEX_SIZE * WALL_VERTICES_PER_QUAD * j + 56] = j * QUAD_WIDTH;
                wallVertices[i * dimension * WALL_BUFFER_VERTEX_SIZE * WALL_VERTICES_PER_QUAD + WALL_BUFFER_VERTEX_SIZE * WALL_VERTICES_PER_QUAD * j + 57] = i * QUAD_HEIGHT + (QUAD_HEIGHT / 2) + (QUAD_HEIGHT / 2);
                wallVertices[i * dimension * WALL_BUFFER_VERTEX_SIZE * WALL_VERTICES_PER_QUAD + WALL_BUFFER_VERTEX_SIZE * WALL_VERTICES_PER_QUAD * j + 58] = 10.0f;
                wallVertices[i * dimension * WALL_BUFFER_VERTEX_SIZE * WALL_VERTICES_PER_QUAD + WALL_BUFFER_VERTEX_SIZE * WALL_VERTICES_PER_QUAD * j + 59] = 0.0f;
                wallVertices[i * dimension * WALL_BUFFER_VERTEX_SIZE * WALL_VERTICES_PER_QUAD + WALL_BUFFER_VERTEX_SIZE * WALL_VERTICES_PER_QUAD * j + 60] = 0.0f;
                wallVertices[i * dimension * WALL_BUFFER_VERTEX_SIZE * WALL_VERTICES_PER_QUAD + WALL_BUFFER_VERTEX_SIZE * WALL_VERTICES_PER_QUAD * j + 61] = 0.0f;
                wallVertices[i * dimension * WALL_BUFFER_VERTEX_SIZE * WALL_VERTICES_PER_QUAD + WALL_BUFFER_VERTEX_SIZE * WALL_VERTICES_PER_QUAD * j + 62] = wallTextureWidth*(walls[wallMap[dimension - i - 1][j]]["upper"].GetInt()/(wallTextureWidth/QUAD_WIDTH))/wallTextureWidth + (0.5f/wallTextureWidth);
                wallVertices[i * dimension * WALL_BUFFER_VERTEX_SIZE * WALL_VERTICES_PER_QUAD + WALL_BUFFER_VERTEX_SIZE * WALL_VERTICES_PER_QUAD * j + 63] = 1.0f;

                wallIndices[i * dimension * WALL_INDICES_PER_QUAD + WALL_INDICES_PER_QUAD * j + 6] = i * dimension * WALL_VERTICES_PER_QUAD + WALL_VERTICES_PER_QUAD * j + 4;
                wallIndices[i * dimension * WALL_INDICES_PER_QUAD + WALL_INDICES_PER_QUAD * j + 7] = i * dimension * WALL_VERTICES_PER_QUAD + WALL_VERTICES_PER_QUAD * j + 5;
                wallIndices[i * dimension * WALL_INDICES_PER_QUAD + WALL_INDICES_PER_QUAD * j + 8] = i * dimension * WALL_VERTICES_PER_QUAD + WALL_VERTICES_PER_QUAD * j + 7;
                wallIndices[i * dimension * WALL_INDICES_PER_QUAD + WALL_INDICES_PER_QUAD * j + 9] = i * dimension * WALL_VERTICES_PER_QUAD + WALL_VERTICES_PER_QUAD * j + 5;
                wallIndices[i * dimension * WALL_INDICES_PER_QUAD + WALL_INDICES_PER_QUAD * j + 10] = i * dimension * WALL_VERTICES_PER_QUAD + WALL_VERTICES_PER_QUAD * j + 6;
                wallIndices[i * dimension * WALL_INDICES_PER_QUAD + WALL_INDICES_PER_QUAD * j + 11] = i * dimension * WALL_VERTICES_PER_QUAD + WALL_VERTICES_PER_QUAD * j + 7;
            }


        }
    }

    return 1;
}

int Engine::initCamera() {
    //projection = glm::ortho(-272.0f, 800.0f, -72.0f, 600.0f, -100.0f, 100.0f);
    //projection = glm::ortho(-100.0f, 400.0f, -50.0f, 300.0f, -100.0f, 100.0f);
    Camera camera = Camera(xOrigin, VIEWPORT_WIDTH, yOrigin, VIEWPORT_HEIGHT, Z_NEAR, Z_FAR);
    projection = glm::ortho(xOrigin, VIEWPORT_WIDTH, yOrigin, VIEWPORT_HEIGHT, Z_NEAR, Z_FAR);
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

    //Shadow

    //Fill with VertexBuffer

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

    int vertices_size = sizeof(float) * dimension * dimension * PLATFORM_VERTICES_PER_QUAD * PLATFORM_BUFFER_VERTEX_SIZE;
    int indices_size = sizeof(unsigned int) * dimension * dimension * PLATFORM_INDICES_PER_QUAD;

    glBufferData(GL_ARRAY_BUFFER, vertices_size, platformVertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);

    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices_size, platformIndices, GL_STATIC_DRAW);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, PLATFORM_BUFFER_VERTEX_SIZE * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // gradient attribute
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, PLATFORM_BUFFER_VERTEX_SIZE * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // Walls

    int walls_vertices_size = sizeof(float) * dimension * dimension * WALL_VERTICES_PER_QUAD * WALL_BUFFER_VERTEX_SIZE;
    int walls_indices_size = sizeof(unsigned int) * dimension * dimension * WALL_INDICES_PER_QUAD;

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

    glBindVertexArray(VAO);
    glBindTexture(GL_TEXTURE_2D, colorTexture);
    glDrawElements(GL_TRIANGLES, PLATFORM_INDICES_PER_QUAD * dimension * dimension, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);

    wallShader.use();

    wallShader.setFloatMatrix("projection", glm::value_ptr(projection));
    wallShader.setFloatMatrix("view", glm::value_ptr(view));
    wallShader.setFloatMatrix("model", glm::value_ptr(model));

    glBindVertexArray(textureVAO);
    glBindTexture(GL_TEXTURE_2D, texture);
    glDrawElements(GL_TRIANGLES, WALL_INDICES_PER_QUAD * dimension * dimension, GL_UNSIGNED_INT, 0);
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

unsigned char* Engine::createColorPlatforms(const rapidjson::Document& colors) {
    unsigned char* pixels = new unsigned char[PLATFORM_TEXTURE_ROWS * QUAD_WIDTH * QUAD_HEIGHT * NUMBER_OF_RGBA_CHANNELS];

    for (int n = 0; n < PLATFORM_TEXTURE_ROWS; n++) {
        for (int i = 0; i < QUAD_HEIGHT; i++) {
            for (int j = 0; j < QUAD_WIDTH; j++) {
                if (i < 2 || j < 2 || i > 61 || j > 61) {
                    pixels[n * QUAD_HEIGHT * QUAD_WIDTH * NUMBER_OF_RGBA_CHANNELS + i * QUAD_WIDTH * NUMBER_OF_RGBA_CHANNELS + j * NUMBER_OF_RGBA_CHANNELS] = 157.0f;
                    pixels[n * QUAD_HEIGHT * QUAD_WIDTH * NUMBER_OF_RGBA_CHANNELS + i * QUAD_WIDTH * NUMBER_OF_RGBA_CHANNELS + j * NUMBER_OF_RGBA_CHANNELS + 1] = 157.0f;
                    pixels[n * QUAD_HEIGHT * QUAD_WIDTH * NUMBER_OF_RGBA_CHANNELS + i * QUAD_WIDTH * NUMBER_OF_RGBA_CHANNELS + j * NUMBER_OF_RGBA_CHANNELS + 2] = 157.0f;
                    pixels[n * QUAD_HEIGHT * QUAD_WIDTH * NUMBER_OF_RGBA_CHANNELS + i * QUAD_WIDTH * NUMBER_OF_RGBA_CHANNELS + j * NUMBER_OF_RGBA_CHANNELS + 3] = 1.0f;
                }
                else {
                    float randomNumber = (float) rand()/RAND_MAX;
                    float red, green, blue, diff;
                    if (randomNumber > 0.9) {
                        pixels[n * QUAD_HEIGHT * QUAD_WIDTH * NUMBER_OF_RGBA_CHANNELS + i * QUAD_WIDTH * NUMBER_OF_RGBA_CHANNELS + j * NUMBER_OF_RGBA_CHANNELS] = 0.0f;
                        pixels[n * QUAD_HEIGHT * QUAD_WIDTH * NUMBER_OF_RGBA_CHANNELS + i * QUAD_WIDTH * NUMBER_OF_RGBA_CHANNELS + j * NUMBER_OF_RGBA_CHANNELS + 1] = 0.0f;
                        pixels[n * QUAD_HEIGHT * QUAD_WIDTH * NUMBER_OF_RGBA_CHANNELS + i * QUAD_WIDTH * NUMBER_OF_RGBA_CHANNELS + j * NUMBER_OF_RGBA_CHANNELS + 2] = 0.0f;
                        pixels[n * QUAD_HEIGHT * QUAD_WIDTH * NUMBER_OF_RGBA_CHANNELS + i * QUAD_WIDTH * NUMBER_OF_RGBA_CHANNELS + j * NUMBER_OF_RGBA_CHANNELS + 3] = 1.0f;
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
                        pixels[n * QUAD_HEIGHT * QUAD_WIDTH * NUMBER_OF_RGBA_CHANNELS + i * QUAD_WIDTH * NUMBER_OF_RGBA_CHANNELS + j * NUMBER_OF_RGBA_CHANNELS] = red;
                        pixels[n * QUAD_HEIGHT * QUAD_WIDTH * NUMBER_OF_RGBA_CHANNELS + i * QUAD_WIDTH * NUMBER_OF_RGBA_CHANNELS + j * NUMBER_OF_RGBA_CHANNELS + 1] = green;
                        pixels[n * QUAD_HEIGHT * QUAD_WIDTH * NUMBER_OF_RGBA_CHANNELS + i * QUAD_WIDTH * NUMBER_OF_RGBA_CHANNELS + j * NUMBER_OF_RGBA_CHANNELS + 2] = blue;
                        pixels[n * QUAD_HEIGHT * QUAD_WIDTH * NUMBER_OF_RGBA_CHANNELS + i * QUAD_WIDTH * NUMBER_OF_RGBA_CHANNELS + j * NUMBER_OF_RGBA_CHANNELS + 3] = 1.0f;
                    }
                }
            }
        }
    }
    return pixels;
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

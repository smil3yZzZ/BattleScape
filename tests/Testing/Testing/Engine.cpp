#include "Engine.hpp"
#define STB_IMAGE_IMPLEMENTATION
#include "utils/stb_image.h"

const int Engine::squareWidth = 64;
const int Engine::platformHeight = 64;
const int Engine::wallHeight = 32;

void processKeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);

// Engine constructor
Engine::Engine(double framesPerSecond, int** map, int dimension, int bufferVertexSize, int bufferVertexTexturesSize,
    int verticesPerQuad, int indicesPerQuad, int quadWidth, int quadHeight, int quadOffset)
{
    Engine::framesPerSecond = framesPerSecond;
    Engine::map = map;
    Engine::dimension = dimension;

    Engine::bufferVertexSize = bufferVertexSize;
    Engine::bufferVertexTexturesSize = bufferVertexTexturesSize;
    Engine::verticesPerQuad = verticesPerQuad;
    Engine::indicesPerQuad = indicesPerQuad;

    Engine::quadWidth = quadWidth;
    Engine::quadHeight = quadHeight;
    Engine::quadOffset = quadOffset;

    Engine::input = Input();

    isRunning = false;

    frameDelay = 1/framesPerSecond;
    counter = 0;

    up = right = down = left = 0;

    Engine::vertices = new float[dimension * dimension * bufferVertexSize * verticesPerQuad];

    Engine::indices = new unsigned int[dimension * dimension * indicesPerQuad];

    Engine::VAO = Engine::VBO = Engine::EBO = 0;

    Engine::textureVertices = new float[dimension * dimension * bufferVertexTexturesSize * verticesPerQuad];

    Engine::textureIndices = new unsigned int[dimension * dimension * indicesPerQuad];

    Engine::textureVAO = Engine::textureVBO = Engine::textureEBO;
}

int Engine::run() {

    isRunning = true;

    FILE* fp = fopen("resources/colors.json", "r");

    char readBuffer[65536];
    rapidjson::FileReadStream is(fp, readBuffer, sizeof(readBuffer));

    rapidjson::Document colors;
    colors.ParseStream(is);

    fclose(fp);

    init(colors);

    double oldTime = glfwGetTime();

    double frameTime = frameDelay;

    while (!glfwWindowShouldClose(window)) {

        if (frameTime >= frameDelay) {
            oldTime = glfwGetTime();

            update(colors);

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



int Engine::init(const rapidjson::Document& colors) {
    initGL();
    initMaze(colors);
    initCamera();
    initShaders();
    generateBuffers();
    initTextures();
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

    //glEnable(GL_DEPTH_TEST);
}

int Engine::initMaze(const rapidjson::Document& colors) {
    for (int i = 0; i < dimension; i++) {
        for (int j = 0; j < dimension; j++) {
            int color = map[i][j];
            std::string color_string = std::to_string(color);
            const char* color_char = color_string.c_str();

            // PLATFORMS //

            vertices[i * dimension * bufferVertexSize * verticesPerQuad + bufferVertexSize * verticesPerQuad * j] = j * quadWidth + (1 + j) * quadOffset;
            vertices[i * dimension * bufferVertexSize * verticesPerQuad + bufferVertexSize * verticesPerQuad * j + 1] = i * quadHeight + (1 + i) * quadOffset;
            vertices[i * dimension * bufferVertexSize * verticesPerQuad + bufferVertexSize * verticesPerQuad * j + 2] = 0.0f;
            vertices[i * dimension * bufferVertexSize * verticesPerQuad + bufferVertexSize * verticesPerQuad * j + 3] = colors[color_char]["code"]["r"].GetInt();
            vertices[i * dimension * bufferVertexSize * verticesPerQuad + bufferVertexSize * verticesPerQuad * j + 4] = colors[color_char]["code"]["g"].GetInt();
            vertices[i * dimension * bufferVertexSize * verticesPerQuad + bufferVertexSize * verticesPerQuad * j + 5] = colors[color_char]["code"]["b"].GetInt();

            vertices[i * dimension * bufferVertexSize * verticesPerQuad + bufferVertexSize * verticesPerQuad * j + 6] = j * quadWidth + (1 + j) * quadOffset;
            vertices[i * dimension * bufferVertexSize * verticesPerQuad + bufferVertexSize * verticesPerQuad * j + 7] = i * quadHeight + (1 + i) * quadOffset + quadHeight;
            vertices[i * dimension * bufferVertexSize * verticesPerQuad + bufferVertexSize * verticesPerQuad * j + 8] = 0.0f;
            vertices[i * dimension * bufferVertexSize * verticesPerQuad + bufferVertexSize * verticesPerQuad * j + 9] = colors[color_char]["code"]["r"].GetInt();
            vertices[i * dimension * bufferVertexSize * verticesPerQuad + bufferVertexSize * verticesPerQuad * j + 10] = colors[color_char]["code"]["g"].GetInt();
            vertices[i * dimension * bufferVertexSize * verticesPerQuad + bufferVertexSize * verticesPerQuad * j + 11] = colors[color_char]["code"]["b"].GetInt();

            vertices[i * dimension * bufferVertexSize * verticesPerQuad + bufferVertexSize * verticesPerQuad * j + 12] = j * quadWidth + (1 + j) * quadOffset + quadWidth;
            vertices[i * dimension * bufferVertexSize * verticesPerQuad + bufferVertexSize * verticesPerQuad * j + 13] = i * quadHeight + (1 + i) * quadOffset;
            vertices[i * dimension * bufferVertexSize * verticesPerQuad + bufferVertexSize * verticesPerQuad * j + 14] = 0.0f;
            vertices[i * dimension * bufferVertexSize * verticesPerQuad + bufferVertexSize * verticesPerQuad * j + 15] = colors[color_char]["code"]["r"].GetInt();
            vertices[i * dimension * bufferVertexSize * verticesPerQuad + bufferVertexSize * verticesPerQuad * j + 16] = colors[color_char]["code"]["g"].GetInt();
            vertices[i * dimension * bufferVertexSize * verticesPerQuad + bufferVertexSize * verticesPerQuad * j + 17] = colors[color_char]["code"]["b"].GetInt();

            vertices[i * dimension * bufferVertexSize * verticesPerQuad + bufferVertexSize * verticesPerQuad * j + 18] = j * quadWidth + (1 + j) * quadOffset + quadWidth;
            vertices[i * dimension * bufferVertexSize * verticesPerQuad + bufferVertexSize * verticesPerQuad * j + 19] = i * quadHeight + (1 + i) * quadOffset + quadHeight;
            vertices[i * dimension * bufferVertexSize * verticesPerQuad + bufferVertexSize * verticesPerQuad * j + 20] = 0.0f;
            vertices[i * dimension * bufferVertexSize * verticesPerQuad + bufferVertexSize * verticesPerQuad * j + 21] = colors[color_char]["code"]["r"].GetInt();
            vertices[i * dimension * bufferVertexSize * verticesPerQuad + bufferVertexSize * verticesPerQuad * j + 22] = colors[color_char]["code"]["g"].GetInt();
            vertices[i * dimension * bufferVertexSize * verticesPerQuad + bufferVertexSize * verticesPerQuad * j + 23] = colors[color_char]["code"]["b"].GetInt();


            //Set indices here
            indices[i * dimension * indicesPerQuad + indicesPerQuad * j] = i * dimension * verticesPerQuad + verticesPerQuad * j;
            indices[i * dimension * indicesPerQuad + indicesPerQuad * j + 1] = i * dimension * verticesPerQuad + verticesPerQuad * j + 1;
            indices[i * dimension * indicesPerQuad + indicesPerQuad * j + 2] = i * dimension * verticesPerQuad + verticesPerQuad * j + 2;
            indices[i * dimension * indicesPerQuad + indicesPerQuad * j + 3] = i * dimension * verticesPerQuad + verticesPerQuad * j + 1;
            indices[i * dimension * indicesPerQuad + indicesPerQuad * j + 4] = i * dimension * verticesPerQuad + verticesPerQuad * j + 2;
            indices[i * dimension * indicesPerQuad + indicesPerQuad * j + 5] = i * dimension * verticesPerQuad + verticesPerQuad * j + 3;


            // WALLS //
            /*
            if (color > 7) {
                textureVertices[0] = 64.0f;
                textureVertices[1] = 32.0f;
                textureVertices[2] = 0.0f;
                textureVertices[3] = 0.0f;
                textureVertices[4] = 0.0f;
                textureVertices[5] = 0.0f;
                textureVertices[6] = 1.0f;
                textureVertices[7] = 1.0f;

                textureVertices[8] = 64.0f;
                textureVertices[9] = 0.0f;
                textureVertices[10] = 0.0f;
                textureVertices[11] = 0.0f;
                textureVertices[12] = 0.0f;
                textureVertices[13] = 0.0f;
                textureVertices[14] = 1.0f;
                textureVertices[15] = 0.0f;

                textureVertices[16] = 0.0f;
                textureVertices[17] = 0.0f;
                textureVertices[18] = 0.0f;
                textureVertices[19] = 0.0f;
                textureVertices[20] = 0.0f;
                textureVertices[21] = 0.0f;
                textureVertices[22] = 0.0f;
                textureVertices[23] = 0.0f;

                textureVertices[24] = 0.0f;
                textureVertices[25] = 32.0f;
                textureVertices[26] = 0.0f;
                textureVertices[27] = 0.0f;
                textureVertices[28] = 0.0f;
                textureVertices[29] = 0.0f;
                textureVertices[30] = 0.0f;
                textureVertices[31] = 1.0f;


            }

            textureIndices[i * dimension * indicesPerQuad + indicesPerQuad * j] = i * dimension * verticesPerQuad + verticesPerQuad * j;
            textureIndices[i * dimension * indicesPerQuad + indicesPerQuad * j + 1] = i * dimension * verticesPerQuad + verticesPerQuad * j + 1;
            textureIndices[i * dimension * indicesPerQuad + indicesPerQuad * j + 2] = i * dimension * verticesPerQuad + verticesPerQuad * j + 2;
            textureIndices[i * dimension * indicesPerQuad + indicesPerQuad * j + 3] = i * dimension * verticesPerQuad + verticesPerQuad * j + 1;
            textureIndices[i * dimension * indicesPerQuad + indicesPerQuad * j + 4] = i * dimension * verticesPerQuad + verticesPerQuad * j + 2;
            textureIndices[i * dimension * indicesPerQuad + indicesPerQuad * j + 5] = i * dimension * verticesPerQuad + verticesPerQuad * j + 3;
            */
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
    glGenVertexArrays(1, &VAO);

    glGenBuffers(1, &VBO);

    glGenBuffers(1, &EBO);
    return 1;
}

int Engine::initTextures() {


    Engine::textureVertices[0] = 64.0f;
    Engine::textureVertices[1] = 32.0f;
    Engine::textureVertices[2] = 0.0f;
    Engine::textureVertices[3] = 0.0f;
    Engine::textureVertices[4] = 0.0f;
    Engine::textureVertices[5] = 0.0f;
    Engine::textureVertices[6] = 1.0f;
    Engine::textureVertices[7] = 1.0f;

    Engine::textureVertices[8] = 64.0f;
    Engine::textureVertices[9] = 0.0f;
    Engine::textureVertices[10] = 0.0f;
    Engine::textureVertices[11] = 0.0f;
    Engine::textureVertices[12] = 0.0f;
    Engine::textureVertices[13] = 0.0f;
    Engine::textureVertices[14] = 1.0f;
    Engine::textureVertices[15] = 0.0f;

    Engine::textureVertices[16] = 0.0f;
    Engine::textureVertices[17] = 0.0f;
    Engine::textureVertices[18] = 0.0f;
    Engine::textureVertices[19] = 0.0f;
    Engine::textureVertices[20] = 0.0f;
    Engine::textureVertices[21] = 0.0f;
    Engine::textureVertices[22] = 0.0f;
    Engine::textureVertices[23] = 0.0f;

    Engine::textureVertices[24] = 0.0f;
    Engine::textureVertices[25] = 32.0f;
    Engine::textureVertices[26] = 0.0f;
    Engine::textureVertices[27] = 0.0f;
    Engine::textureVertices[28] = 0.0f;
    Engine::textureVertices[29] = 0.0f;
    Engine::textureVertices[30] = 0.0f;
    Engine::textureVertices[31] = 1.0f;

    Engine::textureIndices[0] = 0;
    Engine::textureIndices[1] = 1;
    Engine::textureIndices[2] = 3;
    Engine::textureIndices[3] = 1;
    Engine::textureIndices[4] = 2;
    Engine::textureIndices[5] = 3;

    glGenVertexArrays(1, &textureVAO);
    glGenBuffers(1, &textureVBO);
    glGenBuffers(1, &textureEBO);

    glBindVertexArray(textureVAO);

    glBindBuffer(GL_ARRAY_BUFFER, textureVBO);

    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 32, textureVertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, textureEBO);

    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * 6, textureIndices, GL_STATIC_DRAW);


    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // color attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    // texture coord attribute
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);


    // load and create a texture
    // -------------------------
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture); // all upcoming GL_TEXTURE_2D operations now have effect on this texture object
    // set the texture wrapping parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // set texture filtering parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // load image, create texture and generate mipmaps
    int width, height, nrChannels;
    // The FileSystem::getPath(...) is part of the GitHub repository so we can find files on any IDE/platform; replace it with your own image path.
    unsigned char* data = stbi_load("resources/wall/central.png", &width, &height, &nrChannels, 0);
    if (data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
        std::cout << width << "," << height;
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(data);

    /*
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    */

    // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
    // -------------------------------------------------------------------------------
    //glfwSwapBuffers(window);

    return 1;
}

int Engine::clearScreen() {
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    return 1;
}

int Engine::update(const rapidjson::Document& colors) {
    updateBuffers();
    return 1;
}

int Engine::updateBuffers() {

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

    /*Texture rendering*/

    // bind Texture

    // render container ---> PROBAR A METER EN SHADER las variables!
    //glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture);

    wallShader.use();

    wallShader.setFloatMatrix("projection", glm::value_ptr(projection));
    wallShader.setFloatMatrix("view", glm::value_ptr(view));
    wallShader.setFloatMatrix("model", glm::value_ptr(model));
    glBindVertexArray(textureVAO);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
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

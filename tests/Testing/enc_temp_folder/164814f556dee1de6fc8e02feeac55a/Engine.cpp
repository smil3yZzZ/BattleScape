#include "Engine.hpp"

const int Engine::squareWidth = 64;
const int Engine::platformHeight = 64;
const int Engine::wallHeight = 32;

// Engine constructor
Engine::Engine(double framesPerSecond, int** map, int dimension, int bufferVertexSize, int verticesPerQuad, int indicesPerQuad, int quadWidth, int quadHeight, int quadOffset)
{
    Engine::framesPerSecond = framesPerSecond;
    Engine::map = map;
    Engine::dimension = dimension;

    Engine::bufferVertexSize = bufferVertexSize;
    Engine::verticesPerQuad = verticesPerQuad;
    Engine::indicesPerQuad = indicesPerQuad;

    Engine::quadWidth = quadWidth;
    Engine::quadHeight = quadHeight;
    Engine::quadOffset = quadOffset;

    isRunning = false;

    frameDelay = 1/framesPerSecond;
    counter = 0;

    Engine::vertices = new float[dimension * dimension * bufferVertexSize * verticesPerQuad];

    Engine::indices = new unsigned int[dimension * dimension * indicesPerQuad];
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

    glewExperimental = GL_TRUE;

    glewInit();

    FILE* fp = fopen("resources/colors.json", "r");

    char readBuffer[65536];
    rapidjson::FileReadStream is(fp, readBuffer, sizeof(readBuffer));

    rapidjson::Document colors;
    colors.ParseStream(is);

    fclose(fp);

    init(colors);

    while (!glfwWindowShouldClose(window)) {

        //processInput(window, textureShader);

        double oldTime = glfwGetTime();

        update(colors);
        render();

        double frameTime = glfwGetTime() - oldTime;

        if (frameTime < frameDelay) {
            Sleep(1000*(frameDelay - frameTime));
            //std::cout << 1000 * (frameDelay - frameTime) << std::endl;
        }

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    return 1;
}

int Engine::init(const rapidjson::Document& colors) {
    initMaze(colors);
    initCamera();
    return 1;
}

int Engine::initMaze(const rapidjson::Document& colors) {
    for (int i = 0; i < dimension; i++) {
        for (int j = 0; j < dimension; j++) {
            int color = map[i][j];
            std::string color_string = std::to_string(color);
            const char* color_char = color_string.c_str();

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


        }
    }
    return 1;
}

int Engine::initCamera() {
    proj = glm::ortho(-544.0f, 1600.0f, -144.0f, 1200.0f, -100.0f, 100.0f);
    
    model = glm::mat4(1.0f);
    //std::cout << glm::to_string(proj) << std::endl;
    //Create identity matrices for model and view.
    //Change camera position
    glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 100.0f);
    glm::vec3 cameraTarget = glm::vec3(0.0f, 0.0f, 0.0f);
    glm::vec3 cameraDirection = glm::normalize(cameraPos - cameraTarget);
    glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
    glm::vec3 cameraRight = glm::normalize(glm::cross(up, cameraDirection));
    glm::vec3 cameraUp = glm::cross(cameraDirection, cameraRight);

    view = glm::lookAt(cameraPos, cameraTarget, cameraUp);
    return 1;
}

int Engine::update(const rapidjson::Document& colors) {
    updateBuffers();
    return 1;
}

int Engine::updateBuffers() {

    glGenVertexArrays(1, &VAO);

    glGenBuffers(1, &VBO);

    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    int vertices_size = sizeof(float) * dimension * dimension * bufferVertexSize * verticesPerQuad;
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

    Shader textureShader("shaders/triangleVertexShader.vert", "shaders/triangleFragmentShader.frag");

    textureShader.use();
    textureShader.setFloatMatrix("projection", glm::value_ptr(proj));
    //std::cout << "From top-left corner(" << 0 << "," << 600 << ") to bottom-right corner(" << 800 << "," << 0 << ")" << std::endl;
    textureShader.setFloatMatrix("view", glm::value_ptr(view));
    textureShader.setFloatMatrix("model", glm::value_ptr(model));

    return 1;
}

int Engine::render() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, indicesPerQuad * dimension * dimension, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
    return 1;
}

int Engine::getIsRunning() {
    return isRunning;
}

void Engine::setIsRunning(int isRunning) {
    Engine::isRunning = isRunning;
}
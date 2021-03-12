#include "Engine.hpp"

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

    Engine::input = new Input();

    isRunning = false;

    frameDelay = 1/framesPerSecond;

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

    rapidjson::Document wallsInfo;
    wallsInfo.ParseStream(isWalls);

    fclose(fWalls);

    init(colors, wallsInfo);

    double oldTime = glfwGetTime();

    double frameTime = frameDelay;

    std::cout << "Starting loop..." << std::endl;

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


    }

    delete platforms;
    delete walls;
    delete input;
    delete camera;

    glfwTerminate();

    return 1;
}

int Engine::init(const rapidjson::Document& colorsInfo, const rapidjson::Document& wallsInfo) {
    initGL();
    initTextures(colorsInfo, wallsInfo);
    initCamera();
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

int Engine::initTextures(const rapidjson::Document& colorsInfo, const rapidjson::Document& wallsInfo) {

    unsigned char* platformsData = createColorPlatforms(colorsInfo);

    TextureAsset* platformsTexture = new TextureAsset(QUAD_WIDTH, QUAD_HEIGHT*PLATFORM_TEXTURE_ROWS, QUAD_WIDTH,
            QUAD_HEIGHT, PLATFORM_TEXTURE_ROWS, PLATFORM_TEXTURE_COLS, NUMBER_OF_RGBA_CHANNELS,
            PLATFORM_BUFFER_VERTEX_SIZE, PLATFORM_VERTICES_PER_QUAD, PLATFORM_INDICES_PER_QUAD, platformsData);

    Engine::platforms = new PlatformsDrawingObject(dimension, platformsTexture, PLATFORMS_Z,
                                SQUARE_VERTEX_SHADER_PATH, SQUARE_FRAGMENT_SHADER_PATH);

    TextureAsset* wallsTexture = TextureUtils::loadTextureAsset(WALL_TEXTURE_PATH, WALL_TEXTURE_ROWS,
        WALL_TEXTURE_COLS, WALL_BUFFER_VERTEX_SIZE, WALL_VERTICES_PER_QUAD, WALL_INDICES_PER_QUAD);

    Engine::walls = new WallsDrawingObject(dimension, wallsTexture, WALLS_Z,
                    SQUARE_VERTEX_SHADER_PATH, SQUARE_FRAGMENT_SHADER_PATH);

    TextureUtils::initMap(dimension, map, wallMap, platforms, walls, wallsInfo);

    return 1;
}

int Engine::initCamera() {
    camera = new Camera(xOrigin, VIEWPORT_WIDTH, yOrigin, VIEWPORT_HEIGHT, Z_NEAR, Z_FAR);

    return 1;
}

int Engine::generateBuffers() {
    platforms->initBuffers();

    walls->initBuffers();

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

    platforms->updateBuffers(dimension);

    walls->updateBuffers(dimension);

    return 1;
}

int Engine::render() {

    platforms->render(dimension, camera->getProjection(), camera->getView(), camera->getModel());

    walls->render(dimension, camera->getProjection(), camera->getView(), camera->getModel());

    return 1;
}

int Engine::getIsRunning() {
    return isRunning;
}

void Engine::setIsRunning(int isRunning) {
    Engine::isRunning = isRunning;
}


void Engine::checkCamera() {
    float x = 0;
    float y = 0;

    if (input->getKeyState(INPUT_UP)) {
        y += 2.0f;
    }
    if (input->getKeyState(INPUT_RIGHT)) {
        x += 2.0f;
    }
    if (input->getKeyState(INPUT_DOWN)) {
        y -= 2.0f;
    }
    if (input->getKeyState(INPUT_LEFT)) {
        x -= 2.0f;
    }

    camera->setView(glm::vec3(x, y, 0.0f), glm::vec3(x, y, 0.0f));
}

void Engine::updateInput(int key, int action) {
    if (action == GLFW_PRESS){
        switch (key) {
            case GLFW_KEY_ESCAPE:
                //Will be changed to "pause"
                glfwSetWindowShouldClose(window, true);
            case GLFW_KEY_UP:
                input->setKeyState(INPUT_UP, 1);
                break;
            case GLFW_KEY_RIGHT:
                input->setKeyState(INPUT_RIGHT, 1);
                break;
            case GLFW_KEY_DOWN:
                input->setKeyState(INPUT_DOWN, 1);
                break;
            case GLFW_KEY_LEFT:
                input->setKeyState(INPUT_LEFT, 1);
                break;
        }
    }
    if (action == GLFW_RELEASE){
        switch (key) {
            case GLFW_KEY_UP:
                input->setKeyState(INPUT_UP, 0);
                break;
            case GLFW_KEY_RIGHT:
                input->setKeyState(INPUT_RIGHT, 0);
                break;
            case GLFW_KEY_DOWN:
                input->setKeyState(INPUT_DOWN, 0);
                break;
            case GLFW_KEY_LEFT:
                input->setKeyState(INPUT_LEFT, 0);
                break;
        }
    }
}

unsigned char* Engine::createColorPlatforms(const rapidjson::Document& colorsInfo) {
    unsigned char* pixels = new unsigned char[PLATFORM_TEXTURE_ROWS * QUAD_WIDTH * QUAD_HEIGHT * NUMBER_OF_RGBA_CHANNELS];


    for (int n = 0; n < PLATFORM_TEXTURE_ROWS; n++) {
        for (int i = 0; i < QUAD_HEIGHT; i+=2) {
            for (int j = 0; j < QUAD_WIDTH; j+=2) {
                if (i == 0 || j == 0 || i == 62 || j == 62) {
                    for (int y = i; y < i + 2; y++) {
                        for (int x = j; x < j + 2; x++) {
                            pixels[n * QUAD_HEIGHT * QUAD_WIDTH * NUMBER_OF_RGBA_CHANNELS + y * QUAD_WIDTH * NUMBER_OF_RGBA_CHANNELS + x * NUMBER_OF_RGBA_CHANNELS] = 157.0f;
                            pixels[n * QUAD_HEIGHT * QUAD_WIDTH * NUMBER_OF_RGBA_CHANNELS + y * QUAD_WIDTH * NUMBER_OF_RGBA_CHANNELS + x * NUMBER_OF_RGBA_CHANNELS + 1] = 157.0f;
                            pixels[n * QUAD_HEIGHT * QUAD_WIDTH * NUMBER_OF_RGBA_CHANNELS + y * QUAD_WIDTH * NUMBER_OF_RGBA_CHANNELS + x * NUMBER_OF_RGBA_CHANNELS + 2] = 157.0f;
                            pixels[n * QUAD_HEIGHT * QUAD_WIDTH * NUMBER_OF_RGBA_CHANNELS + y * QUAD_WIDTH * NUMBER_OF_RGBA_CHANNELS + x * NUMBER_OF_RGBA_CHANNELS + 3] = 1.0f;
                        }
                    }
                }
                else {
                    float randomNumber = (float) rand()/RAND_MAX;
                    float red, green, blue, diff;
                    if (randomNumber > 0.9) {
                        for (int y = i; y < i + 2; y++) {
                            for (int x = j; x < j + 2; x++) {
                                pixels[n * QUAD_HEIGHT * QUAD_WIDTH * NUMBER_OF_RGBA_CHANNELS + y * QUAD_WIDTH * NUMBER_OF_RGBA_CHANNELS + x * NUMBER_OF_RGBA_CHANNELS] = 0.0f;
                                pixels[n * QUAD_HEIGHT * QUAD_WIDTH * NUMBER_OF_RGBA_CHANNELS + y * QUAD_WIDTH * NUMBER_OF_RGBA_CHANNELS + x * NUMBER_OF_RGBA_CHANNELS + 1] = 0.0f;
                                pixels[n * QUAD_HEIGHT * QUAD_WIDTH * NUMBER_OF_RGBA_CHANNELS + y * QUAD_WIDTH * NUMBER_OF_RGBA_CHANNELS + x * NUMBER_OF_RGBA_CHANNELS + 2] = 0.0f;
                                pixels[n * QUAD_HEIGHT * QUAD_WIDTH * NUMBER_OF_RGBA_CHANNELS + y * QUAD_WIDTH * NUMBER_OF_RGBA_CHANNELS + x * NUMBER_OF_RGBA_CHANNELS + 3] = 1.0f;
                            }
                        }
                        /*
                        pixels[n * QUAD_HEIGHT * QUAD_WIDTH * NUMBER_OF_RGBA_CHANNELS + i * QUAD_WIDTH * NUMBER_OF_RGBA_CHANNELS + j * NUMBER_OF_RGBA_CHANNELS] = 0.0f;
                        pixels[n * QUAD_HEIGHT * QUAD_WIDTH * NUMBER_OF_RGBA_CHANNELS + i * QUAD_WIDTH * NUMBER_OF_RGBA_CHANNELS + j * NUMBER_OF_RGBA_CHANNELS + 1] = 0.0f;
                        pixels[n * QUAD_HEIGHT * QUAD_WIDTH * NUMBER_OF_RGBA_CHANNELS + i * QUAD_WIDTH * NUMBER_OF_RGBA_CHANNELS + j * NUMBER_OF_RGBA_CHANNELS + 2] = 0.0f;
                        pixels[n * QUAD_HEIGHT * QUAD_WIDTH * NUMBER_OF_RGBA_CHANNELS + i * QUAD_WIDTH * NUMBER_OF_RGBA_CHANNELS + j * NUMBER_OF_RGBA_CHANNELS + 3] = 1.0f;
                        */
                    }
                    else {
                        std::string color_string = std::to_string(n);
                        const char* color_char = color_string.c_str();
                        if (colorsInfo[color_char]["codePrimary"]["r"].GetInt() != colorsInfo[color_char]["codeSecondary"]["r"].GetInt()) {
                            diff = (float)abs(colorsInfo[color_char]["codePrimary"]["r"].GetInt() - colorsInfo[color_char]["codeSecondary"]["r"].GetInt()) * randomNumber;
                            red = colorsInfo[color_char]["codePrimary"]["r"].GetInt() < colorsInfo[color_char]["codeSecondary"]["r"].GetInt() ? colorsInfo[color_char]["codePrimary"]["r"].GetInt() + diff : colorsInfo[color_char]["codeSecondary"]["r"].GetInt() + diff;
                        }
                        else {
                            red = colorsInfo[color_char]["codePrimary"]["r"].GetInt();
                        }
                        if (colorsInfo[color_char]["codePrimary"]["g"].GetInt() != colorsInfo[color_char]["codeSecondary"]["g"].GetInt()) {
                            diff = (float)abs(colorsInfo[color_char]["codePrimary"]["g"].GetInt() - colorsInfo[color_char]["codeSecondary"]["g"].GetInt()) * randomNumber;
                            green = colorsInfo[color_char]["codePrimary"]["g"].GetInt() < colorsInfo[color_char]["codeSecondary"]["g"].GetInt() ? colorsInfo[color_char]["codePrimary"]["g"].GetInt() + diff : colorsInfo[color_char]["codeSecondary"]["g"].GetInt() + diff;
                        }
                        else {
                            green = colorsInfo[color_char]["codePrimary"]["g"].GetInt();
                        }
                        if (colorsInfo[color_char]["codePrimary"]["b"].GetInt() != colorsInfo[color_char]["codeSecondary"]["b"].GetInt()) {
                            diff = (float)abs(colorsInfo[color_char]["codePrimary"]["b"].GetInt() - colorsInfo[color_char]["codeSecondary"]["b"].GetInt()) * randomNumber;
                            blue = colorsInfo[color_char]["codePrimary"]["b"].GetInt() < colorsInfo[color_char]["codeSecondary"]["b"].GetInt() ? colorsInfo[color_char]["codePrimary"]["b"].GetInt() + diff : colorsInfo[color_char]["codeSecondary"]["b"].GetInt() + diff;
                        }
                        else {
                            blue = colorsInfo[color_char]["codePrimary"]["b"].GetInt();
                        }
                        for (int y = i; y < i + 2; y++) {
                            for (int x = j; x < j + 2; x++) {
                                pixels[n * QUAD_HEIGHT * QUAD_WIDTH * NUMBER_OF_RGBA_CHANNELS + y * QUAD_WIDTH * NUMBER_OF_RGBA_CHANNELS + x * NUMBER_OF_RGBA_CHANNELS] = red;
                                pixels[n * QUAD_HEIGHT * QUAD_WIDTH * NUMBER_OF_RGBA_CHANNELS + y * QUAD_WIDTH * NUMBER_OF_RGBA_CHANNELS + x * NUMBER_OF_RGBA_CHANNELS + 1] = green;
                                pixels[n * QUAD_HEIGHT * QUAD_WIDTH * NUMBER_OF_RGBA_CHANNELS + y * QUAD_WIDTH * NUMBER_OF_RGBA_CHANNELS + x * NUMBER_OF_RGBA_CHANNELS + 2] = blue;
                                pixels[n * QUAD_HEIGHT * QUAD_WIDTH * NUMBER_OF_RGBA_CHANNELS + y * QUAD_WIDTH * NUMBER_OF_RGBA_CHANNELS + x * NUMBER_OF_RGBA_CHANNELS + 3] = 1.0f;
                            }
                        }
                        /*
                        pixels[n * QUAD_HEIGHT * QUAD_WIDTH * NUMBER_OF_RGBA_CHANNELS + i * QUAD_WIDTH * NUMBER_OF_RGBA_CHANNELS + j * NUMBER_OF_RGBA_CHANNELS] = red;
                        pixels[n * QUAD_HEIGHT * QUAD_WIDTH * NUMBER_OF_RGBA_CHANNELS + i * QUAD_WIDTH * NUMBER_OF_RGBA_CHANNELS + j * NUMBER_OF_RGBA_CHANNELS + 1] = green;
                        pixels[n * QUAD_HEIGHT * QUAD_WIDTH * NUMBER_OF_RGBA_CHANNELS + i * QUAD_WIDTH * NUMBER_OF_RGBA_CHANNELS + j * NUMBER_OF_RGBA_CHANNELS + 2] = blue;
                        pixels[n * QUAD_HEIGHT * QUAD_WIDTH * NUMBER_OF_RGBA_CHANNELS + i * QUAD_WIDTH * NUMBER_OF_RGBA_CHANNELS + j * NUMBER_OF_RGBA_CHANNELS + 3] = 1.0f;
                        */
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
    engine->updateInput(key, action);
}

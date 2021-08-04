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

    double frameTime = frameDelay;
    //auto const frameTime = std::chrono::milliseconds{(int)(frameDelay * 1000.0f)};;

    double oldTime = glfwGetTime();
    //auto const oldTime = std::chrono::steady_clock::now();

    //auto nextFrameTime = oldTime + frameTime;

    float deltaTime = 0.0f;
    float lastFrame = glfwGetTime();

    std::cout << "Starting loop..." << std::endl;

    int frame = 0;

    while (!glfwWindowShouldClose(window)) {

        if (frameTime >= frameDelay) {
            oldTime = glfwGetTime();

            checkCamera(frame);

            update();

            clearScreen();

            render();

            glfwSwapBuffers(window);

            glfwPollEvents();

            frame = (frame % 60) + 1;
        }

        //std::this_thread::sleep_until(nextFrameTime);
        //nextFrameTime += frameTime;

        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        frameTime = glfwGetTime() - oldTime;

    }

    delete platforms;
    delete walls;
    delete input;
    delete camera;
    delete [] map;
    delete [] wallMap;

    glfwTerminate();

    return 1;
}

int Engine::init(const rapidjson::Document& colorsInfo, const rapidjson::Document& wallsInfo) {
    initGL();
    initTextures(colorsInfo, wallsInfo);
    initCharacters();
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
    glDepthFunc(GL_LESS);
    glEnable(GL_SMOOTH);
    glEnable(GL_BLEND);
    glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    return 1;
}

int Engine::initMapTextures(const rapidjson::Document& colorsInfo, const rapidjson::Document& wallsInfo) {

    unsigned char* platformsData = createColorPlatformsAndShadows(colorsInfo);

    TextureAsset* platformsTexture = new TextureAsset(ORIGINAL_QUAD_WIDTH*PLATFORM_TEXTURE_COLS, ORIGINAL_QUAD_HEIGHT*PLATFORM_TEXTURE_ROWS,
        QUAD_WIDTH, QUAD_HEIGHT, PLATFORM_TEXTURE_ROWS, PLATFORM_TEXTURE_COLS, NUMBER_OF_RGBA_CHANNELS,
            PLATFORM_BUFFER_VERTEX_SIZE, PLATFORM_VERTICES_PER_QUAD, PLATFORM_INDICES_PER_QUAD, platformsData);

    //free image data after glGenTexture
    //Añadir sombras a personaje
    //Añadir sombras a muros
    //Movimiento personaje
    //Colisiones
    //Niebla de guerra
    //Objetivos gameplay


    Engine::platforms = new PlatformsDrawingObject(dimension, platformsTexture, PLATFORMS_Z_OFFSET,
                                SQUARE_VERTEX_SHADER_PATH, SQUARE_FRAGMENT_SHADER_PATH);

    TextureAsset* wallsTexture = TextureUtils::loadTextureAsset(WALL_TEXTURE_PATH, WALL_TEXTURE_ROWS,
        WALL_TEXTURE_COLS, WALL_BUFFER_VERTEX_SIZE, WALL_VERTICES_PER_QUAD, WALL_INDICES_PER_QUAD,
        WALL_TEXTURE_LOAD_SUBSAMPLING_FACTOR);
    Engine::walls = new WallsDrawingObject(dimension, wallsTexture, WALLS_Z_OFFSET,
                    SQUARE_VERTEX_SHADER_PATH, SQUARE_FRAGMENT_SHADER_PATH);

    TextureUtils::initMap(dimension, map, wallMap, platforms, walls, wallsInfo);

    return 1;
}

int Engine::initCharacters() {
    TextureAsset* characterTexture = TextureUtils::loadTextureAsset(CHARACTER_TEXTURE_PATH,
        CHARACTER_TEXTURE_ROWS, CHARACTER_TEXTURE_COLS, CHARACTER_BUFFER_VERTEX_SIZE,
        CHARACTER_VERTICES_PER_QUAD, CHARACTER_INDICES_PER_QUAD, SPRITE_TEXTURE_LOAD_SUBSAMPLING_FACTOR);

    unsigned char* characterShadowData = createCharacterShadow();

    TextureAsset* characterShadowTexture = new TextureAsset(
        ORIGINAL_SHADOW_WIDTH, ORIGINAL_SHADOW_HEIGHT,
        ORIGINAL_SHADOW_WIDTH / SPRITE_TEXTURE_LOAD_SUBSAMPLING_FACTOR, ORIGINAL_SHADOW_HEIGHT / SPRITE_TEXTURE_LOAD_SUBSAMPLING_FACTOR, 1, 1, NUMBER_OF_RGBA_CHANNELS,
        SHADOW_BUFFER_VERTEX_SIZE, SHADOW_VERTICES_PER_QUAD, SHADOW_INDICES_PER_QUAD, characterShadowData);

    Engine::testCharacter = new Character(characterTexture, characterShadowTexture, CHARACTER_VERTEX_SHADER_PATH, CHARACTER_FRAGMENT_SHADER_PATH);

    mainPlayerLastDirection = UP;

    testCharacter->update(CHARACTER_INITIAL_X, CHARACTER_INITIAL_Y, 0, mainPlayerLastDirection);

    return 1;
}

int Engine::initCamera() {
    camera = new Camera(xOrigin, VIEWPORT_WIDTH, yOrigin, VIEWPORT_HEIGHT, Z_NEAR, Z_FAR);

    return 1;
}

int Engine::generateBuffers() {
    platforms->initBuffers();

    walls->initBuffers();

    testCharacter->getSprite()->initBuffers();

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

    platforms->updateBuffers();

    walls->updateBuffers();

    testCharacter->getSprite()->updateBuffers();

    return 1;
}

int Engine::render() {

    platforms->render(camera->getProjection(), camera->getView(), camera->getModel());

    walls->render(camera->getProjection(), camera->getView(), camera->getModel());

    testCharacter->getSprite()->render(camera->getProjection(), camera->getView(), camera->getModel());

    return 1;
}

int Engine::getIsRunning() {
    return isRunning;
}

void Engine::setIsRunning(int isRunning) {
    Engine::isRunning = isRunning;
}


void Engine::checkCamera(int frame) {

    float x = 0;
    float y = 0;

    if (input->getKeyState(INPUT_UP)) {
        mainPlayerLastDirection = UP;
        y += MOVEMENT_SPEED;
    }
    if (input->getKeyState(INPUT_DOWN)) {
        mainPlayerLastDirection = DOWN;
        y -= MOVEMENT_SPEED;
    }
    if (input->getKeyState(INPUT_RIGHT)) {
        mainPlayerLastDirection = RIGHT;
        x += MOVEMENT_SPEED;
    }
    if (input->getKeyState(INPUT_LEFT)) {
        mainPlayerLastDirection = LEFT;
        x -= MOVEMENT_SPEED;
    }

    if (testCharacter->collisionExists(x, y, map)) {
        testCharacter->update(0.0f, 0.0f, 0, mainPlayerLastDirection);
    }
    else {
        testCharacter->update(x, y, frame, mainPlayerLastDirection);
        camera->setView(glm::vec3(x, y, 0.0f), glm::vec3(x, y, 0.0f));
    }
    
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

unsigned char* Engine::createColorPlatformsAndShadows(const rapidjson::Document& colorsInfo) {
    unsigned char* pixels = new unsigned char[PLATFORM_TEXTURE_ROWS * ORIGINAL_QUAD_WIDTH * ORIGINAL_QUAD_HEIGHT * NUMBER_OF_RGBA_CHANNELS];

    //Llevar sombras y grietas a otro Drawing Object
    for (int n = 0; n < PLATFORM_TEXTURE_ROWS; n++) {
        for (int i = 0; i < ORIGINAL_QUAD_HEIGHT - 1; i+=2) {
            for (int j = 0; j < ORIGINAL_QUAD_WIDTH - 1; j+=2) {
                if (i < 4 || j < 4 || i > ORIGINAL_QUAD_WIDTH - 6 || j > ORIGINAL_QUAD_WIDTH - 6) {
                    for (int y = i; y < i + 2; y++) {
                        for (int x = j; x < j + 2; x++) {
                            pixels[n * ORIGINAL_QUAD_HEIGHT * ORIGINAL_QUAD_WIDTH * NUMBER_OF_RGBA_CHANNELS + y * ORIGINAL_QUAD_WIDTH * NUMBER_OF_RGBA_CHANNELS + x * NUMBER_OF_RGBA_CHANNELS] = 157.0f;
                            pixels[n * ORIGINAL_QUAD_HEIGHT * ORIGINAL_QUAD_WIDTH * NUMBER_OF_RGBA_CHANNELS + y * ORIGINAL_QUAD_WIDTH * NUMBER_OF_RGBA_CHANNELS + x * NUMBER_OF_RGBA_CHANNELS + 1] = 157.0f;
                            pixels[n * ORIGINAL_QUAD_HEIGHT * ORIGINAL_QUAD_WIDTH * NUMBER_OF_RGBA_CHANNELS + y * ORIGINAL_QUAD_WIDTH * NUMBER_OF_RGBA_CHANNELS + x * NUMBER_OF_RGBA_CHANNELS + 2] = 157.0f;
                            pixels[n * ORIGINAL_QUAD_HEIGHT * ORIGINAL_QUAD_WIDTH * NUMBER_OF_RGBA_CHANNELS + y * ORIGINAL_QUAD_WIDTH * NUMBER_OF_RGBA_CHANNELS + x * NUMBER_OF_RGBA_CHANNELS + 3] = 255.0f;
                        }
                    }
                }
                else {
                    /*
                    float randomNumber = (float) rand()/RAND_MAX;
                    float red, green, blue, diff;
                    if (randomNumber < ((float)(PLATFORM_TEXTURE_ROWS - n))/12.0f) {
                        for (int y = i; y < i + 2; y++) {
                            for (int x = j; x < j + 2; x++) {
                                pixels[n * ORIGINAL_QUAD_HEIGHT * ORIGINAL_QUAD_WIDTH * NUMBER_OF_RGBA_CHANNELS + y * ORIGINAL_QUAD_WIDTH * NUMBER_OF_RGBA_CHANNELS + x * NUMBER_OF_RGBA_CHANNELS] = 66.0f;
                                pixels[n * ORIGINAL_QUAD_HEIGHT * ORIGINAL_QUAD_WIDTH * NUMBER_OF_RGBA_CHANNELS + y * ORIGINAL_QUAD_WIDTH * NUMBER_OF_RGBA_CHANNELS + x * NUMBER_OF_RGBA_CHANNELS + 1] = 66.0f;
                                pixels[n * ORIGINAL_QUAD_HEIGHT * ORIGINAL_QUAD_WIDTH * NUMBER_OF_RGBA_CHANNELS + y * ORIGINAL_QUAD_WIDTH * NUMBER_OF_RGBA_CHANNELS + x * NUMBER_OF_RGBA_CHANNELS + 2] = 66.0f;
                                pixels[n * ORIGINAL_QUAD_HEIGHT * ORIGINAL_QUAD_WIDTH * NUMBER_OF_RGBA_CHANNELS + y * ORIGINAL_QUAD_WIDTH * NUMBER_OF_RGBA_CHANNELS + x * NUMBER_OF_RGBA_CHANNELS + 3] = 255.0f;
                            }
                        }
                    }
                    else {
                        std::string color_string = std::to_string(n);
                        const char* color_char = color_string.c_str();
                        for (int y = i; y < i + 2; y++) {
                            for (int x = j; x < j + 2; x++) {
                                pixels[n * ORIGINAL_QUAD_HEIGHT * ORIGINAL_QUAD_WIDTH * NUMBER_OF_RGBA_CHANNELS + y * ORIGINAL_QUAD_WIDTH * NUMBER_OF_RGBA_CHANNELS + x * NUMBER_OF_RGBA_CHANNELS] = colorsInfo[color_char]["codePrimary"]["r"].GetInt();
                                pixels[n * ORIGINAL_QUAD_HEIGHT * ORIGINAL_QUAD_WIDTH * NUMBER_OF_RGBA_CHANNELS + y * ORIGINAL_QUAD_WIDTH * NUMBER_OF_RGBA_CHANNELS + x * NUMBER_OF_RGBA_CHANNELS + 1] = colorsInfo[color_char]["codePrimary"]["g"].GetInt();
                                pixels[n * ORIGINAL_QUAD_HEIGHT * ORIGINAL_QUAD_WIDTH * NUMBER_OF_RGBA_CHANNELS + y * ORIGINAL_QUAD_WIDTH * NUMBER_OF_RGBA_CHANNELS + x * NUMBER_OF_RGBA_CHANNELS + 2] = colorsInfo[color_char]["codePrimary"]["b"].GetInt();
                                pixels[n * ORIGINAL_QUAD_HEIGHT * ORIGINAL_QUAD_WIDTH * NUMBER_OF_RGBA_CHANNELS + y * ORIGINAL_QUAD_WIDTH * NUMBER_OF_RGBA_CHANNELS + x * NUMBER_OF_RGBA_CHANNELS + 3] = 255.0f;
                            }
                        }
                    }
                    */
                    std::string color_string = std::to_string(n);
                    const char* color_char = color_string.c_str();
                    for (int y = i; y < i + 2; y++) {
                        for (int x = j; x < j + 2; x++) {
                            pixels[n * ORIGINAL_QUAD_HEIGHT * ORIGINAL_QUAD_WIDTH * NUMBER_OF_RGBA_CHANNELS + y * ORIGINAL_QUAD_WIDTH * NUMBER_OF_RGBA_CHANNELS + x * NUMBER_OF_RGBA_CHANNELS] = colorsInfo[color_char]["codePrimary"]["r"].GetInt();
                            pixels[n * ORIGINAL_QUAD_HEIGHT * ORIGINAL_QUAD_WIDTH * NUMBER_OF_RGBA_CHANNELS + y * ORIGINAL_QUAD_WIDTH * NUMBER_OF_RGBA_CHANNELS + x * NUMBER_OF_RGBA_CHANNELS + 1] = colorsInfo[color_char]["codePrimary"]["g"].GetInt();
                            pixels[n * ORIGINAL_QUAD_HEIGHT * ORIGINAL_QUAD_WIDTH * NUMBER_OF_RGBA_CHANNELS + y * ORIGINAL_QUAD_WIDTH * NUMBER_OF_RGBA_CHANNELS + x * NUMBER_OF_RGBA_CHANNELS + 2] = colorsInfo[color_char]["codePrimary"]["b"].GetInt();
                            pixels[n * ORIGINAL_QUAD_HEIGHT * ORIGINAL_QUAD_WIDTH * NUMBER_OF_RGBA_CHANNELS + y * ORIGINAL_QUAD_WIDTH * NUMBER_OF_RGBA_CHANNELS + x * NUMBER_OF_RGBA_CHANNELS + 3] = 255.0f;
                        }
                    }
                }
            }
        }
    }
    return pixels;
}

unsigned char* Engine::createColorPlatformsAndShadows(const rapidjson::Document& colorsInfo) {
    unsigned char* pixels = new unsigned char[ORIGINAL_SHADOW_HEIGHT * ORIGINAL_SHADOW_WIDTH * NUMBER_OF_RGBA_CHANNELS];
    return pixels;
}

void processKeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    Engine* engine = (Engine*)glfwGetWindowUserPointer(window);
    engine->updateInput(key, action);
}

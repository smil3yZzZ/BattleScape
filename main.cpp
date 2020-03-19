//#define STB_IMAGE_IMPLEMENTATION
//#include "include/stb_image.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/string_cast.hpp>


#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <string>
#include <iostream>
#include <cstdlib>

#include <math.h>

//#include "headers/Shader.h"

#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/filereadstream.h"
#include <iostream>
#include <time.h>

#include "Shader.h"

#define BUFFER_VERTEX_SIZE 6
#define INDICES_PER_QUAD 6
#define VERTICES_PER_QUAD 4
#define QUAD_WIDTH 64
#define QUAD_HEIGHT 64
#define QUAD_OFFSET QUAD_WIDTH

using namespace std;
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window, Shader textureShader);
void render(unsigned int VAO, unsigned int VBO, unsigned int vertices [], int vertices_size, Shader textureShader, int iMax, int jMax);

int main(){

    srand(time(NULL));

    if( !glfwInit() )
    {
        fprintf( stderr, "Failed to initialize GLFW\n" );
        return -1;
    }
    // OpenGL version 3.3
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

    // We will use the core profile. Specification:
    // https://www.khronos.org/registry/OpenGL/specs/gl/glspec46.core.pdf
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    // For OS X systems, forward-compatibility
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    // Create the window
    GLFWwindow* window = glfwCreateWindow(800, 600, "Hectorian", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }

    //Main context of the current thread
    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    std::cout << "Defining the triangle vertices.." << std::endl;
    int iMax = 11;
    int jMax = 11;

    float * vertices = new float [iMax*jMax*BUFFER_VERTEX_SIZE*VERTICES_PER_QUAD];


    //glm::vec2 gradients[((iMax + 1)*(jMax + 1))];
    //glm::vec2 gridPoints[iMax*jMax];

    unsigned int * indices = new unsigned int [iMax*jMax*INDICES_PER_QUAD];
    unsigned char* pixelOffset;

    /*
    for(int i = 0; i < iMax + 1; i++){
        for(int j = 0; j < jMax + 1; j++){
            //Set gradients here
            float gradient_x_value = (static_cast <float> (rand())) /( static_cast <float> (RAND_MAX));
            cout << gradient_x_value << endl;
            float gradient_y_value = (static_cast <float> (rand())) /( static_cast <float> (RAND_MAX));
            cout << gradient_y_value << endl;
            gradients[i*(jMax + 1) + j] = glm::vec2(gradient_x_value, gradient_y_value);
        }
    }
    */
    FILE* fp = fopen("resources/colors.json", "r");

    char readBuffer[65536];
    rapidjson::FileReadStream is(fp, readBuffer, sizeof(readBuffer));

    rapidjson::Document d;
    d.ParseStream(is);

    int highest = 6;
    int lowest = 0;
    int range = highest - lowest + 1;

    srand(static_cast<int>(time(0)));

    for(int i = 0; i < iMax; i++){
        for(int j = 0; j < jMax; j++){
            int color = rand() % 7;
            const char * color_char = std::to_string(color).c_str();
            cout << d[color_char]["color"].GetString() << endl;

            vertices[i*jMax*BUFFER_VERTEX_SIZE*VERTICES_PER_QUAD + BUFFER_VERTEX_SIZE*VERTICES_PER_QUAD*j] = j*QUAD_WIDTH + (1 + j)*QUAD_OFFSET;
            vertices[i*jMax*BUFFER_VERTEX_SIZE*VERTICES_PER_QUAD + BUFFER_VERTEX_SIZE*VERTICES_PER_QUAD*j + 1] = i*QUAD_HEIGHT + (1 + i)*QUAD_OFFSET;
            vertices[i*jMax*BUFFER_VERTEX_SIZE*VERTICES_PER_QUAD + BUFFER_VERTEX_SIZE*VERTICES_PER_QUAD*j + 2] = 0.0f;
            vertices[i*jMax*BUFFER_VERTEX_SIZE*VERTICES_PER_QUAD + BUFFER_VERTEX_SIZE*VERTICES_PER_QUAD*j + 3] = d[color_char]["code"]["r"].GetInt();
            vertices[i*jMax*BUFFER_VERTEX_SIZE*VERTICES_PER_QUAD + BUFFER_VERTEX_SIZE*VERTICES_PER_QUAD*j + 4] = d[color_char]["code"]["g"].GetInt();
            vertices[i*jMax*BUFFER_VERTEX_SIZE*VERTICES_PER_QUAD + BUFFER_VERTEX_SIZE*VERTICES_PER_QUAD*j + 5] = d[color_char]["code"]["b"].GetInt();

            vertices[i*jMax*BUFFER_VERTEX_SIZE*VERTICES_PER_QUAD + BUFFER_VERTEX_SIZE*VERTICES_PER_QUAD*j + 6] = j*QUAD_WIDTH + (1 + j)*QUAD_OFFSET;
            vertices[i*jMax*BUFFER_VERTEX_SIZE*VERTICES_PER_QUAD + BUFFER_VERTEX_SIZE*VERTICES_PER_QUAD*j + 7] = i*QUAD_HEIGHT + (1 + i)*QUAD_OFFSET + QUAD_HEIGHT;
            vertices[i*jMax*BUFFER_VERTEX_SIZE*VERTICES_PER_QUAD + BUFFER_VERTEX_SIZE*VERTICES_PER_QUAD*j + 8] = 0.0f;
            vertices[i*jMax*BUFFER_VERTEX_SIZE*VERTICES_PER_QUAD + BUFFER_VERTEX_SIZE*VERTICES_PER_QUAD*j + 9] = d[color_char]["code"]["r"].GetInt();
            vertices[i*jMax*BUFFER_VERTEX_SIZE*VERTICES_PER_QUAD + BUFFER_VERTEX_SIZE*VERTICES_PER_QUAD*j + 10] = d[color_char]["code"]["g"].GetInt();
            vertices[i*jMax*BUFFER_VERTEX_SIZE*VERTICES_PER_QUAD + BUFFER_VERTEX_SIZE*VERTICES_PER_QUAD*j + 11] = d[color_char]["code"]["b"].GetInt();

            vertices[i*jMax*BUFFER_VERTEX_SIZE*VERTICES_PER_QUAD + BUFFER_VERTEX_SIZE*VERTICES_PER_QUAD*j + 12] = j*QUAD_WIDTH + (1 + j)*QUAD_OFFSET + QUAD_WIDTH;
            vertices[i*jMax*BUFFER_VERTEX_SIZE*VERTICES_PER_QUAD + BUFFER_VERTEX_SIZE*VERTICES_PER_QUAD*j + 13] = i*QUAD_HEIGHT + (1 + i)*QUAD_OFFSET;
            vertices[i*jMax*BUFFER_VERTEX_SIZE*VERTICES_PER_QUAD + BUFFER_VERTEX_SIZE*VERTICES_PER_QUAD*j + 14] = 0.0f;
            vertices[i*jMax*BUFFER_VERTEX_SIZE*VERTICES_PER_QUAD + BUFFER_VERTEX_SIZE*VERTICES_PER_QUAD*j + 15] = d[color_char]["code"]["r"].GetInt();
            vertices[i*jMax*BUFFER_VERTEX_SIZE*VERTICES_PER_QUAD + BUFFER_VERTEX_SIZE*VERTICES_PER_QUAD*j + 16] = d[color_char]["code"]["g"].GetInt();
            vertices[i*jMax*BUFFER_VERTEX_SIZE*VERTICES_PER_QUAD + BUFFER_VERTEX_SIZE*VERTICES_PER_QUAD*j + 17] = d[color_char]["code"]["b"].GetInt();

            vertices[i*jMax*BUFFER_VERTEX_SIZE*VERTICES_PER_QUAD + BUFFER_VERTEX_SIZE*VERTICES_PER_QUAD*j + 18] = j*QUAD_WIDTH + (1 + j)*QUAD_OFFSET + QUAD_WIDTH;
            vertices[i*jMax*BUFFER_VERTEX_SIZE*VERTICES_PER_QUAD + BUFFER_VERTEX_SIZE*VERTICES_PER_QUAD*j + 19] = i*QUAD_HEIGHT + (1 + i)*QUAD_OFFSET + QUAD_HEIGHT;
            vertices[i*jMax*BUFFER_VERTEX_SIZE*VERTICES_PER_QUAD + BUFFER_VERTEX_SIZE*VERTICES_PER_QUAD*j + 20] = 0.0f;
            vertices[i*jMax*BUFFER_VERTEX_SIZE*VERTICES_PER_QUAD + BUFFER_VERTEX_SIZE*VERTICES_PER_QUAD*j + 21] = d[color_char]["code"]["r"].GetInt();
            vertices[i*jMax*BUFFER_VERTEX_SIZE*VERTICES_PER_QUAD + BUFFER_VERTEX_SIZE*VERTICES_PER_QUAD*j + 22] = d[color_char]["code"]["g"].GetInt();
            vertices[i*jMax*BUFFER_VERTEX_SIZE*VERTICES_PER_QUAD + BUFFER_VERTEX_SIZE*VERTICES_PER_QUAD*j + 23] = d[color_char]["code"]["b"].GetInt();


            //Set indices here
            indices[i*jMax*INDICES_PER_QUAD + INDICES_PER_QUAD*j] = i*jMax*VERTICES_PER_QUAD + VERTICES_PER_QUAD*j;
            indices[i*jMax*INDICES_PER_QUAD + INDICES_PER_QUAD*j + 1] = i*jMax*VERTICES_PER_QUAD + VERTICES_PER_QUAD*j + 1;
            indices[i*jMax*INDICES_PER_QUAD + INDICES_PER_QUAD*j + 2] = i*jMax*VERTICES_PER_QUAD + VERTICES_PER_QUAD*j + 2;
            indices[i*jMax*INDICES_PER_QUAD + INDICES_PER_QUAD*j + 3] = i*jMax*VERTICES_PER_QUAD + VERTICES_PER_QUAD*j + 1;
            indices[i*jMax*INDICES_PER_QUAD + INDICES_PER_QUAD*j + 4] = i*jMax*VERTICES_PER_QUAD + VERTICES_PER_QUAD*j + 2;
            indices[i*jMax*INDICES_PER_QUAD + INDICES_PER_QUAD*j + 5] = i*jMax*VERTICES_PER_QUAD + VERTICES_PER_QUAD*j + 3;


        }
    }

    fclose(fp);

    /*
    stbi_image_free(grayscale);
    */

    glm::mat4 proj = glm::ortho(-544.0f, 1600.0f, -144.0f, 1200.0f, -100.0f, 100.0f);
    //glm::mat4 proj = glm::mat4(1.0f);
    glm::mat4 model = glm::mat4(1.0f);
    //model = glm::scale(model, glm::vec3(500.0f, 500.0f, 0.0f));
    std::cout << glm::to_string(proj) << std::endl;
    //Create identity matrices for model and view.
    //Change camera position
    glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 100.0f);
    glm::vec3 cameraTarget = glm::vec3(0.0f, 0.0f, 0.0f);
    glm::vec3 cameraDirection = glm::normalize(cameraPos - cameraTarget);
    glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
    glm::vec3 cameraRight = glm::normalize(glm::cross(up, cameraDirection));
    glm::vec3 cameraUp = glm::cross(cameraDirection, cameraRight);

    glm::mat4 view;
    view = glm::lookAt(cameraPos, cameraTarget, cameraUp);

    //view = glm::mat4(1.0f);
    std::cout << "Right vector: " <<  glm::to_string(cameraRight) << std::endl;
    std::cout << "Direction vector: " <<  glm::to_string(cameraDirection) << std::endl;

    unsigned int VAO;

    glGenVertexArrays(1, &VAO);

    unsigned int VBO;

    glGenBuffers(1, &VBO);

    unsigned int EBO;

    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    int vertices_size = sizeof(float) * iMax*jMax*BUFFER_VERTEX_SIZE*VERTICES_PER_QUAD;
    int indices_size = sizeof(unsigned int) * iMax*jMax*INDICES_PER_QUAD;

    glBufferData(GL_ARRAY_BUFFER, vertices_size, vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);

    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices_size, indices, GL_STATIC_DRAW);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, BUFFER_VERTEX_SIZE * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // gradient attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, BUFFER_VERTEX_SIZE * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    Shader textureShader("shaders/triangleVertexShader.vert", "shaders/triangleFragmentShader.frag");

    textureShader.use();
    textureShader.setFloatMatrix("projection", glm::value_ptr(proj));
    std::cout << "From top-left corner(" << 0 << "," << 600 << ") to bottom-right corner(" << 800 << "," << 0 << ")" << std::endl;
    textureShader.setFloatMatrix("view", glm::value_ptr(view));
    textureShader.setFloatMatrix("model", glm::value_ptr(model));
    //textureShader.setGradients("gradients", glm::value_ptr(gradients[0]));
    //textureShader.setGradients("gridPoints", glm::value_ptr(gridPoints[0]));
    //textureShader.setFloat("jMax", jMax);
    cout << "Vertices" << endl;

    // While window is not being closed
    while(!glfwWindowShouldClose(window))
    {
        // input
        processInput(window, textureShader);

        // draw the background color

        //glClear(GL_COLOR_BUFFER_BIT);
        //Creating the map
        glClearColor(0.4f, 0.4f, 0.4f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glBindVertexArray(VAO);
        //textureShader.setFloat("randomTiling", randomTiling[0]);

        glDrawElements(GL_TRIANGLES, INDICES_PER_QUAD*iMax*jMax, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);


        // draw stuff
        //render(VAO, VBO, vertices, (int)sizeof(vertices), textureShader, randomTiling, iMax, jMax);

        //check events and swap the buffers
        glfwPollEvents();
        glfwSwapBuffers(window);

        //use Sleep(int millis) in Windows
        //usleep(100000);
    }

    /*
    cout << "Gradients" << endl;

    for(int i = 0; i < (iMax + 1)*(jMax + 1); i++){
        cout << glm::to_string(gradients[i]) << endl;
    }


    cout << "GridPoints" << endl;

    for(int i = 0; i < iMax*jMax; i++){
        cout << glm::to_string(gridPoints[i]) << endl;
    }


    cout << "Indices" << endl;

    for (int i = 0; i < iMax*jMax*INDICES_PER_QUAD; i++){
        cout << (float)indices[i] << endl;
    }
    */

    cout << "Vertices" << endl;

    for (int i = 0; i < iMax*jMax*VERTICES_PER_QUAD*BUFFER_VERTEX_SIZE; i++){
        if(i%BUFFER_VERTEX_SIZE == 0){
                cout << ((float)i)/((float)BUFFER_VERTEX_SIZE) << "---------" << endl;
        }
        cout << (float)vertices[i] << endl;
    }



    /*
    cout << "Z Vertices" << endl;
    int count = 2;
    while(count < iMax*jMax*4*5){
        cout << vertices[count] << endl;
        count += 5;
    }
    */


    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);


    delete [] vertices;
    delete [] indices;


    glfwTerminate();
    return 0;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void processInput(GLFWwindow *window, Shader textureShader)
{
    float randomTiling, right, left, top, bottom;
    GLfloat GLprojection[16];
    glGetUniformfv(textureShader.getID(), glGetUniformLocation(textureShader.getID(), "randomTiling"), &randomTiling);
    glGetUniformfv(textureShader.getID(), glGetUniformLocation(textureShader.getID(), "projection"), GLprojection);
    glm::mat4 GLMprojection = glm::make_mat4(GLprojection);


    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
    if(glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS && randomTiling < 0.995){
        textureShader.setFloat("randomTiling", randomTiling + 0.005);
        std::cout << randomTiling + 0.005 << std::endl;
    }
    if(glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS && randomTiling > 0.005){
        textureShader.setFloat("randomTiling", randomTiling - 0.005);
        std::cout << randomTiling - 0.005 << std::endl;
    }
    if(glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS){
        GLMprojection = glm::translate(GLMprojection, glm::vec3(0.0f, -10.0f, 0.0f));
        right = (1 - GLMprojection[3][0])/GLMprojection[0][0];
        left = (-GLMprojection[3][0] - 1)/GLMprojection[0][0];
        top = (1 - GLMprojection[3][1])/GLMprojection[1][1];
        bottom = (-GLMprojection[3][1] - 1)/GLMprojection[1][1];
        textureShader.setFloatMatrix("projection", glm::value_ptr(GLMprojection));
        std::cout << "From top-left corner(" << left << "," << top << ") to bottom-right corner(" << right << "," << bottom << ")" << std::endl;
    }
    if(glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS){
        GLMprojection = glm::translate(GLMprojection, glm::vec3(0.0f, 10.0f, 0.0f));
        right = (1 - GLMprojection[3][0])/GLMprojection[0][0];
        left = (-GLMprojection[3][0] - 1)/GLMprojection[0][0];
        top = (1 - GLMprojection[3][1])/GLMprojection[1][1];
        bottom = (-GLMprojection[3][1] - 1)/GLMprojection[1][1];
        textureShader.setFloatMatrix("projection", glm::value_ptr(GLMprojection));
        std::cout << "From top-left corner(" << left << "," << top << ") to bottom-right corner(" << right << "," << bottom << ")" << std::endl;
    }
    if(glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS){
        GLMprojection = glm::translate(GLMprojection, glm::vec3(10.0f, 0.0f, 0.0f));
        right = (1 - GLMprojection[3][0])/GLMprojection[0][0];
        left = (-GLMprojection[3][0] - 1)/GLMprojection[0][0];
        top = (1 - GLMprojection[3][1])/GLMprojection[1][1];
        bottom = (-GLMprojection[3][1] - 1)/GLMprojection[1][1];
        textureShader.setFloatMatrix("projection", glm::value_ptr(GLMprojection));
        std::cout << "From top-left corner(" << left << "," << top << ") to bottom-right corner(" << right << "," << bottom << ")" << std::endl;
    }
    if(glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS){
        GLMprojection = glm::translate(GLMprojection, glm::vec3(-10.0f, 0.0f, 0.0f));
        right = (1 - GLMprojection[3][0])/GLMprojection[0][0];
        left = (-GLMprojection[3][0] - 1)/GLMprojection[0][0];
        top = (1 - GLMprojection[3][1])/GLMprojection[1][1];
        bottom = (-GLMprojection[3][1] - 1)/GLMprojection[1][1];
        textureShader.setFloatMatrix("projection", glm::value_ptr(GLMprojection));
        std::cout << "From top-left corner(" << left << "," << top << ") to bottom-right corner(" << right << "," << bottom << ")" << std::endl;
    }
}

void render(unsigned int VAO, unsigned int VBO, unsigned int vertices [], int vertices_size, Shader textureShader, int iMax, int jMax)
{
    //Esto es estático por lo que se podría sacar del bucle (al menos si no hay agua)

    glBindVertexArray(VAO);
    /*
    for(int i = iMax - 1; i > -1; i--){
        for(int j = 0; j < jMax; j++){

            vertices[0] = j*128.0f + 128.0f;
            vertices[1] = i*64.0f + 64.0f;
            //vertices[2] = 30.0f;
            //vertices[2] = 5.0f + i;
            vertices[2] = (heights[i*(jMax + 1) + j + HEIGHTS_MAT_DIM + 1]/255.0f) * 100.0f; //top right

            vertices[5] = j*128.0f + 128.0f;
            vertices[6] = i*64.0f;
            //vertices[7] = 30.0f;
            //vertices[7] = 5.0f + i;
            vertices[7] = (heights[i*(jMax + 1) + j + 1]/255.0f) * 100.0f; //bottom right

            vertices[10] = j*128.0f;
            vertices[11] = i*64.0f;
            //vertices[12] = 40.0f;
            //vertices[12] = 5.0f + i;
            vertices[12] = (heights[i*(jMax + 1) + j]/255.0f) * 100.0f; //bottom left

            vertices[15] = j*128.0f;
            vertices[16] = i*64.0f + 64.0f;
            //vertices[17] = 50.0f;
            //vertices[17] = 5.0f + i;
            vertices[17] = (heights[i*(jMax + 1) + j + HEIGHTS_MAT_DIM]/255.0f) * 100.0f; //top left


            //std::cout << "i: " << i << " j: " << j << " TopRight: " << vertices[2] << " BottomRight: " << vertices[7] << " BottomLeft: " << vertices[12] << " TopLeft: " << vertices[17]<< std::endl;
            //std::cout << "i: " << i << " j: " << j << " TopRight: " << heights[i*(jMax + 1) + j + HEIGHTS_MAT_DIM + 1] << " BottomRight: " << heights[i*(jMax + 1) + j + 1] << " BottomLeft: " << heights[i*(jMax + 1) + j] << " TopLeft: " << heights[i*(jMax + 1) + j + HEIGHTS_MAT_DIM]<< std::endl;
            glBindBuffer(GL_ARRAY_BUFFER, VBO);
            glBufferData(GL_ARRAY_BUFFER, vertices_size, vertices, GL_STATIC_DRAW);

            //glm::mat4 model = glm::mat4(1.0f);
            //model = glm::translate(model, glm::vec3(i*128.0f, j*128.0f, 0.0f));
            //textureShader.setFloatMatrix("model", glm::value_ptr(model));
            textureShader.setFloat("randomTiling", randomTiling[i*jMax + j]);
            glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        }
        //std::cout << "----" << std::endl;
    }
    */

    for(int i = 0; i < iMax; i++){
         for(int j = 0; j < jMax; j++){
             //textureShader.setFloat("randomTiling", randomTiling[i*jMax + j]);
             glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void*)(6 * (i*jMax + j) * sizeof(GLuint)));
         }
    }

    //glDrawArrays(GL_TRIANGLES, 0, 8);
    //vertices[]
    //textureShader.setFloat("randomTiling", randomTiling[1]);
    //glDrawArrays(GL_TRIANGLE_STRIP, 4, 4);


    //std::cout << "--------------" << std::endl;
    glBindVertexArray(0);
}

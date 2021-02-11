#pragma once
#ifndef SHADER_H
#define SHADER_H

//#include <glad/glad.h> // include glad to get all the required OpenGL headers
#include <GL/glew.h>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

#endif

class Shader
{
private:
    // the program ID
    unsigned int ID;
public:
    Shader();
    // constructor reads and builds the shader
    Shader(const GLchar* vertexPath, const GLchar* fragmentPath);
    // use/activate the shader
    void use();
    // utility uniform functions
    void setBool(const std::string& name, bool value) const;
    void setInt(const std::string& name, int value) const;
    void setFloat(const std::string& name, float value) const;
    void setFloat2DVector(const std::string& name, float x, float y) const;
    void setFloatMatrix(const std::string& name, GLfloat* value) const;
    void setGradients(const std::string& name, GLfloat* value) const;
    unsigned int getID();
};

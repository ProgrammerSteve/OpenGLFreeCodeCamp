#pragma once
#include<glad/glad.h>
#include<iostream>

#define ASSERT(x) if (!(x)) __debugbreak(); 
#define GLCall(x) GLClearError();\
    x;\
    ASSERT(GLLogCall(#x, __FILE__, __LINE__))

static void GLClearError()
{
    //runs through all the errors to clear it
    while (glGetError() != GL_NO_ERROR);
}

static bool GLLogCall(const char* function, const char* file, int line)
{
    //as long as the error is not false
    while (GLenum error = glGetError()) {
        std::cout << "[OpenGL Error] (" << error << "): "
            << function << " " << file << ":" << line << std::endl;
        return false;
    }
    return true;
}
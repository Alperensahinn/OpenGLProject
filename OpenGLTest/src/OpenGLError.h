#pragma once

#include <glad/glad.h>
#include <iostream>

#ifndef GLERROR
#define GLERROR

bool error_callback_breakpoint_enable = false;

#define GLCall(x) x;\
    if (error_callback_breakpoint_enable) __debugbreak();

void GLAPIENTRY MessageCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar * message, const void* userParam)
{
    if (type == GL_DEBUG_TYPE_ERROR)
    {
        error_callback_breakpoint_enable = true;
        std::cout << "[OpenGL ERROR] (" << type << ") " << message << std::endl;
    }
    else if (type == GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR)
    {
        std::cout << "[OpenGL DEPRECATED_BEHAVIOR] (" << type << ") " << message << std::endl;
    }
    else if (type == GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR)
    {
        std::cout << "[OpenGL UNDEFINED_BEHAVIOR] (" << type << ") " << message << std::endl;
    }
    else if (type == GL_DEBUG_TYPE_PORTABILITY)
    {
        std::cout << "[OpenGL PORTABILITY] (" << type << ") " << message << std::endl;
    }
    else if (type == GL_DEBUG_TYPE_PERFORMANCE)
    {
        std::cout << "[OpenGL PERFORMANCE] (" << type << ") " << message << std::endl;
    }
    else if (type == GL_DEBUG_TYPE_OTHER)
    {
        std::cout << "[OpenGL OTHER] (" << type << ") " << message << std::endl;
    }
}

#endif 


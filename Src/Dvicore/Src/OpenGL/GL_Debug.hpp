#pragma once

#include <glad/glad.h>
#include "Assert.hpp"

namespace DviCore 
{
#ifdef _DEBUG

void GLAPIENTRY DebugMessageCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam); 

#endif
}
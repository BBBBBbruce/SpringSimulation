#ifndef TOOL_H
#define TOOL_H

#include <glew.h>
#include <SDL_opengl.h>

void dprintf(const char* fmt, ...);
GLuint CompileGLShader(const char* pchShaderName, const char* pchVertexShader, const char* pchFragmentShader);

#endif // !TOOL_H


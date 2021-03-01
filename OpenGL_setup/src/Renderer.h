#pragma once

#include <GL/glew.h>

#include "VertexArray.h"
#include "IndexBuffer.h"
#include "Shader.h"

/*Se definen las sentencias necesarias que permiten evaluar cada 
función utilizada para encontrar un error en ellas.*/
#define ASSERT(x) if (!(x)) __debugbreak();
#define GLCall(x) GLClearError();\
    x;\
    ASSERT(GLLogCall(#x, __FILE__, __LINE__)) /*permite imprimir exactamente en que función ocurrio un error + el numero de linea,
                                                tambien en que archivo se encuentra dicha función*/
void GLClearError();

bool GLLogCall(const char* function, const char* file, int line);

class Renderer
{
public:
    void Clear() const;
    void Draw(const VertexArray& va, const IndexBuffer& ib, const Shader& shader) const;

};
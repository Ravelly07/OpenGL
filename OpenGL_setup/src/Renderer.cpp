/*Render.cpp*/
//Esta clase nos permite identificar errores que ocurran al trabajar con OpenGL 
#include "Renderer.h"

#include <iostream>

//identifica los errores del programa, bucle que se ejecuta hasta se topa con un error.
void GLClearError()
{
    while (glGetError() != GL_NO_ERROR);
}

//Imprime en consola los errores que existan en el programa 
bool GLLogCall(const char* function, const char* file, int line)
{
    while (GLenum error = glGetError())
    {
        std::cout << "[OpenGL Error] (" << error << "): " << function << " " << ":" << line << std::endl;
        return false;
    }
    return true;
}

void Renderer::Clear() const
{
    GLCall(glClear(GL_COLOR_BUFFER_BIT));
    GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT););
}
//link the objets and call the draw element fuction
void Renderer::Draw(const VertexArray& va, const Shader& shader) const
{
    shader.Bind();
    va.Bind();
    GLCall(glDrawArrays(GL_TRIANGLES, 0, 36));/*Se indica que se va a dibujar
                                                                               cuantos indices hay en el 
                                                                               IndexBuffer, el tipo de atributo
                                                                               que este es, y por ultimo y dado a que
                                                                               el puntero ya fue definido
                                                                               el cuarto parametro en un valor vacio*/
}
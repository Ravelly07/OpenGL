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
    GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));/*Limpiamos el bufer de color 
                                                               y el de profundidad -- Agregado*/
    GLCall(glClearColor(0.3f, 0.3f, 0.3f, 1.0f));//Aplicamos un color al bg -- Agregado
}
//link the objets and call the draw element fuction
void Renderer::Draw(const VertexArray& va, const Shader& shader) const 
{/*En esta función eliminamos la sulicitud d eun index buffer, pues no se esta utilizando*/
    shader.Bind();
    va.Bind();
    GLCall(glDrawArrays(GL_TRIANGLES, 0, 36));/*Dibujamos el array de posiciones*/
}
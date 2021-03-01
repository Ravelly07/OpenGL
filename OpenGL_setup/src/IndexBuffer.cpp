/*IndexBuffer.cpp*/
#include "IndexBuffer.h"

#include "Renderer.h"
/*Creación del objeto, esta clase tiene una estructura similar a la clase VertexBuffer
No podemos usar la misma clase debido a que el indexBuffer no puede recivir numeros con 
signo*/
IndexBuffer::IndexBuffer(const unsigned int* data, unsigned int count)
    : m_Count(count)//obtenemos el tamaño del buffer
{
    ASSERT(sizeof(unsigned int) == sizeof(GLuint));

    GLCall(glGenBuffers(1, &m_RendererID));
    GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID));
    GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(unsigned int), data, GL_STATIC_DRAW));
}
//eliminación del objeto
IndexBuffer::~IndexBuffer()
{
    GLCall(glDeleteBuffers(1, &m_RendererID));
}
//Se crea el enlace 
void IndexBuffer::Bind() const
{
    GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID));
}
//se limpia el enlace
void IndexBuffer::Unbind() const
{
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
}
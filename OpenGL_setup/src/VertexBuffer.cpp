/*VertexBuffer.cpp*/
#include "VertexBuffer.h"

#include "Renderer.h"
//Creamos el Vertexbuffer
VertexBuffer::VertexBuffer(const void* data, unsigned int size)
{
    GLCall(glGenBuffers(1, &m_RendererID));//Se genera el buffer dado un identificador
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_RendererID));//Conectamos al buffer
    GLCall(glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW));//Se cargan los datos data 
                                                                      /*En un buffer de tama�o size(en bytes)*/
}
//Se eleimina el buffer
VertexBuffer::~VertexBuffer()
{
    GLCall(glDeleteBuffers(1, &m_RendererID));
}
/*Se liga al buffer (Se realiza tambien en la creaci�n, pero como se limpia antes de correr
el programa, se necesita volver a ligarlo*/
void VertexBuffer::Bind() const
{
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_RendererID));
}
//Se limpia el enlace del buffer
void VertexBuffer::Unbind() const
{
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
}
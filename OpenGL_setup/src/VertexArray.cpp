/*VertexArrey.cpp*/
/*Esta clase controla la generaci�n de arrays necesarios para nuestro programa*/
#include "VertexArray.h"
#include "VertexBufferLayout.h"
#include "Renderer.h"
//Se crea el identificador del array
VertexArray::VertexArray()
{
	GLCall(glGenVertexArrays(1, &m_RendererID));

}
//Se elimina el vertexArray
VertexArray::~VertexArray()
{
	GLCall(glDeleteVertexArrays(1, &m_RendererID));
}
//Se crea el apuntador a los atributos del array
void VertexArray::AddBuffer(const VertexBuffer& vb, const VertexBufferLayout& layout)
{
	Bind();  //bind the vertex array
	vb.Bind();	//bind the vertex buffer
	const auto& elements = layout.GetElements();
	unsigned int offset = 0;
	for (unsigned int i = 0; i < elements.size(); i++)
	{
		const auto& element = elements[i];
		GLCall(glEnableVertexAttribArray(i));
		GLCall(glVertexAttribPointer(i, element.count, element.type,
			element.normalized, layout.GetStride(), (const void*)offset));
		offset += element.count * VertexBufferElement::GetSizeOfType(element.type);
	}
}
//Se liga el array con el identiicador 
void VertexArray::Bind() const
{
	GLCall(glBindVertexArray(m_RendererID));
}
//Se limpia el anlace
void VertexArray::Unbind() const
{
	GLCall(glBindVertexArray(0));
}
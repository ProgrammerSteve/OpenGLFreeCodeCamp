#include"VAO.h"
#include "GLDebug.h"

// Constructor that generates a VAO ID
VAO::VAO()
{
	GLCall(glGenVertexArrays(1, &ID));
}

// Links a VBO to the VAO using a certain layout
void VAO::LinkAttrib(VBO& VBO, GLuint layout, GLuint numComponents, GLenum type, GLsizeiptr stride, void* offset)
{
	VBO.Bind();
	GLCall(glVertexAttribPointer(layout, numComponents, type, GL_FALSE, stride, offset));
	GLCall(glEnableVertexAttribArray(layout));
	VBO.Unbind();
}

// Binds the VAO
void VAO::Bind()
{
	GLCall(glBindVertexArray(ID));
}

// Unbinds the VAO
void VAO::Unbind()
{
	GLCall(glBindVertexArray(0));
}

// Deletes the VAO
void VAO::Delete()
{
	GLCall(glDeleteVertexArrays(1, &ID));
}
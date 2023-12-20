#pragma once

#include<glad/glad.h>
#include<string>
#include<fstream>
#include<sstream>
#include<iostream>
#include<cerrno>

class VBO
{
public:
	GLuint ID;
	VBO(GLfloat* vertices, GLsizeiptr size);

	void Bind();
	void Unbind();
	void Delete();
};
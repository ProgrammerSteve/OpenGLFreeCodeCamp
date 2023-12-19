#include<iostream>
#include<glad/glad.h>
#include<GLFW/glfw3.h>

#include "shaderClass.h"
#include "VBO.h"
#include "EBO.h"
#include "VAO.h"
#include "stb/stb_image.h"

//vertices coordinates for our triangle
//XYZRGB
//GLfloat vertices[] =
//{
//	-0.5f, -0.5f * float(sqrt(3)) / 3, 0.0f, 0.8f, 0.3f, 0.02f,// Lower left corner
//	0.5f, -0.5f * float(sqrt(3)) / 3, 0.0f, 0.8f, 0.3f, 0.2f,// Lower right corner
//	0.0f, 0.5f * float(sqrt(3)) * 2 / 3, 0.0f, 1.0f, 0.6f, 0.32f,// Upper corner
//	-0.5f / 2, 0.5f * float(sqrt(3)) / 6, 0.0f, 0.9f,0.45f,0.17f,// Inner left
//	0.5f / 2, 0.5f * float(sqrt(3)) / 6, 0.0f,0.9f,0.45f,0.17f, // Inner right
//	0.0f, -0.5f * float(sqrt(3)) / 3, 0.0f,0.8f,0.3f,0.2f // Inner down
//};

// Indices for vertices order
//GLuint indices[] =
//{
//	0, 3, 5, // Lower left triangle
//	3, 2, 4, // Upper triangle
//	5, 4, 1 // Lower right triangle
//};


// Vertices coordinates
GLfloat vertices[] =
{ //     COORDINATES     /        COLORS       //
	-0.5f, -0.5f, 0.0f,     1.0f, 0.0f, 0.0f,	0.0f, 0.0f, // Lower left corner
	-0.5f,  0.5f, 0.0f,     0.0f, 1.0f, 0.0f,	0.0f, 1.0f, // Upper left corner
	 0.5f,  0.5f, 0.0f,     0.0f, 0.0f, 1.0f,	1.0f, 1.0f, // Upper right corner
	 0.5f, -0.5f, 0.0f,     1.0f, 1.0f, 1.0f,	1.0f, 0.0f  // Lower right corner
};



// Indices for vertices order
GLuint indices[] =
{
	0, 2, 1, // Upper triangle
	0, 3, 2 // Lower triangle
};



int main() 
{
	std::cout << "Starting code..." << std::endl;
	glfwInit();

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(800, 800, "YoutubeOpenGL", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	//Load GLAD so it configures OpenGL
	gladLoadGL();
	// Specify the viewport of OpenGL in the Window
	// In this case the viewport goes from x = 0, y = 0, to x = 800, y = 800
	glViewport(0, 0, 800, 800);


	// Generates Shader object using shaders defualt.vert and default.frag
	Shader shaderProgram("default.vert", "default.frag");

	// Generates Vertex Array Object and binds it
	VAO VAO1;
	VAO1.Bind();
	VBO VBO1(vertices, sizeof(vertices));// Generates Vertex Buffer Object and links it to vertices
	EBO EBO1(indices, sizeof(indices));// Generates Element Buffer Object and links it to indices

	// Links VBO to VAO
	VAO1.LinkAttrib(VBO1, 0, 3, GL_FLOAT, 8 *sizeof(float),(void*)0);//coordinates
	VAO1.LinkAttrib(VBO1, 1, 3, GL_FLOAT, 8 * sizeof(float), (void*)(3*sizeof(float)));//colors
	VAO1.LinkAttrib(VBO1, 2, 2, GL_FLOAT, 8 * sizeof(float), (void*)(6 * sizeof(float)));//textures

	// Unbind all to prevent accidentally modifying them
	VAO1.Unbind();
	VBO1.Unbind();
	EBO1.Unbind();

	//can only be used after the shaderProgram is activated
	GLuint uniID = glGetUniformLocation(shaderProgram.ID, "scale");

	//Texture image details, wxh number of color channels
	int widthImg, heightImg, numColCh;
	stbi_set_flip_vertically_on_load(true);
	unsigned char* bytes = stbi_load("clock.png", &widthImg, &heightImg, &numColCh, 0);

	GLuint texture;
	glGenTextures(1, &texture);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, widthImg, heightImg, 0, GL_RGBA, GL_UNSIGNED_BYTE, bytes);
	glGenerateMipmap(GL_TEXTURE_2D);//genertate mipmaps of other resolutions

	stbi_image_free(bytes);
	glBindTexture(GL_TEXTURE_2D, 0);

	
	GLuint tex0Uni = glGetUniformLocation(shaderProgram.ID, "tex0");
	shaderProgram.Activate();// Tell OpenGL which Shader Program we want to use
	glUniform1f(tex0Uni, 0);

	while (!glfwWindowShouldClose(window)) 
	{
		glClearColor(0.07f, 0.13f, 0.17f, 1.0f);// Specify the color of the background
		glClear(GL_COLOR_BUFFER_BIT);// Clean the back buffer and assign the new color to it
		shaderProgram.Activate();// Tell OpenGL which Shader Program we want to use

		glUniform1f(uniID, 0.5f);
		glBindTexture(GL_TEXTURE_2D, texture);

		VAO1.Bind();// Bind the VAO so OpenGL knows to use it
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);// Draw primitives, number of indices, datatype of indices, index of indices
		glfwSwapBuffers(window);// Swap the back buffer with the front buffer
		glfwPollEvents();// Take care of all GLFW events
	}

	// Delete all the objects we've created
	VAO1.Delete();
	VBO1.Delete();
	EBO1.Delete();
	glDeleteTextures(1,&texture);
	shaderProgram.Delete();

	glfwDestroyWindow(window);// Delete window before ending the program
	glfwTerminate();// Terminate GLFW before ending the program
	return 0;
}
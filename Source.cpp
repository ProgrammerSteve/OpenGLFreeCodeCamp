#include<iostream>
#include<glad/glad.h>
#include<GLFW/glfw3.h>

// Vertex Shader source code
const char* vertexShaderSource = "#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"void main()\n"
"{\n"
"   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
"}\0";


//Fragment Shader source code
const char* fragmentShaderSource = "#version 330 core\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
"   FragColor = vec4(0.8f, 0.3f, 0.02f, 1.0f);\n"
"}\n\0";

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


	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);// Create Vertex Shader Object and get its reference
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);// Attach Vertex Shader source to the Vertex Shader Object
	glCompileShader(vertexShader);// Compile the Vertex Shader into machine code so gpu understands
	
	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);// Create Fragment Shader Object and get its reference
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);// Attach Fragment Shader source to the Fragment Shader Object
	glCompileShader(fragmentShader);// Compile the Vertex Shader into machine code so gpu understands

	GLuint shaderProgram = glCreateProgram();// Create Shader Program Object and get its reference
	glAttachShader(shaderProgram, vertexShader);// Attach the Vertex Shader to the Shader Program
	glAttachShader(shaderProgram, fragmentShader);// Attach the Fragment Shader to the Shader Program
	glLinkProgram(shaderProgram);// Wrap-up/Link all the shaders together into the Shader Program

	// Delete the now useless Vertex and Fragment Shader objects
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	//vertices coordinates for our triangle
	GLfloat vertices[] =
	{
		-0.5f, -0.5f * float(sqrt(3)) / 3, 0.0f, // Lower left corner
		0.5f, -0.5f * float(sqrt(3)) / 3, 0.0f, // Lower right corner
		0.0f, 0.5f * float(sqrt(3)) * 2 / 3, 0.0f // Upper corner
	};



	GLuint VAO, VBO;// Create reference containers for the Vartex Array Object and the Vertex Buffer Object

	// Generate the VAO and VBO with only 1 object each
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);


	// Make the VAO the current Vertex Array Object by binding it
	glBindVertexArray(VAO);


	// Bind the VBO specifying it's a GL_ARRAY_BUFFER
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	// Introduce the vertices into the VBO
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);


	// Configure the Vertex Attribute so that OpenGL knows how to read the VBO
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3*sizeof(float), (void*)0);
	// Enable the Vertex Attribute so that OpenGL knows to use it
	glEnableVertexAttribArray(0);


	// Bind both the VBO and VAO to 0 so that we don't accidentally modify the VAO and VBO we created
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);


	while (!glfwWindowShouldClose(window)) 
	{
		glClearColor(0.07f, 0.13f, 0.17f, 1.0f);// Specify the color of the background
		glClear(GL_COLOR_BUFFER_BIT);// Clean the back buffer and assign the new color to it
		glUseProgram(shaderProgram);// Tell OpenGL which Shader Program we want to use
		glBindVertexArray(VAO);// Bind the VAO so OpenGL knows to use it
		glDrawArrays(GL_TRIANGLES, 0, 3);// Draw the triangle using the GL_TRIANGLES primitive
		glfwSwapBuffers(window);// Swap the back buffer with the front buffer
		glfwPollEvents();// Take care of all GLFW events
	}

	// Delete all the objects we've created
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteProgram(shaderProgram);

	glfwDestroyWindow(window);// Delete window before ending the program
	glfwTerminate();// Terminate GLFW before ending the program
	return 0;
}
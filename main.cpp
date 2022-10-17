#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

// Vertex Shader source code
const char* vertex_shader_source = "#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"void main()\n"
"{\n"
"   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
"}\0";
//Fragment Shader source code
const char* fragment_shader_source = "#version 330 core\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
"   FragColor = vec4(255.0f, 255.0f, 255.0f, 1.0f);\n"
"}\n\0";

int main()
{
	// init glfw
	glfwInit();
	
	// tell glfw what version of OpenGL we are using (3.3)
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	// tell glfw we are using the CORE profile so we only have the modern functions
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// vertices coordinates
	GLfloat vertices[] =
	{
		-0.5f, -0.5f * float(sqrt(3)) / 3, 0.0f,    // Lower left corner
		0.5f, -0.5f * float(sqrt(3)) / 3, 0.0f,		// Lower right corner
		0.0f, 0.5f * float(sqrt(3)) * 2 / 3, 0.0f,	// Upper corner
		-0.5f / 2, 0.5f * float(sqrt(3)) / 6, 0.0f,	// Inner left
		0.5f / 2, 0.5f * float(sqrt(3)) / 6, 0.0f,	// Inner right
		0.0f, -0.5f * float(sqrt(3)) / 3, 0.0f		// Inner down
	};

	// vertex drawing order
	constexpr GLuint indices[] =
	{
		0, 3, 5,	// Lower left triangle
		3, 2, 4,	// Lower right triangle
		5, 4, 1		// Upper triangle
	};
	
	// create window
	GLFWwindow* window = glfwCreateWindow(800, 800, "Window", nullptr, nullptr);

	// check an issues while creating the window
	if (window == nullptr)
	{
		std::cout << "Failed to create window \n";
		glfwTerminate();
		return -1;
	}
	// introduce the window into the current context
	glfwMakeContextCurrent(window);

	// load GLAD so it configures OpenGL
	gladLoadGL();

	// specify the viewport of OpenGl in the window
	glViewport(0, 0, 800, 800);

	/*--------------------------------------------------------------------------------------------*/
	
	// create vertex shader obj and get its reference (specify, what kind of shader i gonna use)
	const GLuint vertex_shader = glCreateShader(GL_VERTEX_SHADER);
	// attach vertex shader src to the vertex shader obj
	glShaderSource(vertex_shader, 1, &vertex_shader_source, nullptr);
	// compile the vertex shader into machine code / compile vertex shader
	glCompileShader(vertex_shader);

	// create fragment shader obj and get its reference
	const GLuint fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
	// attach fragment shader src to the fragment shader obj
	glShaderSource(fragment_shader, 1, &fragment_shader_source, nullptr);
	// compile the fragment shader into machine code
	glCompileShader(fragment_shader);

	// create shader program obj and get its reference
	const GLuint shader_program = glCreateProgram();
	//attach the vertex and fragment shaders to the shader program
	glAttachShader(shader_program, vertex_shader);
	glAttachShader(shader_program, fragment_shader);
	// wrap-up / link all the shaders together into the shader program
	glLinkProgram(shader_program);
	
	// delete the now useless vertex and fragment shader objects
	glDeleteShader(vertex_shader);
	glDeleteShader(fragment_shader);

	/*--------------------------------------------------------------------------------------------*/
	
	// create reference containers for the vertex array obj and the vertex buffer obj
	GLuint vao, vbo, ebo;

	// generate the vao and vbo with only 1 obj each
	glGenVertexArrays(1, &vao);
	glGenBuffers(1, &vbo);
	glGenBuffers(1, &ebo);

	// make the vao the current vertex array obj by binding it
	glBindVertexArray(vao);

	// bind the vbo specifying its a GL_ARRAY_BUFFER
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	// introduce the vertices into the vbo
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// configure the vertex attribute so that OpenGL knows how to read the VBO
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	// enable the vertex attribute so that OpenGL knows to use it
	glEnableVertexAttribArray(0);

	// bind both the vbo and vao to 0 so that we dont accidentally modify the vao and vbo we created
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	// specify the color of the background
	glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
	// clean the back buffer and assign the new color to it
	glClear(GL_COLOR_BUFFER_BIT);
	// swap the back buffer with the front buffer
	glfwSwapBuffers(window);

	// keep the window opened / main loop
	while(!glfwWindowShouldClose(window))
	{
		glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		// tell OpenGL which shader program we want to use
		glUseProgram(shader_program);
		// bind the vao so OpenGL knows to use it
		glBindVertexArray(vao);
		// draw triangle using the GL_TRIANGLES primitive
		glDrawElements(GL_TRIANGLES, 9, GL_UNSIGNED_INT, nullptr);
		/* glDrawArrays(GL_TRIANGLES, 0, 3); */
		glfwSwapBuffers(window);
		// process all the pooled events / make the window responding / take care of all GLFW event
		glfwPollEvents();
	}

	// delete all the objects we've created
	glDeleteVertexArrays(1, &vao);
	glDeleteBuffers(1, &vbo);
	glDeleteBuffers(1, &vbo);
	glDeleteProgram(shader_program);

	// destroy window before ending the program
	glfwDestroyWindow(window);

	// terminate glfw before ending the program
	glfwTerminate();
	return 0;
}
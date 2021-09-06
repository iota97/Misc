#include "glad/glad.h"
#include <GLFW/glfw3.h>
#include <stdio.h>

// Resize callback
void FramebufferSizeCallback(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
}

// Input process
void processInput(GLFWwindow *window) {
	if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
}

const char *vertexShaderSource = 
	"#version 330 core\n"
	"layout (location = 0) in vec3 aPos;\n"
	"void main()\n"
	"{\n"
	"   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
    	"}\0";

const char *fragmentShaderSource = 
	"#version 330 core\n"
	"out vec4 FragColor;\n"
	"void main()\n"
    	"{\n"
    	"   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
   	"}\n\0";

int main(int argc, char *argv[]) {
	// Set OpenGL 3.3
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// Create windows
	GLFWwindow *window = glfwCreateWindow(800, 600, "LearnOpenGL", NULL, NULL);
	if (!window) {
		puts("Failed to create GLFW window");
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	// Initialize GLAD
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		puts("Failed to initialize GLAD");
    		return -1;
	}

	// Viewport
	glViewport(0, 0, 800, 600);
	glfwSetFramebufferSizeCallback(window, FramebufferSizeCallback);

	// Vertex shader
	unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexShader);

	int success;
	char infoLog[512];
    	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success) {
        	glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        	printf("ERROR: vertex shader compilation failed: %s\n", infoLog);
    	}
	
	// Fragment shader
	unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);

	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if (!success) {
        	glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        	printf("ERROR: fragment shader compilation failed: %s\n", infoLog);
    	}

	// Link shaders
	unsigned int shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);

	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success) {
        	glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        	printf("ERROR: shader linking failed: %s\n", infoLog);
    	}
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	// Tris
	float vertices[] = {
		-0.5f, -0.5f, 0.0f, // left  
		 0.5f, -0.5f, 0.0f, // right 
		 0.0f,  0.5f, 0.0f  // top   
    	}; 

    	unsigned int VBO, VAO;
    	glGenVertexArrays(1, &VAO);
    	glGenBuffers(1, &VBO);

	// Bind
    	glBindVertexArray(VAO);
   	glBindBuffer(GL_ARRAY_BUFFER, VBO);
    	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    	glEnableVertexAttribArray(0);

	// Unbind
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0); 

	// Main loop
	while(!glfwWindowShouldClose(window)) {
		processInput(window);

		// Rendering command
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		// draw our first triangle
		glUseProgram(shaderProgram);
		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 3);

		glfwSwapBuffers(window);
		glfwPollEvents();    
	}

	// Clean up
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteProgram(shaderProgram);

	// Terminate
	glfwTerminate();
	return 0;
}

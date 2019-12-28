#include <iostream>
#include <math.h>
#define _USE_MATH_DEFINES
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "shader.h"

// Callback function for window resize event
void framebuffer_size_callback(GLFWwindow* window, int width, int height) 
{
   	glViewport(0, 0, width, height);
}
// Input processing
void processInput(GLFWwindow *window)
{
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
	}
	else if (glfwGetKey(window, GLFW_KEY_TAB) == GLFW_PRESS) {
		std::cout << "Tab" << std::endl;
	}
}

/*--------------------------------------------------------------------*/
/*--------------------------------------------------------------------*/

// Init function for glew and glfw
GLFWwindow* initWindow() {
	GLFWwindow* window;
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	window = glfwCreateWindow(800, 600, "LearnOpenGL", NULL, NULL);
	if (window == NULL)
	{
	    std::cout << "Failed to create GLFW window" << std::endl;
	    glfwTerminate();
	    return NULL;
	}
	glfwMakeContextCurrent(window);
	glViewport(0, 0, 800, 600);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);  

	if (glewInit() != GLEW_OK) {
		std::cout << "Error initializing GLEW\n";
		return NULL;
	}
	return window;
}


int main(void)
{
	// INIT
	GLFWwindow* window = initWindow();
	if (window == NULL) {
		return -1;
	}

	// SHADERS
	Shader shader("shader.vs", "shader.fs");

	int numAttribs = 3;
	GLdouble vertices[360*numAttribs];
	float radius = 5;

	for (int i = 0; i < 360; i+=numAttribs) {
		//std::cout << "x component = " << radius * cos(i*(M_PI/180)) << "\t";
		//std::cout << "y component = " << radius * sin(i*(M_PI/180)) << std::endl;
		vertices[i] = radius * cos(i*(M_PI/180));
		vertices[i+1] = radius * sin(i*(M_PI/180));
		vertices[i+2] = 0.0f;
		/*
		vertices[i+3] = 1.0f;
		vertices[i+4] = 0.0f;
		vertices[i+5] = 0.0f;
		*/
	}

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	while (!glfwWindowShouldClose(window)) {
		processInput(window);
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		shader.use();
		glBegin(GL_LINE_LOOP);	
		for (int i = 0; i < 360*numAttribs; i+=3) {
			glVertex3d(vertices[i], vertices[i+1], vertices[i+2]);
		}
		glEnd();
		glFlush();

		glfwSwapBuffers(window);
		glfwPollEvents();

	}

	glfwTerminate();
    return 0;
}
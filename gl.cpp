#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <math.h>
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
	Shader shader("shader.vs","shader.fs");

/*
	float vertices[] = {
		  0.5f,  0.5f, 0.0f,	// 0: top right
		  0.5f, -0.5f, 0.0f, 	// 1: bottom right
		 -0.5f, -0.5f, 0.0f,	// 2: bottom left
		 -0.5f,  0.5f, 0.0f,	// 3: top left
		  0.0f,  1.0f, 0.0f,
		 -1.0f,  0.0f, 0.0f,
		  1.0f,  0.0f, 0.0f
	};


	unsigned int numIndices = 9;
	unsigned int indices[] = {
		0, 1, 3,
		1, 2, 3,
		4, 5, 6
	};
*/
	// Vertex Attributes
	// Here they are x, y, z, and r, g, b -> 6 total
	// After adding the r,g,b attributes, must add that into vertex shader!
	// 		Must also reconfigure the vertexAttribPointers
	float vertices[] = {
		// 	 x, 	y, 	  z, 	r, 	  g, 	b
		 -0.5f,  1.0f, 0.0f, 	1.0f, 0.0f, 0.0f,// top 	 	1/4		0
		  0.0f,  1.0f, 0.0f, 	1.0f, 0.0f, 0.0f,// top 		2/4 	1
		  0.5f,  1.0f, 0.0f, 	1.0f, 0.0f, 0.0f,// top 		3/4 	2

		  1.0f,  0.5f, 0.0f, 	0.0f, 1.0f, 0.0f,// right 	1/4 	3
		  1.0f,  0.0f, 0.0f, 	0.0f, 1.0f, 0.0f,// right 	2/4 	4
		  1.0f, -0.5f, 0.0f, 	1.0f, 0.0f, 0.0f,// right 	3/4 	5

		 -0.5f, -1.0f, 0.0f, 	0.0f, 0.0f, 1.0f,// bottom	1/4 	6
		  0.0f, -1.0f, 0.0f, 	0.0f, 1.0f, 0.0f,// bottom 	2/4		7
		  0.5f, -1.0f, 0.0f, 	0.0f, 0.0f, 1.0f,// bottom 	3/4	 	8

		 -1.0f,  0.5f, 0.0f, 	0.0f, 0.0f, 1.0f,// left 	1/4 	9
		 -1.0f,  0.0f, 0.0f, 	0.0f, 1.0f, 0.0f,// left 	2/4 	10
		 -1.0f, -0.5f, 0.0f,	0.0f, 0.0f, 1.0f // left 	3/4 	11
	};


	unsigned int numIndices = 12;
	unsigned int indices[] = {
		 1,   3,   9,	// top tri
		 4,   8,   2, 	// right tri
		 7,  11,   5, 	// bottom tri
		10,   0,   6	// left tri
	};
	

	// 1 .Create uint for array/buffer ids
	unsigned int VAO, VBO, EBO;

	// 2. GEN THE VAO
	glGenVertexArrays(1, &VAO);

	// 3. GEN VBO
	glGenBuffers(1, &VBO);

	// 4. GEN EBO
	glGenBuffers(1, &EBO);

	// 5. BIND THE VAO
	glBindVertexArray(VAO);

	// 6. BIND THE VBO -> GIVE DATA
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_DYNAMIC_DRAW);

	// 7. BIND THE EBO -> GIVE DATA
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_DYNAMIC_DRAW);

	// 8. SET UP VERTEX ATTRIB POINTER
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0); // for position (index = 0)
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*) (3*sizeof(float))); // for rgb color (index = 1)
	// 9. ENABLE VERTEX ATTRIB POINTER
	glEnableVertexAttribArray(0);  // enable position attribute
	glEnableVertexAttribArray(1);  // enable color attribute

	// 10. NOW YOU CAN UNBUND VBO AND VAO --> NOT EBO WHILE VBO IS STILL ACTIVE
    glBindBuffer(GL_ARRAY_BUFFER, 0); 
    glBindVertexArray(0); 

	// OPTIONAL: SET WIREFRAME MODE -- OpenGL will draw primitives this way
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

/*
	int nrAttributes;
	glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &nrAttributes);
	std::cout << "Maximum nr of vertex attributes supported: " << nrAttributes << std::endl;
*/

	while (!glfwWindowShouldClose(window)) {
		processInput(window);
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		shader.use();
		//shader.setFloat("ourColor",1.0f);

		// Update the UNIFORM value each iteration before drawing the triangle
		float timeValue = glfwGetTime();
		float greenValue = (sin(timeValue) / 2.0f) + 0.5f;
		//int vertexColorLocation = glGetUniformLocation(shader.ID,"ourColor");
		//glUniform4f(vertexColorLocation, 0.0f, greenValue, 0.0f, 1.0f);

		int vertexPosLocation = glGetUniformLocation(shader.ID,"vertPos");
		//float n = std::cin.get();
		float n;
		std::cin >> n;
		glUniform4f(vertexPosLocation, n, 0.0f, 0.0f, 0.0f);

        glBindVertexArray(VAO);

		// Draw elements vs draw arrays
		//glDrawArrays(GL_TRIANGLES, 0, 3);
		glDrawElements(GL_TRIANGLES, numIndices, GL_UNSIGNED_INT, 0);

		glfwSwapBuffers(window);
		glfwPollEvents();

	}

	glDeleteBuffers(1, &VBO);
	glfwTerminate();
    return 0;
}
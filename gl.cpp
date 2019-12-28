#include <iostream>
#include <math.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "shader.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

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


	// TEXTURE COORDINATES
	// bottom left = 0.0, 0.0 -> top right = 1.0, 1.0
	/*
	float texCoords[] = {
		0.0f, 0.0f,  // lower-left corner  
		1.0f, 0.0f,  // lower-right corner
		0.5f, 1.0f   // top-center corner
	};
	*/

	float vertices[] = {
		//   x 		y 	  z 	r 	  g 	b 	   	t1 	 	t2
		  0.5f,  0.5f, 0.0f,	1.0f, 0.0f,	0.0f, 	1.0f, 1.0f, // 0: top right
		  0.5f, -0.5f, 0.0f, 	0.0f, 1.0f, 0.0f, 	1.0f, 0.0f, // 1: bottom right
		 -0.5f, -0.5f, 0.0f,	0.0f, 0.0f, 1.0f, 	0.0f, 0.0f, // 2: bottom left
		 -0.5f,  0.5f, 0.0f,	1.0f, 1.0f, 1.0f, 	0.0f, 1.0f // 3: top left
	};



	unsigned int numIndices = 6;
	unsigned int indices[] = {
		0, 1, 2,
		0, 2, 3
	};
	// Vertex Attributes
	// Here they are x, y, z, and r, g, b -> 6 total
	// After adding the r,g,b attributes, must add that into vertex shader!
	// 		Must also reconfigure the vertexAttribPointers



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
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0); // for position (index = 0)
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*) (3*sizeof(float))); // for rgb color (index = 1)
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*) (6*sizeof(float))); // for rgb color (index = 1)

	// 9. ENABLE VERTEX ATTRIB POINTER
	glEnableVertexAttribArray(0);  // enable position attribute
	glEnableVertexAttribArray(1);  // enable color attribute
	glEnableVertexAttribArray(2);  // enable texture attribute

	// 10. NOW YOU CAN UNBUND VBO AND VAO --> NOT EBO WHILE VBO IS STILL ACTIVE
    glBindBuffer(GL_ARRAY_BUFFER, 0); 
    glBindVertexArray(0); 

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);





	// Texture/image loading

	unsigned int texture;
		// Create texture
	glGenTextures(1, &texture);
		// Bind
	glBindTexture(GL_TEXTURE_2D, texture);
		// set the texture wrapping/filtering options (on the currently bound texture object)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		// Load the image data
	int width, height, nrChannels;
	unsigned char *data = stbi_load("container.jpg", &width, &height, &nrChannels, 0);
	if (data)
	{
		// Attach image to texture object
	    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		// Generate mipmaps
	    glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
	    std::cout << "Failed to load texture" << std::endl;
	}
		// Free image memory
	stbi_image_free(data);






	while (!glfwWindowShouldClose(window)) {
		processInput(window);
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		shader.use();
		//shader.setFloat("ourColor",1.0f);

		// Update the UNIFORM value each iteration before drawing the triangle
		//float timeValue = glfwGetTime();
		//float greenValue = (sin(timeValue) / 2.0f) + 0.5f;
		//int vertexColorLocation = glGetUniformLocation(shader.ID,"ourColor");
		//glUniform4f(vertexColorLocation, 0.0f, greenValue, 0.0f, 1.0f);

		//int vertexPosLocation = glGetUniformLocation(shader.ID,"vertPos");
		//float n = std::cin.get();
		//float n;
		//std::cin >> n;
		//glUniform4f(vertexPosLocation, n, 0.0f, 0.0f, 0.0f);

		glActiveTexture(GL_TEXTURE0); // activate the texture unit first before binding texture
		glBindTexture(GL_TEXTURE_2D, texture);
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
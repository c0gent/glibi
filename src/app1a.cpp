// Local Headers
#include "glibi.hpp"

// System Headers
#include <glad/glad.h>
#include <GLFW/glfw3.h>
// Standard Headers
#include <cstdio>
#include <cstdlib>
#include <chrono>

#ifndef GLSL
#define GLSL(src) "#version 330 core\n" #src
#endif

GLFWwindow* win_ctx(float width, float height);
GLuint createShader(GLenum type, const GLchar* src);

uint sq(uint x) {
	return x * x;
}

int app1a() {
	// Shader sources
	const GLchar* vertexSource = GLSL(
		in vec2 position;
		in vec3 color;
		out vec3 Color;
		// uniform mat4 model;
		uniform mat4 model;
		uniform mat4 view;
		uniform mat4 proj;

		void main() {
			Color = color;
			// gl_Position = vec4(position, 0.0, 1.0);
			// gl_Position = vec4(position.x, -position.y, 0.0, 1.0);
			// gl_Position = model * vec4(position, 0.0, 1.0);
			// float scl = 0.0082; // Represents size of hex
			// float r = pow(pow(0.00500f, 2.0) + pow(0.00866f, 2.0), 0.5);
			// float a = 0.5f / 10.0f;
			// float s = 0.57735026919f / 10.0f; // 1/sqrt(3)
			// float h = s / 2.0f;

			// 0.173205080757 (1/sqrt(3) * 3)

			uint grid_dim = 256 << 2;

			float border = 0.01;

			float x_scl = 0.086602540378f + border;
			float y_scl = 0.05 + border;

			float u = float(uint(gl_InstanceID) % grid_dim);
			float v = float(uint(gl_InstanceID) / grid_dim);

		 	float x_ofs = ((v + u) * x_scl);
		 	float y_ofs = ((v * -y_scl) + (u * y_scl));

			gl_Position = proj * view * model * vec4( 
				position.x + x_ofs, 
				position.y + y_ofs, 
				0.0, 1.0);
		};
	);

		// #version 150 core\n	
		// in vec2 position;
		// in vec3 color;
		// in vec2 texcoord; 	
		// out vec3 Color;
		// out vec2 Texcoord;	
		// uniform mat4 model;	
		// void main() {
		//  	Color = color;
		//    Texcoord = texcoord;
		//  	gl_Position = model * vec4(position, 0.0, 1.0);
		// };

	const GLchar* fragmentSource = GLSL(
		// uniform vec3 triangleColor;
		in vec3 Color;
		out vec4 outColor;
		void main() {
		   outColor = vec4(Color, 1.0);
		//    outColor = vec4(triangleColor, 1.0);
		};
	);


	GLFWwindow* mWindow = win_ctx(1280.0f, 800.0f);


	// ////////// NON-COLOR-PASSING VERSION
	// GLfloat vertices[] = {
	// 	 0.0f,  0.433f,     // Vertex 1 (X, Y)
	// 	 0.25f, 0.0f,        // Vertex 2 (X, Y)
	// 	-0.25f, 0.0f       // Vertex 3 (X, Y)
	// };

	// Rectangle half (for use with elements):
	// GLfloat vertices[] = {
	//     -0.5f,  0.5f, 1.0f, 0.0f, 0.0f, // Top-left
	//      0.5f,  0.5f, 0.0f, 1.0f, 0.0f, // Top-right
	//      0.5f, -0.5f, 0.0f, 0.0f, 1.0f, // Bottom-right
	//     -0.5f, -0.5f, 1.0f, 1.0f, 1.0f  // Bottom-left
	// };

	// Triangle colored:
	// GLfloat vertices[] = {
	//      0.0f,  0.5f, 1.0f, 0.0f, 0.0f, // Vertex 1: Red
	//      0.5f, -0.5f, 0.0f, 1.0f, 0.0f, // Vertex 2: Green
	//     -0.5f, -0.5f, 0.0f, 0.0f, 1.0f  // Vertex 3: Blue
	// };

	// // Two-triangle square:
	// GLfloat vertices[] = {
	//     -0.5f,  0.5f, 1.0f, 0.0f, 0.0f, // Top-left
	//      0.5f,  0.5f, 0.0f, 1.0f, 0.0f, // Top-right
	//      0.5f, -0.5f, 0.0f, 0.0f, 1.0f, // Bottom-right

	//      0.5f, -0.5f, 0.0f, 0.0f, 1.0f, // Bottom-right
	//     -0.5f, -0.5f, 1.0f, 1.0f, 1.0f, // Bottom-left
	//     -0.5f,  0.5f, 1.0f, 0.0f, 0.0f  // Top-left
	// };

	// HEXAGON v.1:
	// GLfloat vertices[] = {     
	// 	 0.0f,   0.0f, 1.0f, 1.0f, 1.0f,    // center 		0
	// 	-0.25f,  0.5f, 0.0f, 1.0f, 0.0f,    // left top 	1
	// 	 0.25f,  0.5f, 0.0f, 0.0f, 1.0f,    // right top 	2
	// 	 0.5f,   0.0f, 0.0f, 1.0f, 0.0f,    // right 		3
	// 	 0.25f, -0.5f, 1.0f, 0.0f, 0.0f,    // right bottom 4
	// 	-0.25f, -0.5f, 0.0f, 1.0f, 0.0f,    // left bottom 	5
	// 	-0.5f,   0.0f, 0.0f, 0.0f, 1.0f,    // left 		6
	// };

	// GLfloat vertices[] = {     
	// 	 0.00000f, 	 0.00000f,	1.0f, 1.0f, 1.0f,    // center 		0
	// 	-0.00500f, 	 0.00866f, 	0.0f, 1.0f, 0.0f,    // left top 	1
	// 	 0.00500f, 	 0.00866f, 	0.0f, 0.5f, 0.5f,    // right top 	2
	// 	 0.01000f, 	 0.00000f, 	0.5f, 0.0f, 0.5f,    // right 		3
	// 	 0.00500f, 	-0.00866f, 	0.5f, 0.5f, 0.0f,    // right bottom 4
	// 	-0.00500f, 	-0.00866f, 	0.0f, 0.5f, 0.5f,    // left bottom 	5
	// 	-0.01000f, 	 0.00000f, 	0.5f, 0.0f, 0.5f,    // left 		6
	// };

	float a = 0.5f / 10.0f;
	float s = 0.57735026919f / 10.0f; // 1/sqrt(3)
	float h = s / 2.0f;

	// Rainbow:
	GLfloat vertices[] = {     
		 0.0f, 	 0.0f, 	 1.0f, 1.0f, 1.0f,    // center 		0
		-h, 	 a, 	 0.5f, 0.5f, 0.2f,    // left top 	1
		 h, 	 a, 	 0.2f, 0.5f, 0.5f,    // right top 	2
		 s, 	 0.0f, 	 0.5f, 0.2f, 0.5f,    // right 		3
		 h, 	-a, 	 0.5f, 0.5f, 0.2f,    // right bottom 4
		-h, 	-a, 	 0.2f, 0.5f, 0.5f,    // left bottom 	5
		-s, 	 0.0f, 	 0.5f, 0.2f, 0.5f,    // left 		6
	};

	// Solid color:
	// GLfloat vertices[] = {     
	// 	 0.0f, 	 0.0f, 	 0.8f, 0.8f, 0.8f,    // center 		0
	// 	-h, 	 a, 	 0.8f, 0.8f, 0.8f,    // left top 	1
	// 	 h, 	 a, 	 0.8f, 0.8f, 0.8f,    // right top 	2
	// 	 s, 	 0.0f, 	 0.8f, 0.8f, 0.8f,    // right 		3
	// 	 h, 	-a, 	 0.8f, 0.8f, 0.8f,    // right bottom 4
	// 	-h, 	-a, 	 0.8f, 0.8f, 0.8f,    // left bottom 	5
	// 	-s, 	 0.0f, 	 0.8f, 0.8f, 0.8f,    // left 		6
	// };

	// // Define elements array for a triangle:
	// GLuint elements[] = {
	// 	0, 1, 2
	// };

	// // Elements array for rectangle thingy:
	// GLuint elements[] = {
	// 	0, 1, 2,
	// 	2, 3, 0
	// };

	// Elements array for HEXAGON:
	GLushort elements[] = {
		// 0, 1, 2, 3, 4, 5, 6, 1
		0, 1, 2,
		2, 3, 0,
		0, 3, 4,
		4, 5, 0,
		0, 5, 6,
		6, 1, 0
	};

	// Create vertex array object to store links between attributes and buffer objects
	// and bind to make active:
	GLuint vao;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	// Create vertex buffer object, make active, and write data:
	GLuint vbo;
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// Create element buffer object and do all the same fun stuff:
	GLuint ebo;
	glGenBuffers(1, &ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(elements), elements, GL_STATIC_DRAW);

	// Create shaders:
	GLuint vertexShader = createShader(GL_VERTEX_SHADER, vertexSource);
	GLuint fragmentShader = createShader(GL_FRAGMENT_SHADER, fragmentSource);

	// // Create vertex shader, load source, and compile:
	// GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	// glShaderSource(vertexShader, 1, &vertexSource, NULL);
	// glCompileShader(vertexShader);

	// // Create fragment shader, load source, and compile:
	// GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	// glShaderSource(fragmentShader, 1, &fragmentSource, NULL);
	// glCompileShader(fragmentShader);

	// // [INCOMPLETE] If status is equal to GL_TRUE, shader was compiled successfully.
	// GLint status;
	// glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &status);
	// if (!status) {
	// 	// [INCOMPLETE] Retrieve the compile log:
	// 	char buffer[512];
	// 	glGetShaderInfoLog(vertexShader, 512, NULL, buffer);
	// 	fprintf(stdout, "No bueno: %s", buffer);
	// }
	
	// Create program:
	GLuint shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);

	// Specify output buffer:
	glBindFragDataLocation(shaderProgram, 0, "outColor");

	// Link and use program:
	glLinkProgram(shaderProgram);
	glUseProgram(shaderProgram);

	//////// NON-COLOR-PASSING VERSION
	// // Retrieve reference to 'position' input (from vertex shader):
	// GLint posAttrib = glGetAttribLocation(shaderProgram, "position");
	// // Specify 'position' vertex attribute input format:
	// glVertexAttribPointer(posAttrib, 2, GL_FLOAT, GL_FALSE, 0, 0);
	// // Enable 'position' vertex attribute:
	// glEnableVertexAttribArray(posAttrib);   

	GLint posAttrib = glGetAttribLocation(shaderProgram, "position");
	glEnableVertexAttribArray(posAttrib);
	glVertexAttribPointer(posAttrib, 2, GL_FLOAT, GL_FALSE, 5*sizeof(float), 0);

	GLint colAttrib = glGetAttribLocation(shaderProgram, "color");
	glEnableVertexAttribArray(colAttrib);
	glVertexAttribPointer(colAttrib, 3, GL_FLOAT, GL_FALSE, 5*sizeof(float), 
		(void*)(2*sizeof(float)));
	
	// Color triangle:
	GLint uniColor = glGetUniformLocation(shaderProgram, "triangleColor");
	glUniform3f(uniColor, 1.0f, 0.5f, 0.0f);

	// Transformations:
	glm::mat4 model;
	// model = glm::rotate(model, glm::radians(0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	// glm::vec4 result = model * glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);
	// printf("%f, %f, %f\n", result.x, result.y, result.z);
	GLint uniTrans = glGetUniformLocation(shaderProgram, "model");
	glUniformMatrix4fv(uniTrans, 1, GL_FALSE, glm::value_ptr(model));

	// View:
	glm::mat4 view = glm::lookAt(
		glm::vec3(1.5000f, 0.0000f, 4.0000f),
		glm::vec3(1.5000f, 0.0000f, 0.0000f),
		glm::vec3(0.0f, 1.0f, 0.0f)
	);
	GLint uniView = glGetUniformLocation(shaderProgram, "view");
	glUniformMatrix4fv(uniView, 1, GL_FALSE, glm::value_ptr(view));

	// Projection:
	glm::mat4 proj = glm::perspective(glm::radians(35.0f), 1280.0f / 800.0f, 0.0f, 400.0f);
	GLint uniProj = glGetUniformLocation(shaderProgram, "proj");
	glUniformMatrix4fv(uniProj, 1, GL_FALSE, glm::value_ptr(proj));


	uint dim_side = 256 << 2;
	float center_x = (float)dim_side * 0.1f;


	// Time variable:
	auto t_start = std::chrono::high_resolution_clock::now();

	// Rendering Loop
	while (glfwWindowShouldClose(mWindow) == false) {
		if (glfwGetKey(mWindow, GLFW_KEY_ESCAPE) == GLFW_PRESS
			|| glfwGetKey(mWindow, GLFW_KEY_X) == GLFW_PRESS)
		{
			glfwSetWindowShouldClose(mWindow, true);
		}

		// Background Fill Color
		glClearColor(0.25f, 0.25f, 0.25f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		// Get elapsed time:
		auto t_now = std::chrono::high_resolution_clock::now();
		float time = std::chrono::duration_cast<std::chrono::duration<float>>(t_now - t_start).count();

		// Spin:
		glm::mat4 model;
		model = glm::rotate(
		    model,
		    (time / 500.0f) * glm::radians(180.0f),
		    glm::vec3(center_x, 0.0f, 0.0f)
		);
		glUniformMatrix4fv(uniTrans, 1, GL_FALSE, glm::value_ptr(model));

		// Camera revolve:
		float radius = 4.0f;
		float time_scl = 2.0f;
		GLfloat camX = sin(time / time_scl / 5) * radius;
		GLfloat camY = cos(time / time_scl / 3) * radius;
		GLfloat camZ = cos(time / time_scl / 20.0f) * radius;
		glm::mat4 view = glm::lookAt(
			// glm::vec3(0.00001f, 0.0000f, 2.0f),
			// glm::vec3(camX, camY, 2.0f),
			glm::vec3(center_x + camX, 0.0000f + camY, 0.0000f + (camZ * 25.0f)),
			glm::vec3(center_x, 0.0000f, 0.0000f),
			glm::vec3(0.0f, 1.0f, 0.0f)
		);		
		glUniformMatrix4fv(uniView, 1, GL_FALSE, glm::value_ptr(view));


		// glUniform3f(uniColor, 
		// 	(sin(time * 4.0f) + 1.0f) / 2.0f, 
		// 	0.0f, 
		// 	(sin(time * 2.0f) + 1.0f) / 2.0f);


		// glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		glDrawElementsInstanced(GL_TRIANGLES, sizeof(elements), GL_UNSIGNED_SHORT, 0, sq(dim_side));

		// Flip Buffers and Draw
		glfwSwapBuffers(mWindow);
		glfwPollEvents();
	}  

	glfwTerminate();


	// Release all the things:
	glDeleteProgram(shaderProgram);
	glDeleteShader(fragmentShader);
	glDeleteShader(vertexShader);

	// glDeleteBuffers(1, &ebo);
	glDeleteBuffers(1, &vbo);

	glDeleteVertexArrays(1, &vao);

	return EXIT_SUCCESS;
}




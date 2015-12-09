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
#define GLSL(src) "#version 150 core\n" #src
#endif

GLFWwindow* win_ctx(float width, float height);
GLuint createShader(GLenum type, const GLchar* src);



int app_geo_shdr() {
	// Vertex shader
	const char* vertexShaderSrc = GLSL(
	    in vec2 pos;
	    in vec3 color;
	    in float sides;

	    out vec3 vColor; // Output to geometry (or fragment) shader
	    out float vSides;

	    void main() {
	        gl_Position = vec4(pos, 0.0, 1.0);
	        vColor = color;
	        vSides = sides;
	    }
	);
	
	// Geometry shader
	const char* geometryShaderSrc = GLSL(
		layout(points) in;
		layout(line_strip, max_vertices = 64) out;

		in float vSides[];
		in vec3 vColor[];
		out vec3 fColor;

		const float PI = 3.1415926;

		void main() {
		    fColor = vColor[0];

		    for (int i = 0; i <= vSides[0]; i++) {
		        // Angle between each side in radians
		        float ang = PI * 2.0 / vSides[0] * i;

		        // Offset from center of point (0.3 to accomodate for aspect ratio)
		        vec4 offset = vec4(cos(ang) * 0.3, -sin(ang) * 0.4, 0.0, 0.0);
		        gl_Position = gl_in[0].gl_Position + offset;

		        EmitVertex();
		    }

		    EndPrimitive();
		}
	);

	// Fragment shader
	const char* fragmentShaderSrc = GLSL(
		in vec3 fColor;

		out vec4 outColor;

		void main() {
		    outColor = vec4(fColor, 1.0);
		}
	);

	// Create context and window:
	auto mWindow = win_ctx(800.0f, 600.0f);
	
	// Vertices:
	float points[] = {
		//  Coordinates  Color             Sides
		-0.45f,  0.45f, 1.0f, 0.0f, 0.0f,  4.0f,
		 0.45f,  0.45f, 0.0f, 1.0f, 0.0f,  8.0f,
		 0.45f, -0.45f, 0.0f, 0.0f, 1.0f, 16.0f,
		-0.45f, -0.45f, 1.0f, 1.0f, 0.0f, 32.0f
	};

	// Create VAO:
	GLuint vao;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	// Create VBO and make active:
	GLuint vbo;
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(points), points, GL_STATIC_DRAW);

	// Create shaders:
	GLuint vertexShader = createShader(GL_VERTEX_SHADER, vertexShaderSrc);
	GLuint fragmentShader = createShader(GL_FRAGMENT_SHADER, fragmentShaderSrc);
	GLuint geometryShader = createShader(GL_GEOMETRY_SHADER, geometryShaderSrc);

	// Create program:
	GLuint shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glAttachShader(shaderProgram, geometryShader);
	glLinkProgram(shaderProgram);
	glUseProgram(shaderProgram);

	// Specify layout of point data:
	GLint posAttrib = glGetAttribLocation(shaderProgram, "pos");
	glEnableVertexAttribArray(posAttrib);
	glVertexAttribPointer(posAttrib, 2, GL_FLOAT, GL_FALSE, 6 * sizeof(float), 0);

	GLint colAttrib = glGetAttribLocation(shaderProgram, "color");
	glEnableVertexAttribArray(colAttrib);
	glVertexAttribPointer(colAttrib, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), 
		(void*) (2 * sizeof(float)));

	GLint sidesAttrib = glGetAttribLocation(shaderProgram, "sides");
	glEnableVertexAttribArray(sidesAttrib);
	glVertexAttribPointer(sidesAttrib, 1, GL_FLOAT, GL_FALSE, 6 * sizeof(float), 
		(void*) (5 * sizeof(float)));

	// // Projection:
	// glm::mat4 proj = glm::perspective(glm::radians(15.0f), 1200.0f / 800.0f, 1.0f, 10.0f);
	// GLint uniProj = glGetUniformLocation(shaderProgram, "proj");
	// glUniformMatrix4fv(uniProj, 1, GL_FALSE, glm::value_ptr(proj));

	// Event loop:
	while (glfwWindowShouldClose(mWindow) == false) {
		if (glfwGetKey(mWindow, GLFW_KEY_ESCAPE) == GLFW_PRESS
			|| glfwGetKey(mWindow, GLFW_KEY_X) == GLFW_PRESS)
		{
			glfwSetWindowShouldClose(mWindow, true);
		}

		// Background Fill Color
		glClearColor(0.25f, 0.25f, 0.25f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		glDrawArrays(GL_POINTS, 0, 4);

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

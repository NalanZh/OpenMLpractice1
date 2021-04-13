#include <GL\glew.h>
#include <GLFW\glfw3.h>
#include <string>
#include <iostream>
#include <fstream>
#include <cmath>
#include <glm\glm.hpp>
#include <glm\gtc\type_ptr.hpp> // glm::value_ptr
#include <glm\gtc\matrix_transform.hpp> // glm::translate, glm::rotate, glm::scale, glm::perspective
#include "Utils.h"
using namespace std;

#define numVAOs 1
#define numVBOs 1

float cameraX, cameraY, cameraZ;
glm::vec4 cameraVX, cameraVY, cameraVZ; // adding new global variable
GLuint renderingProgram;
GLuint vao[numVAOs];
GLuint vbo[numVBOs];

// variable allocation for display
GLuint mvLoc, projLoc;
int width, height, displayLoopi;
float aspect;
float tf;
glm::mat4 pMat, vMat, tMat, rMat, mMat, mvMat;
void TiltUp();
void TiltDown();
void TiltRight();
void TiltLeft();



void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods){
	if (key == GLFW_KEY_LEFT && action == GLFW_PRESS)
	{
		TiltLeft();
	}
	else if (key == GLFW_KEY_RIGHT && action == GLFW_PRESS)
	{
		TiltRight();
	}
	else if (key == GLFW_KEY_UP && action == GLFW_PRESS)
	{
		TiltUp();
	}
	else if (key == GLFW_KEY_DOWN && action == GLFW_PRESS)
	{
		TiltDown();
	}
	else if (key == GLFW_KEY_A && action == GLFW_PRESS)
	{
		cameraX += 10.0f;
	}
	else if (key == GLFW_KEY_W && action == GLFW_PRESS)
	{
		cameraZ += 10.0f;
	}
	else if (key == GLFW_KEY_S && action == GLFW_PRESS)
	{
		cameraZ -= 10.0f;
	}
	else if (key == GLFW_KEY_D && action == GLFW_PRESS)
	{
		cameraX -= 10.0f;
	}
	}


void TiltUp() {
	glm::mat4 TiltUp;
	TiltUp = glm::rotate(glm::mat4(1.0f), glm::radians(10.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	cameraVX = TiltUp * cameraVX;
	cameraVY = TiltUp * cameraVY;
	cameraVZ = TiltUp * cameraVZ;
}
void TiltDown() {
	glm::mat4 TiptDown;
	TiptDown = glm::rotate(glm::mat4(1.0f), glm::radians(10.0f), glm::vec3(-1.0f, 0.0f, 0.0f));
	cameraVX = TiptDown * cameraVX;
	cameraVY = TiptDown * cameraVY;
	cameraVZ = TiptDown * cameraVZ;
}
void TiltLeft() {
	glm::mat4 TiltLeft;
	TiltLeft = glm::rotate(glm::mat4(1.0f), glm::radians(10.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	cameraVX = TiltLeft * cameraVX;
	cameraVY = TiltLeft * cameraVY;
	cameraVZ = TiltLeft * cameraVZ;
}
void TiltRight() {
	glm::mat4 TiltRight;
	TiltRight = glm::rotate(glm::mat4(1.0f), glm::radians(10.0f), glm::vec3(0.0f, -1.0f, 0.0f));
	cameraVX = TiltRight * cameraVX;
	cameraVY = TiltRight * cameraVY;
	cameraVZ = TiltRight * cameraVZ;
}

void setupVertices(void)
{
	float vertexPositions[108] =
	{ -1.0f,  1.0f, -1.0f, -1.0f, -1.0f, -1.0f, 1.0f, -1.0f, -1.0f,
		1.0f, -1.0f, -1.0f, 1.0f,  1.0f, -1.0f, -1.0f,  1.0f, -1.0f,
		1.0f, -1.0f, -1.0f, 1.0f, -1.0f,  1.0f, 1.0f,  1.0f, -1.0f,
		1.0f, -1.0f,  1.0f, 1.0f,  1.0f,  1.0f, 1.0f,  1.0f, -1.0f,
		1.0f, -1.0f,  1.0f, -1.0f, -1.0f,  1.0f, 1.0f,  1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f, -1.0f,  1.0f,  1.0f, 1.0f,  1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f, -1.0f, -1.0f, -1.0f, -1.0f,  1.0f,  1.0f,
		-1.0f, -1.0f, -1.0f, -1.0f,  1.0f, -1.0f, -1.0f,  1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f,  1.0f, -1.0f,  1.0f,  1.0f, -1.0f, -1.0f,
		1.0f, -1.0f, -1.0f, -1.0f, -1.0f, -1.0f, -1.0f, -1.0f,  1.0f,
		-1.0f,  1.0f, -1.0f, 1.0f,  1.0f, -1.0f, 1.0f,  1.0f,  1.0f,
		1.0f,  1.0f,  1.0f, -1.0f,  1.0f,  1.0f, -1.0f,  1.0f, -1.0f
	};

	glGenVertexArrays(1, vao);
	glBindVertexArray(vao[0]);
	glGenBuffers(numVBOs, vbo);

	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertexPositions), vertexPositions, GL_STATIC_DRAW);
}

void init(GLFWwindow* window) {
	renderingProgram = Utils::createShaderProgram("vertShader.glsl", "fragShader.glsl");
	cameraVX = glm::vec4(1.0, 0.0, 0.0, 0.0); // initialized for moving
	cameraVY = glm::vec4(0.0, 1.0, 0.0, 0.0);
	cameraVZ = glm::vec4(0.0, 0.0, 1.0, 0.0);
	glfwSetKeyCallback(window, key_callback); // set key callback

	glfwGetFramebufferSize(window, &width, &height);
	aspect = (float)width / (float)height;
	pMat = glm::perspective(1.0472f, aspect, 0.1f, 1000.0f);

	cameraX = 0.0f; cameraY = 0.0f; cameraZ = 32.0f;
	setupVertices();
}

void display(GLFWwindow* window, double currentTime) {
	glClear(GL_DEPTH_BUFFER_BIT);
	glClearColor(0.0, 0.0, 0.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT);

	glUseProgram(renderingProgram);

	mvLoc = glGetUniformLocation(renderingProgram, "mv_matrix");
	projLoc = glGetUniformLocation(renderingProgram, "proj_matrix");

	//vMat = glm::translate(glm::mat4(1.0f), glm::vec3(-cameraX, -cameraY, -cameraZ));
	vMat = glm::inverse(glm::mat4(cameraVX, cameraVY, cameraVZ, glm::vec4(cameraX, cameraY, cameraZ, 1.0)));

	glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(pMat));

	for (displayLoopi = 0; displayLoopi<24; displayLoopi++) {
		tf = currentTime + displayLoopi;
		tMat = glm::translate(glm::mat4(1.0f), glm::vec3(sin(.35f*tf)*8.0f, cos(.52f*tf)*8.0f, sin(.70f*tf)*8.0f));
		rMat = glm::rotate(glm::mat4(1.0f), 1.75f*tf, glm::vec3(0.0f, 1.0f, 0.0f));
		rMat = glm::rotate(rMat, 1.75f*tf, glm::vec3(1.0f, 0.0f, 0.0f));
		rMat = glm::rotate(rMat, 1.75f*tf, glm::vec3(0.0f, 0.0f, 1.0f));
		mMat = tMat * rMat;
		mvMat = vMat * mMat;

		glUniformMatrix4fv(mvLoc, 1, GL_FALSE, glm::value_ptr(mvMat));

		glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
		glVertexAttribPointer(0, 3, GL_FLOAT, false, 0, 0);
		glEnableVertexAttribArray(0);

		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LEQUAL);

		glDrawArrays(GL_TRIANGLES, 0, 36);
	}
}

void window_size_callback(GLFWwindow* win, int newWidth, int newHeight) {
	aspect = (float)newWidth / (float)newHeight;
	glViewport(0, 0, newWidth, newHeight);
	pMat = glm::perspective(1.0472f, aspect, 0.1f, 1000.0f);
}

int main(void) {
	if (!glfwInit()) { exit(EXIT_FAILURE); }
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	GLFWwindow* window = glfwCreateWindow(600, 600, "Chapter 4 - program 1d", NULL, NULL);
	glfwMakeContextCurrent(window);
	if (glewInit() != GLEW_OK) { exit(EXIT_FAILURE); }
	glfwSwapInterval(1);

	glfwSetWindowSizeCallback(window, window_size_callback);

	init(window);

	while (!glfwWindowShouldClose(window)) {
		display(window, glfwGetTime());
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwDestroyWindow(window);
	glfwTerminate();
	exit(EXIT_SUCCESS);
}


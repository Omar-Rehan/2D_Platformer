#include <iostream>
#include <GLAD/glad.h>
#include <GLFW/glfw3.h>
#include <GLM/glm.hpp>
#include <GLM/gtc/matrix_transform.hpp>

#include "Shaders.h"
#include "Player.h"
#include "Ground.h"
#include "Platform.h"

// OpenGL context
GLFWwindow *window;

// Time variables
float lastFrame = 0.0f;
float deltaTime = 0.0f;

// Settings
unsigned int SCR_WIDTH  = 800;
unsigned int SCR_HEIGHT = 800;

// Initialization functions
void initGLAD();
void initGLFW();

// Callback functions
void processKeyboardInput();
void framebuffer_size_callback(GLFWwindow *window, int width, int height);


// Player
Player player;
const float circleRadius = 0.07f;
const int numOfCircleVertices = 20;
float circleVertices[(numOfCircleVertices + 1) * 3];
void calculatePlayerData();


// Ground
Ground ground;
float groundVertices[4 * 3]; // 4 points, 3 coordinates each
unsigned int indices[6];
void calculateGroundData();



// Platforms
const int numOfPlatforms = 4;
Platform platforms[numOfPlatforms];
float platformsVertices[4 * 3]; // 4 points, 3 coordinates each
glm::vec3 platformsPositions[numOfPlatforms];
void calculatePlatformsData();



int main () {
	initGLFW(); // Create opengl context
	if (window == NULL) return -1;

	initGLAD(); // Get function pointers from GPU drivers

	
	calculatePlayerData();
	player.Setup(numOfCircleVertices, circleVertices, "circleShader.vs", "circleShader.fs", circleRadius);

	calculateGroundData();
	ground.Setup(groundVertices, indices, "groundShader.vs", "groundShader.fs");

	calculatePlatformsData();
	for (int i = 0; i < numOfPlatforms; i++)
		platforms[i].Setup(platformsVertices, indices, "platformsShader.vs", "platformsShader.fs");


	while (!glfwWindowShouldClose(window)) {
		// Update time variables
		float crntFrame = (float)glfwGetTime();
		deltaTime = crntFrame - lastFrame;
		lastFrame = crntFrame;

		// Render background
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		// Render objects
		ground.Draw();
		player.Draw(deltaTime, numOfPlatforms, platforms);
		for (int i = 0; i < numOfPlatforms; i++) 
			platforms[i].Draw(platformsPositions[i]);

		processKeyboardInput();
		glfwSwapBuffers(window); // swap the two buffers (front & back)
		glfwPollEvents(); // check for triggered events, update window state, call callback functions
	}

	player.DeleteVAO();
	ground.DeleteVAO();
	glfwTerminate();
	return 0;
}


// User-defined Functions:

void initGLFW() {
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); // version
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3); // version
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // mode

	// Create a window using GLFW
	window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
	if (window == NULL) {
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		std::cin.get();
		return;
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback); // set callback function for changing window size
}

void initGLAD() {
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
		std::cout << "Failed to initialize GLAD" << std::endl;
}


void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
	// make sure the viewport matches the new window dimensions; note that width and 
	// height will be significantly larger than specified on retina displays.
	SCR_WIDTH = width;
	SCR_HEIGHT = height;
	glViewport(0, 0, width, height);
}

void processKeyboardInput() {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true); // close window when esc is pressed

	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) // jump
		player.Move(UP, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) // move right
		player.Move(RIGHT, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) // move left
		player.Move(LEFT, deltaTime);
}


void calculatePlayerData() {
	float deg = 0.0f;

	// points on circumference
	for (int idx = 0; idx < 3 * numOfCircleVertices; idx += 3, deg += 360.0f / numOfCircleVertices) {
		// x coordinate
		circleVertices[idx + 0] = (circleRadius * (float)std::cos(glm::radians(deg)));
		// y coordinate
		circleVertices[idx + 1] = (circleRadius * (float)std::sin(glm::radians(deg)));
		// z coordinate
		circleVertices[idx + 2] = 0.0f;
	}

	// center of circle
	circleVertices[3 * numOfCircleVertices + 0] = 0.0f;
	circleVertices[3 * numOfCircleVertices + 1] = 0.0f;
	circleVertices[3 * numOfCircleVertices + 2] = 0.0f;
}


void calculateGroundData() {
	// Position data:
	// lower left
	groundVertices[0] = -1.0f;
	groundVertices[1] = -1.0f;
	groundVertices[2] =  0.0f;
	// lower right
	groundVertices[3] =  1.0f;
	groundVertices[4] = -1.0f;
	groundVertices[5] =  0.0f;
	// upper right
	groundVertices[6] =  1.0f;
	groundVertices[7] = -0.8f;
	groundVertices[8] =  0.0f;
	// upper left
	groundVertices[9] = -1.0f;
	groundVertices[10] = -0.8f;
	groundVertices[11] =  0.0f;


	// EBO data:
	// first triangle
	indices[0] = 0;
	indices[1] = 1;
	indices[2] = 3;
	// second triangle
	indices[3] = 1;
	indices[4] = 2;
	indices[5] = 3;
}



void calculatePlatformsData() {
	// Position data:
	// first vertex
	platformsVertices[0] = -0.25f;
	platformsVertices[1] =  -0.08f;
	platformsVertices[2] =   0.0f;
    // second vertex
	platformsVertices[3] = 0.25f;
	platformsVertices[4] = -0.08f;
	platformsVertices[5] =  0.0f;
	// third vertex
	platformsVertices[6] = 0.25f;
	platformsVertices[7] =  0.08f;
	platformsVertices[8] =  0.0f;
	// fourth vertex
	platformsVertices[9] = -0.25f;
	platformsVertices[10] =  0.08f;
	platformsVertices[11] =  0.0f;


	// translation vectors
	platformsPositions[0] = glm::vec3(0.7f, -0.5f, 0.0f);
	platformsPositions[1] = glm::vec3(0.0f, -0.2f, 0.0f);
	platformsPositions[2] = glm::vec3(-0.7f, 0.1f, 0.0f);
	platformsPositions[3] = glm::vec3(0.0f, 0.45f, 0.0f);
}


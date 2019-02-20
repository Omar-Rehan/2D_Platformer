#include <string>
#include <vector>

#include <GLAD/glad.h>
#include <GLM/glm.hpp>
#include <GLFW/glfw3.h>

#include "Text.h"
#include "Player.h"
#include "Ground.h"
#include "Platform.h"
#include "ShaderProgram.h"

// OpenGL context
GLFWwindow *window;

// Time variables
float lastFrame = 0.0f;
float deltaTime = 0.0f;

// Settings
unsigned int SCR_WIDTH  = 700;
unsigned int SCR_HEIGHT = 700;

// Functions
void InitGLAD();
void InitGLFW();
std::string FormatTime(int timeNow);
void ProcessKeyboardInput(Player &player);


// Player
Player player;
const float circleRadius = 0.07f;
const int numOfCircleVertices = 20;
float circleVertices[(numOfCircleVertices + 1) * 3];
void CalculatePlayerData();


// Ground
unsigned int rectangleIndices[6];
float groundVertices[4 * 3]; // 4 points, 3 coordinates each
void CalculateGroundData();


// Platforms
const int numOfPlatforms = 4;
float platformsVertices[4 * 3]; // 4 points, 3 coordinates each
glm::vec3 platformsPositions[numOfPlatforms];
void CalculatePlatformsData();



int main () {
	InitGLFW(); // Create opengl context
	if (window == NULL) return -1;

	InitGLAD(); // Get function pointers from GPU drivers

	
	CalculatePlayerData();
	player.Setup(numOfCircleVertices, circleVertices, "Shaders/circleShader.vs", "Shaders/circleShader.fs", circleRadius);

	CalculateGroundData();
	Ground ground(groundVertices, rectangleIndices, "Shaders/groundShader.vs", "Shaders/groundShader.fs");

	CalculatePlatformsData();
	std::vector<Platform> platforms;
	for (int i = 0; i < numOfPlatforms; i++) 
		platforms.push_back(Platform(platformsVertices, rectangleIndices, "Shaders/platformsShader.vs", "Shaders/platformsShader.fs"));


	Text timerText(0, 36, "Shaders/fontShader.vs", "Shaders/fontShader.fs", SCR_WIDTH, SCR_HEIGHT);


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
		for (int i = 0; i < numOfPlatforms; i++) platforms[i].Draw(platformsPositions[i]);
		player.Draw(deltaTime, numOfPlatforms, platforms);

		int timeNow = (int)round(glfwGetTime());
		timerText.RenderText(FormatTime(timeNow), 550.0f, 650.0f, 1.0f, glm::vec3(1.0f, 1.0f, 1.0f));
		
		ProcessKeyboardInput(player);
		glfwSwapBuffers(window); // swap the two buffers (front & back)
		glfwPollEvents(); // check for triggered events, update window state, call callback functions
	}

	player.DeleteVAO();
	ground.DeleteVAO();
	glfwTerminate();
	return 0;
}


// User-defined Functions:

void InitGLFW() {
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); // version
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3); // version
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // mode

	// Disable changing window size
	glfwWindowHint(GLFW_RESIZABLE, false);
	// Create a window using GLFW
	window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "2D Platformer", NULL, NULL);
	if (window == NULL) {
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		std::cin.get();
		return;
	}
	glfwMakeContextCurrent(window);
}

void InitGLAD() {
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
		std::cout << "Failed to initialize GLAD" << std::endl;
}

void ProcessKeyboardInput(Player &player) {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true); // close window when esc is pressed

	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) // jump
		player.Move(UP, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) // move right
		player.Move(RIGHT, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) // move left
		player.Move(LEFT, deltaTime);


	glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) ? player.GetHyper() : player.BeNormal();
}


void CalculatePlayerData() {
	float deg = 0.0f;

	// points on circumference
	for (int idx = 0; idx < 3 * numOfCircleVertices; idx += 3, deg += 360.0f / numOfCircleVertices) {
		circleVertices[idx + 0] = (circleRadius * (float)std::cos(glm::radians(deg))); // x coordinate
		circleVertices[idx + 1] = (circleRadius * (float)std::sin(glm::radians(deg))); // y coordinate
		circleVertices[idx + 2] = 0.0f; // z coordinate
	}

	// center of circle
	circleVertices[3 * numOfCircleVertices + 0] = 0.0f;
	circleVertices[3 * numOfCircleVertices + 1] = 0.0f;
	circleVertices[3 * numOfCircleVertices + 2] = 0.0f;
}


void CalculateGroundData() {
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


	// Indices data (EBO):
	// first triangle
	rectangleIndices[0] = 0;
	rectangleIndices[1] = 1;
	rectangleIndices[2] = 3;
	// second triangle
	rectangleIndices[3] = 1;
	rectangleIndices[4] = 2;
	rectangleIndices[5] = 3;
}



void CalculatePlatformsData() {
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
	platformsPositions[0] = glm::vec3(0.75f, -0.5f, 0.0f);
	platformsPositions[1] = glm::vec3(0.0f, -0.25f, 0.0f);
	platformsPositions[2] = glm::vec3(-0.75f, 0.175f, 0.0f);
	platformsPositions[3] = glm::vec3(0.0f, 0.45f, 0.0f);
}


std::string FormatTime(int seconds) {
	int hours = seconds / 3600; seconds %= 3600;
	int minutes = seconds / 60; seconds %= 60;

	std::string time;
	time += (hours < 10 ? "0" : "") + std::to_string(hours) + ":";
	time += (minutes < 10 ? "0" : "") + std::to_string(minutes) + ":";
	time += (seconds < 10 ? "0" : "") + std::to_string(seconds);
	return time;
}
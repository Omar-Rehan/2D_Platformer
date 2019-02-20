#pragma once

#include "ShaderProgram.h"
#include "Platform.h"


enum Player_Movement {UP, RIGHT, LEFT};


class Player {
public:
	// Functions
	void Setup(int vertices, float positionAttribute[], const char* vrtxShaderPath, const char* frgmtShaderPath, float radius);
	void Draw(float deltaTime, int numOfPlatforms, std::vector<Platform> &platforms);
	void Move(Player_Movement key, float deltaTime);
	void GetCrazy();
	void BeNormal();
	void DeleteVAO();


private:
	unsigned int vaoId;
	ShaderProgram shaderProgram;

	int numOfVertices;
	float circleRadius;
	glm::vec3 playerPosition;

	// Movement variables
	float timer = 0.0f;
	bool tired = false;
	float speedup = 0.0f;
	float hyperTime = 2.0f;
	float cooldownTime = 3.0f;

	float velocityY = 0.0f;
	const float velocityX = 0.5f;

	const float gravity = 0.002f;
	const float kickoff = 0.00112f; // initial velocity when jumping

	float lowestPoint, highestPoint, rightmostPosition, leftmostPosition;

	// Ground variables
	bool onGround = false;
	const float groundUpperline = -0.8f;

	// collision varaibles
	float closestX, closestY, distanceX, distanceY;


	// Functions
	bool DetectCollision(Platform platform);
	void Collide(Platform platform);
};
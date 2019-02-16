#ifndef PLAYER_H
#define PLAYER_H

#include <GLAD/glad.h>
#include <GLM/glm.hpp>
#include <iostream>

#include "Shaders.h"
#include "Platform.h"


enum Player_Movement {UP, RIGHT, LEFT};

class Player {
public:
	
	unsigned int vaoId;
	Shaders shaderProgram = Shaders();

	int numOfVertices;
	float circleRadius;
	glm::vec3 playerPosition;

	// Movement variables
	float velocityY = 0.0f;
	const float velocityX = 0.5f;
	const float gravity = 0.002f;
	const float kickoff = 0.00111f;
	float lowestPoint, highestPoint;
	float rightmostPosition, leftmostPosition;

	// Ground variables
	bool onGround = false;
	const float groundUpperline = -0.8f;


	void Setup(int vertices, float positionAttribute[], const char* vrtxShaderPath, const char* frgmtShaderPath, float radius) {
		circleRadius = radius;
		numOfVertices = vertices;
		playerPosition.y = -0.4f;
		highestPoint = 1.0f - circleRadius;
		lowestPoint = groundUpperline + circleRadius;


		// Create vertex array object
		glGenVertexArrays(1, &vaoId);
		glBindVertexArray(vaoId);

		// Create vertex buffer object
		unsigned int positionVBO;
		glGenBuffers(1, &positionVBO);
		glBindBuffer(GL_ARRAY_BUFFER, positionVBO);
		glBufferData(GL_ARRAY_BUFFER, numOfVertices * 3 * sizeof(float), positionAttribute, GL_DYNAMIC_DRAW); // put data in buffer
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)(0 * sizeof(float))); // specify data layout
		glEnableVertexAttribArray(0);

		// Assign shaders to shader program
		shaderProgram.Setup(vrtxShaderPath, frgmtShaderPath);

		// Unbind VAO
		glBindVertexArray(0);
	}

	
	void Draw(float deltaTime, int numOfPlatforms, Platform platforms[]) {
		glBindVertexArray(vaoId);
		shaderProgram.activate();


		playerPosition.x = glm::max(-1.0f + circleRadius, glm::min(1.0f - circleRadius, playerPosition.x));
		playerPosition.y = glm::min(highestPoint, glm::max(playerPosition.y + velocityY, lowestPoint));

		onGround = false;
		if (playerPosition.y == lowestPoint) onGround = true;

		
		// Create model matrix (local space -> world space)
		glm::mat4 modelMat = glm::mat4(1.0f);
		modelMat = glm::translate(modelMat, playerPosition);
		shaderProgram.setMat4Uniform("modelMat", modelMat);

		for (int i = 0; i < numOfPlatforms; i++) {
			currentPlatform = platforms[i];
			if (DetectCollision()) Collide();
		}

		glDrawArrays(GL_TRIANGLE_FAN, 0, numOfVertices);

		if (onGround) 
			velocityY = 0.0f;
		else if (playerPosition.y == highestPoint) 
			velocityY = glm::min(0.0f, velocityY - gravity * deltaTime);
		else 
			velocityY -= gravity * deltaTime;

		shaderProgram.deactivate();
		glBindVertexArray(0);
	}
	

	void Move(Player_Movement key, float deltaTime) {
		if (key == UP && onGround) velocityY = kickoff; // jump

		if (key == RIGHT) playerPosition.x += velocityX * deltaTime; // move right
		if (key == LEFT) playerPosition.x -= velocityX * deltaTime; // move left
	}


	void DeleteVAO() {
		glDeleteVertexArrays(1, &vaoId);
		glDeleteBuffers(1, &vaoId);
	}


private:
	Platform currentPlatform;
	float closestX, closestY, distanceX, distanceY;

	bool DetectCollision() {
		// calculate the closest point on platform to circle
		closestX = glm::max(currentPlatform.leftSide,  glm::min(playerPosition.x, currentPlatform.rightSide));
		closestY = glm::max(currentPlatform.lowerSide, glm::min(playerPosition.y, currentPlatform.upperSide));

		// calculate square distance between point and circle center
		distanceX = playerPosition.x - closestX;
		distanceY = playerPosition.y - closestY;
		float distanceSquare = (distanceX * distanceX) + (distanceY * distanceY);


		// if circle collides with platform
		return (distanceSquare < (circleRadius * circleRadius));
	}


	void Collide() {

		// circle equation: (x - centerX) ^ 2	+ (y - centerY) ^ 2 = (radius) ^ 2
		// application    :    firstTerm        +     secondTerm    = radiusSquare
		float radiusSquare = circleRadius * circleRadius;

		if (closestY == currentPlatform.upperSide && glm::abs(distanceX) < (0.5f * circleRadius)) { // circle is above platform
			//velocityY = glm::min(velocityY, 0.0f);

			if (closestX == currentPlatform.leftSide) { // upper left corner
				float firstTerm = (playerPosition.x - currentPlatform.leftSide) * (playerPosition.x - currentPlatform.leftSide);
				playerPosition.y = glm::sqrt(radiusSquare - firstTerm) + currentPlatform.upperSide;

			} else if (closestX == currentPlatform.rightSide) { // upper right corner
				float firstTerm = (playerPosition.x - currentPlatform.rightSide) * (playerPosition.x - currentPlatform.rightSide);
				playerPosition.y = glm::sqrt(radiusSquare - firstTerm) + currentPlatform.upperSide;

			} else {
				playerPosition.y = glm::max(playerPosition.y, currentPlatform.upperSide + circleRadius);
			}

			onGround = true;


		} else if (closestY == currentPlatform.lowerSide) { // circle is below platform
			velocityY = glm::min(velocityY, 0.0f);

			if (closestX == currentPlatform.leftSide) { // lower left corner
				float firstTerm = (playerPosition.x - currentPlatform.leftSide) * (playerPosition.x - currentPlatform.leftSide);
				playerPosition.y = -glm::sqrt(radiusSquare - firstTerm) + currentPlatform.lowerSide;

			} else if (closestX == currentPlatform.rightSide) { // lower right corner
				float firstTerm = (playerPosition.x - currentPlatform.rightSide) * (playerPosition.x - currentPlatform.rightSide);
				playerPosition.y = -glm::sqrt(radiusSquare - firstTerm) + currentPlatform.lowerSide;

			} else {
				playerPosition.y = glm::min(playerPosition.y, currentPlatform.lowerSide - circleRadius);
			}


		} else if (closestX == currentPlatform.leftSide) { // circle is left of platform
			playerPosition.x = currentPlatform.leftSide - circleRadius;

		} else if (closestX == currentPlatform.rightSide) { // circle is right of platform
			playerPosition.x = currentPlatform.rightSide + circleRadius;

		}
	}
};

#endif
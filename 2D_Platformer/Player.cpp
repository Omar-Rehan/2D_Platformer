#include <GLAD/glad.h>
#include <GLM/glm.hpp>
#include <GLM/gtc/matrix_transform.hpp>
#include <vector>

#include "Player.h"
#include "ShaderProgram.h"


// Public Functions:

void Player::Setup(int vertices, float positionAttribute[], const char * vrtxShaderPath, const char * frgmtShaderPath, float radius) {
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

	// Unbind VAO & VBO
	glBindVertexArray(0);
}

void Player::Draw(float deltaTime, int numOfPlatforms, std::vector<Platform>& platforms) {
	glBindVertexArray(vaoId);
	shaderProgram.activate();

	// Make sure player doesn't get out of screen
	playerPosition.x = glm::max(-1.0f + circleRadius, glm::min(1.0f - circleRadius, playerPosition.x));
	playerPosition.y = glm::min(highestPoint, glm::max(playerPosition.y + velocityY, lowestPoint));

	// Check if player is on ground
	onGround = false;
	if (playerPosition.y == lowestPoint) onGround = true;


	// Create model matrix (local space -> world space)
	glm::mat4 modelMat = glm::mat4(1.0f);
	modelMat = glm::translate(modelMat, playerPosition);
	shaderProgram.setMat4Uniform("modelMat", modelMat);

	// Check if collisions occur
	for (int i = 0; i < numOfPlatforms; i++)
		if (DetectCollision(platforms[i])) Collide(platforms[i]);

	// Draw player
	glDrawArrays(GL_TRIANGLE_FAN, 0, numOfVertices);
	
	// Calculate new "velocityY" value
	if (onGround)                              velocityY = 0.0f;
	else if (playerPosition.y == highestPoint) velocityY = glm::min(0.0f, velocityY - gravity * deltaTime);
	else                                       velocityY -= gravity * deltaTime;

	shaderProgram.deactivate();
	glBindVertexArray(0);
}


void Player::Move(Player_Movement key, float deltaTime) {
	if (key == UP && onGround) velocityY = kickoff; // jump

	if (key == RIGHT) playerPosition.x += velocityX * deltaTime; // move right
	if (key == LEFT) playerPosition.x -= velocityX * deltaTime; // move left
}


void Player::DeleteVAO() {
	glDeleteVertexArrays(1, &vaoId);
	glDeleteBuffers(1, &vaoId);
}




// Private Functions:

bool Player::DetectCollision(Platform platform) {
	// calculate the closest point on platform to circle
	closestX = glm::max(platform.leftSide,  glm::min(playerPosition.x, platform.rightSide));
	closestY = glm::max(platform.lowerSide, glm::min(playerPosition.y, platform.upperSide));

	// calculate square distance between point and circle center
	distanceX = playerPosition.x - closestX;
	distanceY = playerPosition.y - closestY;
	float distanceSquare = (distanceX * distanceX) + (distanceY * distanceY);


	// if circle collides with platform
	return (distanceSquare < (circleRadius * circleRadius));
}


void Player::Collide(Platform platform) {
	// circle equation: (x - centerX) ^ 2	+ (y - centerY) ^ 2 = (radius) ^ 2
	// application    :    firstTerm        +     secondTerm    = radiusSquare
	float radiusSquare = circleRadius * circleRadius;

	if (closestY == platform.upperSide && glm::abs(distanceX) < (0.5f * circleRadius)) { // circle is above platform

		if (closestX == platform.leftSide) { // upper left corner
			float firstTerm = (playerPosition.x - platform.leftSide) * (playerPosition.x - platform.leftSide);
			playerPosition.y = glm::sqrt(radiusSquare - firstTerm) + platform.upperSide;

		} else if (closestX == platform.rightSide) { // upper right corner
			float firstTerm = (playerPosition.x - platform.rightSide) * (playerPosition.x - platform.rightSide);
			playerPosition.y = glm::sqrt(radiusSquare - firstTerm) + platform.upperSide;

		} else {
			playerPosition.y = glm::max(playerPosition.y, platform.upperSide + circleRadius);
		}

		onGround = true;


	} else if (closestY == platform.lowerSide) { // circle is below platform
		velocityY = glm::min(velocityY, 0.0f);

		if (closestX == platform.leftSide) { // lower left corner
			float firstTerm = (playerPosition.x - platform.leftSide) * (playerPosition.x - platform.leftSide);
			playerPosition.y = -glm::sqrt(radiusSquare - firstTerm) + platform.lowerSide;

		} else if (closestX == platform.rightSide) { // lower right corner
			float firstTerm = (playerPosition.x - platform.rightSide) * (playerPosition.x - platform.rightSide);
			playerPosition.y = -glm::sqrt(radiusSquare - firstTerm) + platform.lowerSide;

		} else {
			playerPosition.y = glm::min(playerPosition.y, platform.lowerSide - circleRadius);
		}


	} else if (closestX == platform.leftSide) { // circle is left of platform
		playerPosition.x = platform.leftSide - circleRadius;

	} else if (closestX == platform.rightSide) { // circle is right of platform
		playerPosition.x = platform.rightSide + circleRadius;

	}
}
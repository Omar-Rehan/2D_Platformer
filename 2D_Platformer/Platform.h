#ifndef PLATFORM_H
#define PLATFORM_H

#include <GLAD/glad.h>
#include <GLM/glm.hpp>
#include <iostream>

#include "Shaders.h"
#include "Ground.h"

class Platform : public Ground {
public:
	int numOfPlatforms;
	float originalColliderUpperSide, originalColliderLowerSide, originalColliderRightSide, originalColliderLeftSide;

	void Setup(float positionAttribute[], unsigned int positionIndices[], const char* vrtxShaderPath, const char* frgmtShaderPath) {
		// call parent version
		Ground::Setup(positionAttribute, positionIndices, vrtxShaderPath, frgmtShaderPath);
		
		originalColliderUpperSide = upperSide;
		originalColliderLowerSide = lowerSide;
		originalColliderRightSide = rightSide;
		originalColliderLeftSide  = leftSide;

	}

	void Draw(glm::vec3 translationVector) {
		glBindVertexArray(vaoId);
		shaderProgram.activate();

		upperSide = originalColliderUpperSide + translationVector.y;
		lowerSide = originalColliderLowerSide + translationVector.y;
		rightSide = originalColliderRightSide + translationVector.x;
		leftSide  = originalColliderLeftSide  + translationVector.x;


		glm::mat4 modelMat = glm::mat4(1.0f); // local -> world
		modelMat = glm::translate(modelMat, translationVector);
		shaderProgram.setMat4Uniform("modelMat", modelMat);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		shaderProgram.deactivate();
		glBindVertexArray(0);
	}
};

#endif
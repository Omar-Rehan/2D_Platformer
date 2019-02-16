#ifndef GROUND_H
#define GROUND_H

#include <GLAD/glad.h>
#include <GLM/glm.hpp>
#include <iostream>

#include "Shaders.h"

class Ground {
public:
	unsigned int vaoId;
	Shaders shaderProgram = Shaders();

	float upperSide, lowerSide, rightSide, leftSide;

	void Setup(float positionAttribute[], unsigned int positionIndices[], const char* vrtxShaderPath, const char* frgmtShaderPath) {
		upperSide = glm::max(positionAttribute[1], glm::max(positionAttribute[4], positionAttribute[7]));
		lowerSide = glm::min(positionAttribute[1], glm::min(positionAttribute[4], positionAttribute[7]));
		rightSide = glm::max(positionAttribute[0], glm::max(positionAttribute[3], positionAttribute[6]));
		leftSide  = glm::min(positionAttribute[0], glm::min(positionAttribute[3], positionAttribute[6]));

		// Create vertex array object
		glGenVertexArrays(1, &vaoId);
		glBindVertexArray(vaoId);

		// Create vertex buffer object 
		unsigned int positionVBO;
		glGenBuffers(1, &positionVBO);
		glBindBuffer(GL_ARRAY_BUFFER, positionVBO);
		glBufferData(GL_ARRAY_BUFFER, 4 * 3 * sizeof(float), positionAttribute, GL_STATIC_DRAW);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)(0 * sizeof(float)));
		glEnableVertexAttribArray(0);

	    // Create element buffer object
		unsigned int positionEBO;
		glGenBuffers(1, &positionEBO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, positionEBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * sizeof(unsigned int), positionIndices, GL_STATIC_DRAW);

		// Assign shaders to shader program
		shaderProgram.Setup(vrtxShaderPath, frgmtShaderPath);

		// Unbind VAO
		glBindVertexArray(0);
	}


	void Draw() {
		glBindVertexArray(vaoId);
		shaderProgram.activate();

		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		shaderProgram.deactivate();
		glBindVertexArray(0);
	}


	void DeleteVAO() {
		glDeleteVertexArrays(1, &vaoId);
		glDeleteBuffers(1, &vaoId);
	}
};

#endif
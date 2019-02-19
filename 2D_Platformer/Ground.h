#pragma once

#include "ShaderProgram.h"

class Ground {
public:
	// Members
	float upperSide, lowerSide, rightSide, leftSide;
	
	// Constructor
	Ground(float positionAttribute[], unsigned int positionIndices[], const char* vrtxShaderPath, const char* frgmtShaderPath);
	
	// Functions
	void Draw();
	void DeleteVAO();


protected:
	// Members
	unsigned int vaoId;
	ShaderProgram shaderProgram;
};
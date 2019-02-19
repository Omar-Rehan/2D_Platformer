#pragma once

#include <GLM/glm.hpp>
#include "Ground.h"

class Platform : public Ground {
public:
	Platform(float positionAttribute[], unsigned int positionIndices[], const char* vrtxShaderPath, const char* frgmtShaderPath);
	void Draw(glm::vec3 translationVector);

private:
	int numOfPlatforms;
	float originalColliderUpperSide, originalColliderLowerSide, originalColliderRightSide, originalColliderLeftSide;
};

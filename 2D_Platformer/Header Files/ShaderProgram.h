#pragma once

#include <GLM/glm.hpp>
#include <string>

class ShaderProgram {
public:

	// Public Functions 
	void Setup(const char* vrtxPath, const char* frgmtPath, const char* gmtryPath = nullptr);
	void activate();
	void deactivate();

	// Uniform Setters:
	void setBoolUniform(const std::string &name, bool val);
	void setIntUniform(const std::string &name, int val);
	void setFloatUniform(const std::string &name, float val);

	void setVec2Uniform(const std::string &name, const glm::vec2 &val) const;
	void setVec2Uniform(const std::string &name, float x, float y) const;

	void setVec3Uniform(const std::string &name, const glm::vec3 &val) const;
	void setVec3Uniform(const std::string &name, float x, float y, float z) const;

	void setVec4Uniform(const std::string &name, const glm::vec4 &value) const;
	void setVec4Uniform(const std::string &name, float x, float y, float z, float w) const;

	void setMat2Uniform(const std::string &name, const glm::mat2 &mat) const;
	void setMat3Uniform(const std::string &name, const glm::mat3 &mat) const;
	void setMat4Uniform(const std::string &name, const glm::mat4 &mat) const;



private:
	unsigned int shaderProgramId;
	void checkCompileErrors(GLuint shader, std::string type);

};

#ifndef SHADERS_H
#define SHADERS_H

#include <GLAD/glad.h>
#include <GLM/glm.hpp>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

class Shaders {
public:
	unsigned int shaderProgramId;

	// Function to set the 
	void Setup(const char* vrtxPath, const char* frgmtPath, const char* gmtryPath = nullptr) {
		// Source codes
		std::string vrtxSrcCode, frgmtSrcCode, gmtrySrcCode;

		// Input file streams
		std::ifstream vrtxFileStream, frgmtFileStream, gmtryFileStream;

		vrtxFileStream.exceptions(std::ifstream::failbit | std::ifstream::badbit);
		frgmtFileStream.exceptions(std::ifstream::failbit | std::ifstream::badbit);
		gmtryFileStream.exceptions(std::ifstream::failbit | std::ifstream::badbit);

		try {
			// open files
			vrtxFileStream.open(vrtxPath);
			frgmtFileStream.open(frgmtPath);

			std::stringstream vrtxStringStream, frgmtStringStream;

			// read file's buffer contents into streams
			vrtxStringStream << vrtxFileStream.rdbuf();
			frgmtStringStream << frgmtFileStream.rdbuf();

			// close files
			vrtxFileStream.close();
			frgmtFileStream.close();

			// convert stream into string
			vrtxSrcCode = vrtxStringStream.str();
			frgmtSrcCode = frgmtStringStream.str();

			if (gmtryPath != nullptr) {
				gmtryFileStream.open(gmtryPath);
				std::stringstream gmtryStringStream;
				gmtryStringStream << gmtryFileStream.rdbuf();
				gmtryFileStream.close();
				gmtrySrcCode = gmtryStringStream.str();
			}
		} catch (std::ifstream::failure e) {
			std::cout << "ERROR::SHADER::FILE_NOT_SUCCESSFULLY_READ" << std::endl;
		}

		// Source codes but in (const char*) form not strings
		const char* vrtxCode = vrtxSrcCode.c_str();
		const char* frgmtCode = frgmtSrcCode.c_str();

		// vertex shader
		unsigned int vrtxId = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(vrtxId, 1, &vrtxCode, NULL);
		glCompileShader(vrtxId);
		checkCompileErrors(vrtxId, "VERTEX");

		// fragment shader
		unsigned int frgmtId = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(frgmtId, 1, &frgmtCode, NULL);
		glCompileShader(frgmtId);
		checkCompileErrors(frgmtId, "FRAGMENT");

		// geometry shader
		unsigned int gmtryId;
		if (gmtryPath != nullptr) {
			const char* gmtryCode = gmtrySrcCode.c_str();
			gmtryId = glCreateShader(GL_GEOMETRY_SHADER);
			glShaderSource(gmtryId, 1, &gmtryCode, NULL);
			glCompileShader(gmtryId);
			checkCompileErrors(gmtryId, "GEOMETRY");
		}


		// Shader Program
		shaderProgramId = glCreateProgram();

		glAttachShader(shaderProgramId, vrtxId);
		glAttachShader(shaderProgramId, frgmtId);
		if (gmtryPath != nullptr) glAttachShader(shaderProgramId, gmtryId);

		glLinkProgram(shaderProgramId);

		checkCompileErrors(shaderProgramId, "PROGRAM");

		glDeleteShader(vrtxId);
		glDeleteShader(frgmtId);
		if (gmtryPath != nullptr) glDeleteShader(gmtryId);
	}

	void activate() { glUseProgram(shaderProgramId); }

	void deactivate() { glUseProgram(0); }

	// Set uniforms of different datatypes
	// Boolean
	void setBoolUniform(const std::string &name, bool val) { glUniform1i(glGetUniformLocation(shaderProgramId, name.c_str()), (int)val); }

	// Integer
	void setIntUniform(const std::string &name, int val) { glUniform1i(glGetUniformLocation(shaderProgramId, name.c_str()), val); }

	// Float
	void setFloatUniform(const std::string &name, float val) { glUniform1f(glGetUniformLocation(shaderProgramId, name.c_str()), val); }

	// Vector2 of floats
	void setVec2Uniform(const std::string &name, const glm::vec2 &val) { glUniform2fv(glGetUniformLocation(shaderProgramId, name.c_str()), 1, &val[0]); }
	void setVec2Uniform(const std::string &name, float x, float y) const { glUniform2f(glGetUniformLocation(shaderProgramId, name.c_str()), x, y); }

	// Vector3 of floats
	void setVec3Uniform(const std::string &name, const glm::vec3 &val) const { glUniform3fv(glGetUniformLocation(shaderProgramId, name.c_str()), 1, &val[0]); }
	void setVec3Uniform(const std::string &name, float x, float y, float z) const { glUniform3f(glGetUniformLocation(shaderProgramId, name.c_str()), x, y, z); }

	// Vector4 of floats
	void setVec4Uniform(const std::string &name, const glm::vec4 &value) const { glUniform4fv(glGetUniformLocation(shaderProgramId, name.c_str()), 1, &value[0]); }
	void setVec4Uniform(const std::string &name, float x, float y, float z, float w) { glUniform4f(glGetUniformLocation(shaderProgramId, name.c_str()), x, y, z, w); }

	// Martix2
	void setMat2Uniform(const std::string &name, const glm::mat2 &mat) const { glUniformMatrix2fv(glGetUniformLocation(shaderProgramId, name.c_str()), 1, GL_FALSE, &mat[0][0]); }

	// Matrix3
	void setMat3Uniform(const std::string &name, const glm::mat3 &mat) const { glUniformMatrix3fv(glGetUniformLocation(shaderProgramId, name.c_str()), 1, GL_FALSE, &mat[0][0]); }

	// Matrix4
	void setMat4Uniform(const std::string &name, const glm::mat4 &mat) const { glUniformMatrix4fv(glGetUniformLocation(shaderProgramId, name.c_str()), 1, GL_FALSE, &mat[0][0]); }



private:
	void checkCompileErrors(GLuint shader, std::string type) {
		GLint success;
		GLchar infoLog[1024];

		if (type != "PROGRAM") {
			glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
			if (!success) {
				glGetShaderInfoLog(shader, 1024, NULL, infoLog);
				std::cout << "ERROR::SHADER_COMPILATION_ERROR of type: " << type << std::endl;
			}
		} else {
			glGetProgramiv(shader, GL_LINK_STATUS, &success);
			if (!success) {
				glGetProgramInfoLog(shader, 1024, NULL, infoLog);
				std::cout << "ERROR::PROGRAM_LINKING_ERROR of type: " << type << std::endl;
			}
		}
	}

};

#endif

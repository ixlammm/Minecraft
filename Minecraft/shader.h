#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <string>
#include <iostream>
#include <fstream>

class Shader {
	unsigned int PID;
public:
	Shader(const char* vss, const char* fss) {
		unsigned int vs = glCreateShader(GL_VERTEX_SHADER);
		unsigned int fs = glCreateShader(GL_FRAGMENT_SHADER);
		
		PID = glCreateProgram();

		std::ifstream vsf(vss, std::ios::binary);
		std::ifstream fsf(fss, std::ios::binary);

		std::string line;
		std::string program;
		const char* program_size;

		// VERTEX SHADER
		while (std::getline(vsf, line))
			program += line + "\n";
		program.pop_back();
		program_size = program.c_str();
		glShaderSource(vs, 1, &program_size, 0);

		program.clear();

		//FRAGEMENT SHADER
		while (std::getline(fsf, line))
			program += line + "\n";
		program.pop_back();
		program_size = program.c_str();
		glShaderSource(fs, 1, &program_size, 0);

		// COMPILING
		int status;
		char log[512];

		glCompileShader(vs);
		glGetShaderiv(vs, GL_COMPILE_STATUS, &status);
		if (!status) {
			glGetShaderInfoLog(vs, 512, 0, log);
			std::cout << "Error: Compiling Vertex Shader\n" << log;
		}

		glCompileShader(fs);
		glGetShaderiv(fs, GL_COMPILE_STATUS, &status);
		if (!status) {
			glGetShaderInfoLog(fs, 512, 0, log);
			std::cout << "Error: Compiling Fragment Shader\n" << log;
		}

		glAttachShader(PID, vs);
		glAttachShader(PID, fs);

		glLinkProgram(PID);
		glGetProgramiv(PID, GL_LINK_STATUS, &status);
		if (!status)
			std::cout << "Error: Linking Program\n";
	}

	unsigned int GetPID() {
		return PID;
	}

	void Use() {
		glUseProgram(PID);
	}

	void SetMat4(const char* name, glm::mat4 mat) {
		glUniformMatrix4fv(glGetUniformLocation(PID, name), 1, GL_FALSE, glm::value_ptr(mat));
	}

	void SetUint(const char* name, unsigned v) {
		glUniform1ui(glGetUniformLocation(PID, name), v);
	}
};
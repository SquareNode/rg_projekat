#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>


#include <string>
#include <fstream>
#include <iostream>
#include <sstream>


class Shader { 
	public:
		unsigned int ID;
		
		Shader(const char *vs, const char *fs);
		
		void use();
		
		void setBool(const std::string &name, bool val) const;
		void setInt(const std::string &name, int val) const;
		void setFloat(const std::string &name, float val) const;
		void setVec2(const std::string &name, const glm::vec2 &value) const;
		void setVec2(const std::string &name, float x, float y) const;
		void setVec3(const std::string &name, const glm::vec3 &value) const;
		void setVec3(const std::string &name, float x, float y, float z) const;
		void setVec4(const std::string &name, const glm::vec4 &value) const;
		void setVec4(const std::string &name, float x, float y, float z, float w) const;
		void setMat2(const std::string &name, const glm::mat2 &mat) const;
		void setMat3(const std::string &name, const glm::mat3 &mat) const;
		void setMat4(const std::string &name, const glm::mat4 &mat) const;

};
		
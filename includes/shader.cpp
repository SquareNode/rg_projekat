#include "shader.h"

Shader::Shader(const char *vs, const char *fs) {
	
	std::string vsCode, fsCode;
	std::ifstream vsFile, fsFile;
	
	vsFile.exceptions( std::ifstream::failbit | std::ifstream::badbit);
	fsFile.exceptions( std::ifstream::failbit | std::ifstream::badbit);
	
	try {
		
		vsFile.open(vs);
		fsFile.open(fs);
		
		std::stringstream vsStream, fsStream;
		
		vsStream << vsFile.rdbuf();
		fsStream << fsFile.rdbuf();
		
		vsFile.close();
		fsFile.close();
		
		vsCode = vsStream.str();
		fsCode = fsStream.str();
		
	}
	catch(std::ifstream::failure e) {
		std::cout << "Error shader file not successfully read!\n";
	}
	
	const char *vShaderCode = vsCode.c_str();
	const char *fShaderCode = fsCode.c_str();
	
	unsigned int vertex, fragment;
	int success;
	char log[512];
	
	vertex = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertex, 1, &vShaderCode, NULL);
	
	glCompileShader(vertex);
	
	glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
	
	if(!success){
		glGetShaderInfoLog(vertex, 512, NULL, log);
		std::cout << "err shader compilation : " << log << '\n';
	}
	
	fragment = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragment, 1, &fShaderCode, NULL);
	
	glCompileShader(fragment);
	
	glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
	
	if(!success){
		glGetShaderInfoLog(fragment, 512, NULL, log);
		std::cout << "err frag compilation : " << log << '\n';
	}
	
	this->ID = glCreateProgram();
	glAttachShader(this->ID, vertex);
	glAttachShader(this->ID, fragment);
	glLinkProgram(this->ID);
	
	glGetProgramiv(this->ID, GL_LINK_STATUS, &success);

	if(!success){
		glGetProgramInfoLog(this->ID, 512, NULL, log);
		std::cout << "err linking : " << log << '\n';
	}
	
	glDeleteShader(vertex);
	glDeleteShader(fragment);
	
}

void Shader::use() {
	glUseProgram(this->ID);
}

void Shader::setBool(const std::string &name, bool val) const {
	glUniform1i(glGetUniformLocation(this->ID, name.c_str()), (int) val);
}

void Shader::setInt(const std::string &name, int val) const{ 
    glUniform1i(glGetUniformLocation(this->ID, name.c_str()), val); 
}
void Shader::setFloat(const std::string &name, float val) const{ 
    glUniform1f(glGetUniformLocation(this->ID, name.c_str()), val); 
} 

void Shader::setVec2(const std::string &name, const glm::vec2 &value) const { 
	glUniform2fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]); 
}
void Shader::setVec2(const std::string &name, float x, float y) const { 
	glUniform2f(glGetUniformLocation(ID, name.c_str()), x, y); 
}
void Shader::setVec3(const std::string &name, const glm::vec3 &value) const { 
	glUniform3fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]); 
}
void Shader::setVec3(const std::string &name, float x, float y, float z) const { 
	glUniform3f(glGetUniformLocation(ID, name.c_str()), x, y, z); 
}
void Shader::setVec4(const std::string &name, const glm::vec4 &value) const { 
        glUniform4fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]); 
}
void Shader::setVec4(const std::string &name, float x, float y, float z, float w) const { 
	glUniform4f(glGetUniformLocation(ID, name.c_str()), x, y, z, w); 
}

void Shader::setMat2(const std::string &name, const glm::mat2 &mat) const{
	glUniformMatrix2fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
}

void Shader::setMat3(const std::string &name, const glm::mat3 &mat) const{
	glUniformMatrix3fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
}

void Shader::setMat4(const std::string &name, const glm::mat4 &mat) const{
	glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
}





























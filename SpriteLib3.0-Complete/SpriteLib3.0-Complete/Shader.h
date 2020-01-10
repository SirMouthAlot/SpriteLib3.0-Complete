#ifndef __SHADER_H__
#define __SHADER_H__

#include <string>
#include <utility>
#include <vector>
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <GL/glew.h>
#include "Camera.h"

#pragma comment(lib, "Matrix.lib")

#include "Matrix.h"

class Shader
{
public:
	//Empty constructor
	Shader() {}
	//Constructor
	//*Loads the shader with vert file and frag file
	Shader(const std::string &vertFile, const std::string &fragFile, const std::string & geomFile="");
	//Deconstructor
	//*If it's loaded, unload the shader
	~Shader();

	//Load the shader
	//*Read in the files
	//*Send them to GL
	bool Load(const std::string &vertFile, const std::string &fragFile, const std::string & geomFile="");
	//gets if the shader is loaded
	bool IsLoaded() const;
	//Unload the shader
	//*unlink the program
	//*delete each part
	void Unload();
	//Link program
	bool LinkProgram();

	//Binds the shader
	void Bind() const;
	//Unbinds the shader
	void Unbind() const;

	//Sends the uniforms to the shader
	void SendUniform(const std::string &uniformName, const int &i) const;
	void SendUniform(const std::string &uniformName, const unsigned int &i) const;
	void SendUniform(const std::string &uniformName, const float &f) const;
	void SendUniform(const std::string &uniformName, const vec2 &vector) const;
	void SendUniform(const std::string &uniformName, const float &x, const float &y) const;
	void SendUniform(const std::string &uniformName, const vec3 &vector) const;
	void SendUniform(const std::string &uniformName, const float &x, const float &y, const float &z) const;
	void SendUniform(const std::string &uniformName, const vec4 &vector) const;
	void SendUniform(const std::string &uniformName, const float &x, const float &y, const float &z, const float &w) const;
	void SendUniform(const std::string &uniformName, const mat3 &matrix) const;
	void SendUniform(const std::string &uniformName, const mat4 &matrix) const;
private:
	//Holds the file names
	std::string m_vertShaderFile, m_fragShaderFile, m_geomShaderFile;

	//Handle to the different parts of the shader
	GLuint m_vertexShader = GL_NONE;
	GLuint m_fragShader = GL_NONE;
	GLuint m_geomShader = GL_NONE;
	GLuint m_program = GL_NONE;
	//Is the shader initializes?
	bool m_isInit = false;

	//Reads in the file
	//*Stores it in a string
	std::string ReadFile(const std::string &fileName) const;
	//Compiles the shader
	bool CompileShader(GLuint shader) const;
	//Outputs the shader error log
	void OutputShaderLog(GLuint shader) const;
	//Outputs the program error log
	void OutputProgramLog() const;

	//Gets a uniform location using a uniform name
	GLint GetUniformLocation(const std::string &uniformName) const;
};

#endif // !__SHADER_H__

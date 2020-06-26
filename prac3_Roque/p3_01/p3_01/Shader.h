#pragma once
#include<stdio.h>
#include<string>
#include<iostream>
#include<fstream>

#include<glew.h>

class Shader
{
public:
	Shader();
	void CreatefromString(const char* vertexCode, const char* fragmentCode);
	void CreateFromFiles(const char* vertexLocation, const char* fragmentLocation);
	std::string ReadFile(const char*  fileLocation);

	//******************************
	void turnOn();
	void turnOff();
	//************************************
	GLuint getProjectLocation();
	GLuint getModelLoaction();
	GLuint getViewLocation();
	//-----------------------
	GLuint getc1Location();
	GLuint getc2Location();

	void useShader();
	void ClearShader();
	~Shader();

private: 
	GLuint shaderID, uniformProjection, uniformModel, uniformView, uniformc1, uniformc2;
	void CompileShader(const char* vertexCode, const char* fragmentCode);
	void AddShader(GLuint theProgram, const char* ShaderCode, GLenum shaderType);
};


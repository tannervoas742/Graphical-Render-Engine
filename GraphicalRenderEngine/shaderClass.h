#ifndef SHADER_CLASS_H
#define SHADER_CLASS_H

#include "GREGlobals.h"
#include<glad/glad.h>
#include<string>
#include<fstream>
#include<sstream>
#include<iostream>
#include<cerrno>
#include <map>

std::string get_file_contents(const char* filename);

class Shader
{
public:
	// Reference ID of the Shader Program
	GLuint ID;
	// Constructor that build the Shader Program from 2 different shaders
	Shader(const char* vertexFile, const char* fragmentFile);
	Shader(const char* vertexFile, const char* fragmentFile, const char* geometryFile);

	void StoreShader(std::string Tag);

	// Activates the Shader Program
	void Activate();
	// Deletes the Shader Program
	void Delete();

	inline static std::map<std::string, Shader*> LoadedShaders;
private:
	std::string MyTag;
	// Checks if the different Shaders have compiled properly
	void compileErrors(unsigned int shader, const char* type);
};


#endif
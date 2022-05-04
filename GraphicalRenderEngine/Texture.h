#ifndef TEXTURE_CLASS_H
#define TEXTURE_CLASS_H

#include<glad/glad.h>
#include<stb/stb_image.h>

#include"shaderClass.h"

class Texture
{
public:
	GLuint ID;
	std::string type;
	GLuint unit;

	Texture(std::string image, std::string texType, GLuint slot);

	void StoreTexture(std::string File);

	// Assigns a texture unit to a texture
	void texUnit(Shader& shader, const char* uniform, GLuint unit);
	// Binds a texture
	void Bind();
	// Unbinds a texture
	void Unbind();
	// Deletes a texture
	void Delete();

	inline static std::map<std::string, Texture*> LoadedTextures;

private:
	std::string File;
};
#endif
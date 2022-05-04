#ifndef MODEL_CLASS_H
#define MODEL_CLASS_H

#include "nlohmann/json.hpp"
#include "Mesh.h"
#include "Texture.h"
#include "shaderClass.h"

using json = nlohmann::json;

class Model
{
public:
	// Loads in a model from a file and stores tha information in 'data', 'JSON', and 'file'
	Model(const char* file);
	~Model(void);

	void Draw
	(
		Shader& shader,
		Camera& camera,
		glm::mat4 matrix = glm::mat4(1.0f),
		glm::vec3 translation = glm::vec3(0.0f, 0.0f, 0.0f),
		glm::quat rotation = glm::quat(1.0f, 0.0f, 0.0f, 0.0f),
		glm::vec3 scale = glm::vec3(1.0f, 1.0f, 1.0f)
	);

	Texture* GetTexture(std::string Type) { return MyTextures[Type]; }

	inline static std::map<std::string, Model*> LoadedModels;

	std::string Title;
	Mesh* MyMesh;
private:
	std::vector<Vertex>& LoadVertices(std::string VerticesFileName);
	std::vector<GLuint>& LoadIndices(std::string IndicesFileName);
	std::map<std::string, Texture*>& LoadTextures(std::vector<std::string> Order, std::map<std::string, std::string> TextureMap);


	std::vector<Vertex> MyVertices;
	std::vector<GLuint> MyIndices;
	std::map<std::string, Texture*> MyTextures;
	json JSON;

	int MyID;
};


#endif
#ifndef MODEL_INSTANCE_CLASS_H
#define MODEL_INSTANCE_CLASS_H

#include "Model.h"
#include "GREGlobals.h"

class ModelInstance
{
public:
	// Loads in a model from a file and stores tha information in 'data', 'JSON', and 'file'
	ModelInstance(Model* SourceModel);
	ModelInstance(std::string SourceModelName);
	~ModelInstance(void);

	void Draw(Shader& shader, Camera& camera);

	void SetTranslation(glm::vec3 In) { Translation = In; }
	void SetRotation(glm::quat In) { Rotation = In; }
	void SetScale(glm::vec3 In) { Scale = In; }

	Texture* GetTexture(std::string Type) { return SourceModel->GetTexture(Type); }
	glm::mat4& GetMatrix(void) { return Matrix; }
	glm::vec3& GetTranslation(void) { return Translation; }
	glm::quat& GetRotation(void) { return Rotation; }
	glm::vec3& GetScale(void) { return Scale; }

	inline static std::map<std::string, Model*> LoadedModels;

	std::string Title;
	

private:
	void DoModelStore(Model* ModelToStore);
	Model* SourceModel;

	glm::mat4 Matrix = glm::mat4(1.0f);
	glm::vec3 Translation = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::quat Rotation = glm::quat(1.0f, 0.0f, 0.0f, 0.0f);
	glm::vec3 Scale = glm::vec3(1.0f, 1.0f, 1.0f);
};


#endif
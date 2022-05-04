#include "PCH.h"
#include "ModelInstance.h"

ModelInstance::ModelInstance(Model* SourceModel)
{
	DoModelStore(SourceModel);
}

ModelInstance::ModelInstance(std::string SourceModelName)
{
	if (Model::LoadedModels.find(SourceModelName) == Model::LoadedModels.end()) {
		COUT(std::format("[ERROR]: Failed to create model instance from: {}", SourceModelName));
		GREGlobalSettings.TerminateRenderer = true;
	}
	DoModelStore(Model::LoadedModels[SourceModelName]);
}

ModelInstance::~ModelInstance(void)
{
}

void ModelInstance::Draw(Shader& shader, Camera& camera)
{
	SourceModel->Draw(shader, camera, Matrix, Translation, Rotation, Scale);
}

void ModelInstance::DoModelStore(Model* ModelToStore)
{
	SourceModel = ModelToStore;
}

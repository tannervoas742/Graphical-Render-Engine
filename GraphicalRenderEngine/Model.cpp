#include "PCH.h"
#include"Model.h"

Model::Model(const char* file)
{
	JSON = json::parse(get_file_contents(file));

	Title = JSON["Title"];
	if (Model::LoadedModels.find(Title) != Model::LoadedModels.end()) {
		COUT(std::format("Model \"{}\" already loaded!", Title));
		return;
	}
	COUT(std::format("Loading Model \"{}\" from file \"{}\"", Title, file));

	/////////////////////////////

	MyID = JSON.size() + 1;


	LoadVertices(JSON["Mesh"]["vertices"]);
	LoadIndices(JSON["Mesh"]["indices"]);
	LoadTextures(JSON["Texture Order"], JSON["Textures"]);

	MyMesh = new Mesh(MyVertices, MyIndices, MyTextures);

	/////////////////////////////

	Model::LoadedModels[Title] = this;
	COUT(std::format("Loaded Model[#{}]: \"{}\"", Model::LoadedModels.size(), Title));
}

Model::~Model(void)
{
	delete MyMesh;
}

void Model::Draw(Shader& shader, Camera& camera, glm::mat4 matrix, glm::vec3 translation, glm::quat rotation, glm::vec3 scale)
{
	MyMesh->Draw(shader, camera, matrix, translation, rotation, scale);
}

std::vector<Vertex>& Model::LoadVertices(std::string VerticesFileName)
{
	std::ifstream VerticesFile = std::ifstream(VerticesFileName);

	glm::vec3 PositionStore(0.0f);
	glm::vec3 NormalStore(0.0f);
	glm::vec3 ColorStore(0.0f);
	glm::vec2 TexStore(0.0f);
	int VerticeInputCount = 0;

	while (VerticesFile) {
		float Value;
		VerticesFile >> Value;


		if (VerticeInputCount < 3) {
			PositionStore[VerticeInputCount - 0] = Value;
		}
		else if (VerticeInputCount < 6) {
			NormalStore[VerticeInputCount - 3] = Value;
		}
		else if (VerticeInputCount < 9) {
			ColorStore[VerticeInputCount - 6] = Value;
		}
		else if (VerticeInputCount < 11) {
			TexStore[VerticeInputCount - 9] = Value;
		}
		VerticeInputCount++;
		if (VerticeInputCount == 3 + 3 + 3 + 2) {
			MyVertices.push_back(Vertex{ PositionStore, NormalStore, ColorStore, TexStore });
			PositionStore = glm::vec3(0.0f);
			NormalStore = glm::vec3(0.0f);
			ColorStore = glm::vec3(0.0f);
			TexStore = glm::vec2(0.0f);
			VerticeInputCount = 0;
		}

	}


	VerticesFile.close();
	return MyVertices;
}

std::vector<GLuint>& Model::LoadIndices(std::string IndicesFileName)
{
	std::ifstream IndicesFile = std::ifstream(IndicesFileName);


	while (IndicesFile) {
		GLuint Value;
		IndicesFile >> Value;

		MyIndices.push_back(Value);
	}

	IndicesFile.close();
	return MyIndices;
}

std::map<std::string, Texture*>& Model::LoadTextures(std::vector<std::string> Order, std::map<std::string, std::string> TextureMap)
{
	int Index = 0;
	for (auto TextureKey : Order) {
		std::string Type = TextureKey;
		std::string File = TextureMap[TextureKey];

		COUT(std::format("{} {} {}", File.c_str(), Type.c_str(), Index));
		if (Texture::LoadedTextures.find(File) == Texture::LoadedTextures.end()) {
			MyTextures[Type] = new Texture(File.c_str(), Type.c_str(), Index);
			MyTextures[Type]->StoreTexture(File);
		}
		else {
			MyTextures[Type] = Texture::LoadedTextures[File];
		}
		Index++;

	}
	return MyTextures;
}

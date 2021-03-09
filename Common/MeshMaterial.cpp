#include "MeshMaterial.h"
#include "Assets.h"
#include "TextureLoader.h"
#include "ResourceManager.h"

#include <fstream>
#include <iostream>

using namespace NCL;
using namespace NCL::Rendering;
using std::ifstream;

MeshMaterial::MeshMaterial(const std::string& filename) {
	ifstream file(Assets::MESHDIR + filename);

	string dataType;
	file >> dataType;

	if (dataType != "MeshMat") {
		std::cout << "File " << filename << " is not a MeshMaterial!\n";
		return;
	}
	int version;
	file >> version;

	if (version != 1) {
		std::cout << "File " << filename << " has incompatible version " << version << "!\n";
		return;
	}

	int matCount;
	int meshCount;
	file >> matCount;
	file >> meshCount;

	materialLayers.resize(matCount);

	for (int i = 0; i < matCount; ++i) {
		string name;
		int count;
		file >> name;
		file >> count;

		for (int j = 0; j < count; ++j) {
			string entryData;
			file >> entryData;
			string channel;
			string file;
			int split = (int)entryData.find_first_of(':');
			channel = entryData.substr(0, split);
			file = entryData.substr(split + 1);

			materialLayers[i].entries.insert(std::make_pair(channel, std::make_pair(file, nullptr)));
		}
	}

	for (int i = 0; i < meshCount; ++i) {
		int entry;
		file >> entry;
		meshLayers.emplace_back(&materialLayers[entry]);
	}
}

const MeshMaterialEntry* MeshMaterial::GetMaterialForLayer(int i) const {
	if (i < 0 || i >= meshLayers.size()) {
		return nullptr;
	}
	return meshLayers[i];
}

void MeshMaterial::LoadTextures() {
	for(auto & i : meshLayers) {
		i->LoadTextures();
	}
}

void MeshMaterial::LoadTextures(ResourceManager* manager) {
	for (auto& i : meshLayers) {
		i->LoadTextures(manager);
	}
}

void MeshMaterialEntry::LoadTextures() {
	for (auto& i : entries) {
		string filename = Assets::TEXTUREDIR + i.second.first;

		TextureBase* t = TextureLoader::LoadAPITexture(filename);

		i.second.second = t;
	}
}

void MeshMaterialEntry::LoadTextures(ResourceManager* manager) {
	for (auto& i : entries) {
	//	string filename = Assets::TEXTUREDIR.substr(0,Assets::TEXTUREDIR.size()-1) + i.second.first;

		TextureBase* t = manager->LoadTexture(i.second.first);

		i.second.second = t;
	}
}
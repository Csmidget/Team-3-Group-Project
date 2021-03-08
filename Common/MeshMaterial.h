#pragma once
#include <string>
#include <vector>
#include <map>

using std::string;
using std::vector;
using std::map;

namespace NCL {

	namespace Rendering {
		class TextureBase;
		class ResourceManager;
	}
	class MeshMaterialEntry {
		friend class MeshMaterial;
	public:
		bool GetEntry(const string& name, const string** output) const {
			auto i = entries.find(name);
			if (i == entries.end()) {
				return false;
			}
			*output = &i->second.first;
			return true;
		}
		Rendering::TextureBase* GetEntry(const string& name) const {
			auto i = entries.find(name);
			if (i == entries.end()) {
				return nullptr;
			}
			return i->second.second;
		}
		void LoadTextures();
		void LoadTextures(Rendering::ResourceManager* manager);

	protected:
		std::map<string, std::pair<string, Rendering::TextureBase*>> entries;
	};

	class MeshMaterial
	{
	public:
		MeshMaterial(const std::string& filename);
		~MeshMaterial() {}
		const MeshMaterialEntry* GetMaterialForLayer(int i) const;

		void LoadTextures();
		void LoadTextures(Rendering::ResourceManager* manager);

	protected:
		std::vector<MeshMaterialEntry>	materialLayers;
		std::vector<MeshMaterialEntry*> meshLayers;
	};

}
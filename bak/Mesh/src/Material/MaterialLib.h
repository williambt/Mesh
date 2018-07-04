#pragma once

#include <vector>
#include "Material.h"

class MaterialLib
{
private:
	std::vector<Material*> _materials;
public:

	inline std::vector<Material*>& GetMaterials() { return _materials; }
	inline Material* GetMaterial(std::string name) { for (Material* mat : _materials) if (mat->name == name) return mat; return nullptr; }
	inline void AddMaterial(Material* mat) { _materials.push_back(mat); }
};
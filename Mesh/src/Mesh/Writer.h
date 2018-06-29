#pragma once

#include "Mesh.h"

static void writeObj(const char* path, const Mesh& mesh)
{

	FILE* file;
	fopen_s(&file, path, "wt");

	if (!file)
		return;

	for (int i = 0; i < mesh.vertices.size(); ++i)
	{
		std::string line = "v " + std::to_string(mesh.vertices[i].x) + ' ' + std::to_string(mesh.vertices[i].y) + ' ' + std::to_string(mesh.vertices[i].z) + '\n';
		fwrite(line.c_str(), 1, line.size(), file);
	}

	for (int i = 0; i < mesh.normals.size(); ++i)
	{
		std::string line = "vn " + std::to_string(mesh.normals[i].x) + ' ' + std::to_string(mesh.normals[i].y) + ' ' + std::to_string(mesh.normals[i].z) + '\n';
		fwrite(line.c_str(), 1, line.size(), file);
	}

	for (int i = 0; i < mesh.textureCoords.size(); ++i)
	{
		std::string line = "vt " + std::to_string(mesh.textureCoords[i].x) + ' ' + std::to_string(mesh.textureCoords[i].y) + '\n';
		fwrite(line.c_str(), 1, line.size(), file);
	}

	for (const Group& g : mesh.groups)
	{
		fwrite(("g " + g.name + '\n').c_str(), 1, g.name.size() + 3, file);
		for (const Face& f : g.faces)
		{
			std::string line = "f";
			for (int i = 0; i < f.verts.size(); ++i)
			{
				line += ' ' + std::to_string(f.verts[i]+1) + '/' + std::to_string(f.texts[i]+1) + '/' + std::to_string(f.norms[i]+1);
			}
			line += '\n';
			fwrite(line.c_str(), 0, line.size(), file);
		}
	}

	fclose(file);
}
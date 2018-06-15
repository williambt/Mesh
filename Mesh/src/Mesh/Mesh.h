#pragma once

#include <vector>
#include "Group.h"
#include <glm\vec3.hpp>
#include <glm\vec2.hpp>
#include "../Material/MaterialLib.h"
#include "../Utils/StringUtils.h"
#include <iostream>

class Mesh
{
public:
	std::vector<glm::vec3> vertices;
	std::vector<glm::vec3> normals;
	std::vector<glm::vec2> textureCoords;

	std::vector<Group> groups;

	MaterialLib mtllib;
};

static bool readMtl(const char* mtlib, MaterialLib& outMaterialLib);

static bool readObj(const char* fileName, Mesh& outMesh, bool triangulate = true)
{
	FILE* file;
	fopen_s(&file, fileName, "rt");

	if (!file)
		return false;

	Group* currGroup = nullptr;

	std::string mtlib;

	char line[250];
	while (fgets(line, 250, file) != NULL)
	{
		char type[] = { line[0], line[1], '\0' };
		if (line[0] == '#')
			continue;
		if (strcmp(type, "v ") == 0)
		{
			glm::vec3 vert;
			sscanf_s(line, "v %f %f %f", &vert.x, &vert.y, &vert.z);
			outMesh.vertices.push_back(vert);
		}
		else if (strcmp(type, "vt") == 0)
		{
			glm::vec2 vt;
			sscanf_s(line, "vt %f %f", &vt.x, &vt.y);
			outMesh.textureCoords.push_back(vt);
		}
		else if (strcmp(type, "vn") == 0)
		{
			glm::vec3 normal;
			sscanf_s(line, "vn %f %f %f", &normal.x, &normal.y, &normal.z);
			outMesh.normals.push_back(normal);
		}
		else if (strcmp(type, "f ") == 0)
		{
			Face* f = new Face();
			std::string sLine(line);
			std::vector<std::string> verts = splitString(sLine.substr(2), ' ');

			int currTriVerts = 0;
			struct
			{
				unsigned int vert, tex, norm;
			} first;

			for (unsigned int i = 0; i < verts.size(); ++i)
			{
				if (triangulate)
				{
					if (currTriVerts == 3)
					{
						i--;
						currTriVerts = 0;
						currGroup->faces.push_back(*f);
						delete f;
						f = new Face();

						f->verts.push_back(first.vert);
						f->texts.push_back(first.tex);
						f->norms.push_back(first.norm);
					}
					currTriVerts++;
				}

				std::vector<std::string> inds = splitString(verts[i], '/');

				f->verts.push_back(std::stoul(inds[0]) - 1ul);
				if (inds.size() > 1)
				{
					if (inds.size() > 2)
					{
						if (inds[1] != "")
						{
							f->texts.push_back(std::stoul(inds[1]) - 1ul);
						}
						f->norms.push_back(std::stoul(inds[2]) - 1ul);
					}
					else
					{
						f->texts.push_back(std::stoul(inds[1]) - 1ul);
					}
				}
				if (i == 0)
				{
					first.vert = f->verts[0];
					if (!f->texts.empty())
						first.tex = f->texts[0];
					if (!f->norms.empty())
						first.norm = f->norms[0];
				}
			}

			if (!currGroup)
				currGroup = new Group("Default");
			currGroup->faces.push_back(*f);
			delete f;
		}
		else if (strcmp(type, "us") == 0)
		{
			if (!currGroup)
				currGroup = new Group("Default");
			currGroup->material = std::string(line + 7);
			currGroup->material = currGroup->material.substr(0, currGroup->material.size() - 1);
		}
		else if (strcmp(type, "g ") == 0)
		{
			if (currGroup)
			{
				outMesh.groups.push_back(*currGroup);
				delete currGroup;
				currGroup = NULL;
			}
			currGroup = new Group(std::string(line).substr(2));
		}
		else if (strcmp(type, "mt") == 0)
		{
			mtlib = line + 7;
			mtlib = getPathDir(fileName) + mtlib.substr(0, mtlib.size() - 1);
		}
	}

	if (currGroup != NULL)
	{
		outMesh.groups.push_back(*currGroup);
		delete currGroup;
	}

	fclose(file);


	//---------- MTL ----------//
	if (mtlib.empty())
	{
		mtlib = removePathExt(fileName) + ".mtl";
	}

	if (!readMtl(mtlib.c_str(), outMesh.mtllib))
	{
		Material* defmat = new Material();
		defmat->ambient = glm::vec3(1.0f);
		defmat->diffuse = glm::vec3(1.0f);
		defmat->specular = glm::vec3(1.0f);
		defmat->shine = 5.0f;
		defmat->name = "Material";
		defmat->diffuseTexture.LoadFromFile("res/white.png");
		outMesh.mtllib.AddMaterial(defmat);
		for (Group& g : outMesh.groups)
			g.material = "Material";

		std::cout << "Nenhum material encontrado. Usando padrao." << std::endl;
	}
	return true;
}

bool readMtl(const char* mtlib, MaterialLib& outMaterialLib)
{
	FILE* file;
	fopen_s(&file, mtlib, "rt");

	if (!file)
		return false;

	Material* currMaterial = NULL;

	char line[250];
	while (fgets(line, 250, file) != NULL)
	{
		char type[] = { line[0], line[1], '\0' };

		if (line[0] == '#')
			continue;

		if (strcmp(type, "ne") == 0)
		{
			if (currMaterial != NULL)
			{
				if (!currMaterial->diffuseTexture.HasData())
					currMaterial->diffuseTexture.LoadFromFile("res/white.png");
				outMaterialLib.AddMaterial(currMaterial);
				//delete currMaterial;
			}
			char name[50];
			sscanf_s(line, "newmtl %s", name, 50);
			currMaterial = new Material(name);
		}
		else if (strcmp(type, "Ka") == 0)
		{
			glm::vec3 rgb;
			sscanf_s(line, "Ka %f %f %f", &rgb.r, &rgb.g, &rgb.b);
			currMaterial->ambient = rgb;
		}
		else if (strcmp(type, "Kd") == 0)
		{
			glm::vec3 rgb;
			sscanf_s(line, "Kd %f %f %f", &rgb.r, &rgb.g, &rgb.b);
			currMaterial->diffuse = rgb;
		}
		else if (strcmp(type, "Ks") == 0)
		{
			glm::vec3 rgb;
			sscanf_s(line, "Ks %f %f %f", &rgb.r, &rgb.g, &rgb.b);
			currMaterial->specular = rgb;
		}
		else if (strcmp(type, "Ns") == 0)
		{
			sscanf_s(line, "Ns %f", &currMaterial->shine);
		}
		else if (strcmp(type, "ma") == 0)
		{
			char* mapType = (char*)malloc(10);
			char* fName = (char*)malloc(100);
			sscanf_s(line, "%s %s", mapType, 10, fName, 100);

			if (strcmp(mapType, "map_Kd") == 0)
			{
				currMaterial->diffuseTexture.LoadFromFile(fName);
			}

			free(mapType);
			free(fName);
		}
	}

	if (currMaterial != NULL)
	{
		if (!currMaterial->diffuseTexture.HasData())
			currMaterial->diffuseTexture.LoadFromFile("res/white.png");
		outMaterialLib.AddMaterial(currMaterial);
	}

	fclose(file);

	return true;
}
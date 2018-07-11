#pragma once

#include "Mesh.h"
#include "../Renderer/VertexArray.h"
#include <vector>
#include "../Transform.h"

class DrawableGroup
{
public:
	VertexBuffer buffer;
	BufferLayout layout;
	VertexArray va;
	unsigned int count;

	Material material;

	DrawableGroup() {}

	void Draw(const Shader& shader)
	{

		material.ApplyToShader(shader);

		shader.Bind();
		material.diffuseTexture.Bind(0);
		material.normalMap.Bind(1);
		va.Bind();

		glDrawArrays(GL_TRIANGLES, 0, count);
	}
};

class Drawable
{
public:
	std::vector<DrawableGroup*> groups;
	Transform transform;
	
	Drawable(void) {}
	~Drawable(void)
	{
		for (auto g : groups)
			delete g;
	}

	Drawable* GetCopy()
	{
	}

	void Draw(const Shader& shader)
	{
		glm::mat4 matrix = transform.GetMatrix();
		shader.UniformMat4f("mMat", matrix);
		shader.UniformMat3f("inv_transp", glm::transpose(glm::inverse(glm::mat3(matrix))));
		for (auto g : groups)
			g->Draw(shader);
	}
};

static Drawable* meshToDrawable(Mesh& mesh)
{
	bool hasTexCoord;
	bool hasNormals;

	Drawable* res = new Drawable();

	if (mesh.textureCoords.empty())
		hasTexCoord = false;
	else
		hasTexCoord = true;

	if (mesh.normals.empty())
		hasNormals = false;
	else
		hasNormals = true;

	for (const Group& g : mesh.groups)
	{
		DrawableGroup* currGroup = new DrawableGroup();

		std::vector<float> buffer;
		unsigned int count = 0;

		for (const Face& face : g.faces)
		{
			glm::vec3 tangent;
			glm::vec3 edge1 = mesh.vertices[face.verts[1]] - mesh.vertices[face.verts[0]];
			glm::vec3 edge2 = mesh.vertices[face.verts[2]] - mesh.vertices[face.verts[0]];
			glm::vec3 normal;
			if (!hasNormals)
			{
				normal = glm::cross(edge1, edge2);
			}
			glm::vec2 deltaUV1 = mesh.textureCoords[face.texts[1]] - mesh.textureCoords[face.texts[0]];
			glm::vec2 deltaUV2 = mesh.textureCoords[face.texts[2]] - mesh.textureCoords[face.texts[0]];

			float f = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV2.x * deltaUV1.y);

			tangent.x = f * (deltaUV2.y * edge1.x - deltaUV1.y * edge2.x);
			tangent.y = f * (deltaUV2.y * edge1.y - deltaUV1.y * edge2.y);
			tangent.z = f * (deltaUV2.y * edge1.z - deltaUV1.y * edge2.z);
			tangent = glm::normalize(tangent);

			for (unsigned int i = 0; i < face.verts.size(); ++i)
			{
				count++;

				buffer.push_back(mesh.vertices[face.verts[i]].x);
				buffer.push_back(mesh.vertices[face.verts[i]].y);
				buffer.push_back(mesh.vertices[face.verts[i]].z);
				if (hasNormals)
				{
					buffer.push_back(mesh.normals[face.norms[i]].x);
					buffer.push_back(mesh.normals[face.norms[i]].y);
					buffer.push_back(mesh.normals[face.norms[i]].z);
				}
				else
				{
					buffer.push_back(normal.x);
					buffer.push_back(normal.y);
					buffer.push_back(normal.z);
				}
				if (hasTexCoord)
				{
					buffer.push_back(mesh.textureCoords[face.texts[i]].x);
					buffer.push_back(mesh.textureCoords[face.texts[i]].y);
				}
				buffer.push_back(tangent.x);
				buffer.push_back(tangent.y);
				buffer.push_back(tangent.z);
			}
		}

		currGroup->buffer.BufferData(&buffer[0], buffer.size() * sizeof(float));
		currGroup->layout.AddElement(LayoutElement(3, GL_FLOAT, ATTRIB_VERTEX_POS));
		currGroup->layout.AddElement(LayoutElement(3, GL_FLOAT, ATTRIB_NORMAL));
		if (hasTexCoord)
			currGroup->layout.AddElement(LayoutElement(2, GL_FLOAT, ATTRIB_TEX_COORD));
		currGroup->layout.AddElement(LayoutElement(3, GL_FLOAT, ATTRIB_TANGENT));
		currGroup->va.AddBuffers(currGroup->buffer, currGroup->layout);
		currGroup->count = count;

		currGroup->material = *mesh.mtllib.GetMaterial(g.material);

		res->groups.push_back(currGroup);
	}

	return res;
}
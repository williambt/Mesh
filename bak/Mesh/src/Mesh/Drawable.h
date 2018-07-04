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

	Drawable* res = new Drawable();

	if (mesh.textureCoords.empty())
		hasTexCoord = false;
	else
		hasTexCoord = true;

	for (const Group& g : mesh.groups)
	{
		DrawableGroup* currGroup = new DrawableGroup();

		std::vector<float> buffer;
		unsigned int count = 0;

		for (const Face& f : g.faces)
		{
			for (unsigned int i = 0; i < f.verts.size(); ++i)
			{
				count++;

				buffer.push_back(mesh.vertices[f.verts[i]].x);
				buffer.push_back(mesh.vertices[f.verts[i]].y);
				buffer.push_back(mesh.vertices[f.verts[i]].z);
				buffer.push_back(mesh.normals[f.norms[i]].x);
				buffer.push_back(mesh.normals[f.norms[i]].y);
				buffer.push_back(mesh.normals[f.norms[i]].z);
				if (hasTexCoord)
				{
					buffer.push_back(mesh.textureCoords[f.texts[i]].x);
					buffer.push_back(mesh.textureCoords[f.texts[i]].y);
				}
			}
		}

		currGroup->buffer.BufferData(&buffer[0], buffer.size() * sizeof(float));
		currGroup->layout.AddElement(LayoutElement(3, GL_FLOAT, ATTRIB_VERTEX_POS));
		currGroup->layout.AddElement(LayoutElement(3, GL_FLOAT, ATTRIB_NORMAL));
		if (hasTexCoord)
			currGroup->layout.AddElement(LayoutElement(2, GL_FLOAT, ATTRIB_TEX_COORD));
		currGroup->va.AddBuffers(currGroup->buffer, currGroup->layout);
		currGroup->count = count;

		currGroup->material = *mesh.mtllib.GetMaterial(g.material);

		res->groups.push_back(currGroup);
	}

	return res;
}
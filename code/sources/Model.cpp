

#define TINYOBJLOADER_IMPLEMENTATION

#include "Model.hpp"

#include <tiny_obj_loader.h>

using namespace std;
using namespace tinyobj;

namespace przurro
{
	Model::Model(const String & modelFilePath)
	{
		attrib_t             attributes;
		vector< shape_t    > shapes;
		vector< material_t > materials;

		// Attempt to load an object

		if (!tinyobj::LoadObj(&attributes, &shapes, &materials, &error, modelFilePath.c_str()) || !error.empty())
		{
			return;
		}

		// Checking if the data is valid

		if (shapes.size() == 0) { error = String("There're no shapes in ") + modelFilePath; return; }
		if (attributes.vertices.size() == 0) { error = String("There're no vertices in ") + modelFilePath; return; }
		if (attributes.normals.size() == 0) { error = String("There're no normals in ") + modelFilePath; return; }

		// Creation of the meshes that compose the model:

		for (auto & shape : shapes)
		{
			const vector< index_t > & indices = shape.mesh.indices;
			const size_t             nIndices = (size_t)indices.size();

			if (nIndices > 0)
			{
				// Se fusionan los índices de coordenadas y de normales y se ordenan secuencialmente los vértices:

				const size_t   vertices_count = nIndices;

				vector< float > vertex_components(vertices_count * 3);
				vector< float > normal_components(vertices_count * 3);

				for (size_t src = 0, dst = 0; src < nIndices; ++src, dst += 3)
				{
					int vertex_src = indices[src].vertex_index * 3;
					int normal_src = indices[src].normal_index * 3;

					vertex_components[dst + 0] = attributes.vertices[vertex_src + 0];
					vertex_components[dst + 1] = attributes.vertices[vertex_src + 1];
					vertex_components[dst + 2] = attributes.vertices[vertex_src + 2];

					normal_components[dst + 0] = attributes.normals[normal_src + 0];
					normal_components[dst + 1] = attributes.normals[normal_src + 1];
					normal_components[dst + 2] = attributes.normals[normal_src + 2];
				}

				//// Se crean los buffers de atributos de vértices:

				//shared_ptr< Vertex_Buffer_Object > vbo_coordinates
				//(
				//	new Vertex_Buffer_Object(vertex_components.data(), vertex_components.size() * sizeof(float))
				//);

				//shared_ptr< Vertex_Buffer_Object > vbo_normals
				//(
				//	new Vertex_Buffer_Object(normal_components.data(), normal_components.size() * sizeof(float))
				//);

				//// Se crea una mesh a partir de la shape de tinyobj:

				//shared_ptr< Model > mesh(new Mesh);

				//shared_ptr< Vertex_Array_Object > vao
				//(
				//	new Vertex_Array_Object
				//	(
				//		{
				//			{ vbo_coordinates, Model::Vertex_Attribute::COORDINATES, 3, GL_FLOAT },
				//			{ vbo_normals,     Model::Vertex_Attribute::NORMALS,     3, GL_FLOAT }
				//		}
				//	)
				//);

				//mesh->set_vao(vao);
				//mesh->set_primitive_type(GL_TRIANGLES);
				//mesh->set_vertices_count(nIndices);

				//// Se añade la nueva mesh al modelo:

				//add(mesh, Material::default_material());
			}
		}
	}
	void Model::update(Projection_Matrix3f & projectionM)
	{
		for (Model_sptr & model : meshes)
		{
			model->update(projectionM);
		}
	}

	void Model::draw(Rasterizer<Color_Buff> & rasterizer)
	{
		for (Model_sptr & model : meshes)
		{
			model->draw(rasterizer);
		}
	}
}
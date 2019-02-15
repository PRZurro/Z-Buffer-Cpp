

#define TINYOBJLOADER_IMPLEMENTATION

#include "Model.hpp"
#include "Mesh.hpp"

#include <tiny_obj_loader.h>

namespace przurro
{
	Model::Model(const String & assetFolderPath, const String & assetName)
		: name(assetName)
	{
		Attrib_t             attributes;
		std::vector< Shape_t   > shapes;
		std::vector< Material_t > materials;

		String path = assetFolderPath + assetName;

		// Attempt to load an object

		if (!tinyobj::LoadObj(&attributes, &shapes, &materials, &error, path.c_str()) || !error.empty())
		{
			return;
		}

		// Checking if the data is valid

		size_t vertexComponentsN = (size_t)attributes.vertices.size();
		size_t normalComponentsN = (size_t)attributes.normals.size();
		size_t shapesN = (size_t)shapes.size();

		if (shapesN == 0) { error = String("There're no shapes in ") + path; return; }
		if (vertexComponentsN == 0) { error = String("There're no vertices in ") + path; return; }
		if (shapesN == 0) { error = String("There're no normals in ") + path; return; }

		//Here is loaded the vertex and normal arrays 

		for (Shape_t & shape : shapes)
		{
			const std::vector< Index_t > & indices = shape.mesh.indices;
			const size_t             nIndices = (size_t)indices.size();

			for (size_t i = 0; i < nIndices; i += 3)
			{
				originalVertices.push_back
				(
					Point4f
					({
						attributes.vertices[i + X],
						attributes.vertices[i + Y],
						attributes.vertices[i + Z],
						1.f
						})
				);
			}
			for (size_t i = 0; i < normalComponentsN; i += 3)
			{
				originalVertices.push_back
				(
					Vector4f
					({
						attributes.normals[i + X],
						attributes.normals[i + Y],
						attributes.normals[i + Z],
						0.f
						})
				);
			}
		}
		// Creation of the meshes that compose the model:

		for (auto & shape : shapes)
		{
			const std::vector< Index_t > & indices = shape.mesh.indices;
			const size_t             nIndices = (size_t)indices.size();

			if (nIndices > 0)
			{
				// Se fusionan los índices de coordenadas y de normales y se ordenan secuencialmente los vértices:

				const size_t   vertices_count = nIndices;

				f_Buffer vertex_components(vertices_count * 3);
				f_Buffer normal_components(vertices_count * 3);

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

				// Se crean los buffers de atributos de vértices:

				// Se añade la nueva mesh al modelo:

				// Loop over faces(polygon)
				size_t index_offset = 0;
				for (size_t f = 0; f < shape.mesh.num_face_vertices.size(); f++) {
					int fv = shape.mesh.num_face_vertices[f];

					// Loop over vertices in the face.
					for (size_t v = 0; v < fv; v++) {
						// access to vertex

						Point4f vertex;
						Vector4f normal;

						Index_t idx = shape.mesh.indices[index_offset + v];

						for (int i = 0; i < 3; ++i)
						{
							vertex[i] = inputAttributes.vertices[3 * idx.vertex_index + i];
							normal[i] = inputAttributes.vertices[3 * idx.normal_index + i];
						}

						vertex[3] = 1.f;
						normal[3] = 0.f;

						originalVertices.push_back(vertex);
						ovNormals.push_back(normal);
						//attributes->originalIndices.push_back(shape.mesh.indices[index_offset + v]); // ??????
					}
					index_offset += fv;
				}

					// Se crean los buffers de atributos de vértices:

			}
		}
	}
	void Model::update(Camera * activeCamera)
	{
		rotation += constantRotation;

		Translation_Matrix3f	translation(position);
		Rotation_Matrix3f		rotationX, rotationY, rotationZ;
		Scale_Matrix3f			scaling(scale);

		// Creación de la matriz de transformación unificada:

		rotationX.set< Rotation_Matrix3f::AROUND_THE_X_AXIS >(rotation[X]);
		rotationY.set< Rotation_Matrix3f::AROUND_THE_Y_AXIS >(rotation[Y]);
		rotationZ.set< Rotation_Matrix3f::AROUND_THE_Z_AXIS >(rotation[Z]);

		localTransform = translation * rotationX * rotationY * rotationZ *  scaling;

		assert(transformParent); //always should exist so this have to be not triggered
		transform = localTransform * (*transformParent);

		//transform global = transform_local * local scale 
		Transform_Matrix3f projectedTransformation = projectionM * transform;

		for (auto & mesh : meshes)
		{
			mesh.second->update(projectedTransformation);
		}
	}

	void Model::draw(Rasterizer<Color_Buff> & rasterizer)
	{
		for (auto & mesh : meshes)
		{
			mesh.second->draw(rasterizer);
		}
	}


	void Model::translate(const Vector3f & translationV)
	{
		position += translationV;
	}

	void Model::set_position(const Vector3f & positionV)
	{
		position = positionV;
	}

	void Model::rotate(const Vector3f & rotationV)
	{
		rotation += rotationV;
	}

	void Model::set_rotation(const Vector3f & rotationV)
	{
		rotation = rotationV;
	}

	void Model::set_constant_rotation(const Vector3f & rotationV)
	{
		constantRotation = rotationV;
	}

	void Model::set_scale(const float & scaleF)
	{
		scale = scaleF;
	}
	bool Model::set_mesh_color(String & meshName, const Vector3f & colorV)
	{
		auto iterator = meshes.find(meshName);

		if (iterator == meshes.end())
			return false;

		meshes[meshName]->set_color(colorV);

		return true;
	}
	void Model::set_local_transform(const Transform_Matrix3f newTransform)
	{
		localTransform = newTransform;
	}
	void Model::set_transform(const Transform_Matrix3f newTransform)
	{
		transform = newTransform;
	}
}
#include "Model.hpp"
#include "Mesh.hpp"
#include "Camera.hpp"

#define TINYOBJLOADER_IMPLEMENTATION
#include <tiny_obj_loader.h>

namespace przurro
{
	Model::Model(const String & assetFolderPath, const String & assetName)
		: name(assetName)
	{
		Attrib_t             attribs;
		std::vector< Shape_t   > shapes;
		std::vector< Material_t > materials;

		String path = assetFolderPath;

		// Attempt to load an object

		if (!tinyobj::LoadObj(&attribs, &shapes, &materials, &error, path.c_str()) || !error.empty())
		{
			return;
		}

		// Checking if the data is valid

		size_t vertexComponentsN = (size_t)attribs.vertices.size();
		size_t normalComponentsN = (size_t)attribs.normals.size();
		size_t shapesN = (size_t)shapes.size();

		if (shapesN == 0) { error = String("There're no shapes in ") + path; return; }
		if (vertexComponentsN == 0) { error = String("There're no vertices in ") + path; return; }
		if (shapesN == 0) { error = String("There're no normals in ") + path; return; }

		//Here is loaded the vertex and normal arrays 

		size_t verticesProcessedN = 0, indicesProcessedN;
		for (Shape_t & shape : shapes)
		{
			const std::vector< Index_t > &	indices = shape.mesh.indices;
			const size_t					nIndices = (size_t) indices.size();

			Mesh_sptr tempMesh(new Mesh(ovPositions, ovNormals, indices.size(), shape.name));
			i_Buffer & tempMeshIndices = tempMesh->get_original_indices();
			tempMeshIndices.resize(nIndices);

			// We loop through the faces index array 
			indicesProcessedN = 0;
			for (const Index_t & index : indices)
			{
				tempMeshIndices[indicesProcessedN] = verticesProcessedN;  // 

				size_t vertexIndex = 3 * index.vertex_index; // Storing the index of the first component of the vertex (X)
				size_t normalIndex = 3 * index.normal_index; // Storing the index of the first component of the normal (X)

				ovPositions.push_back
				(	
					Point4f
					({
						attribs.vertices[vertexIndex + X],
						attribs.vertices[vertexIndex + Y],
						attribs.vertices[vertexIndex + Z],
						1.f
					})
				);
				ovNormals.push_back
				(
					Vector4f
					({
						attribs.normals[normalIndex + X],
						attribs.normals[normalIndex + Y],
						attribs.normals[normalIndex + Z],
						0.f
					})
				);
				++indicesProcessedN;
			}
			++verticesProcessedN;
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
		globalTransform = localTransform * (*transformParent);

		//transform global = transform_local * local scale 
		Transform_Matrix3f projectedTransformation = activeCamera->get_projection_matrix() * globalTransform;

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

	bool Model::set_mesh_color(String & meshName, const Vector4f & colorV)
	{
		auto iterator = meshes.find(meshName);

		if (iterator == meshes.end())
			return false;

		meshes[meshName]->set_color(colorV);

		return true;
	}
	void Model::set_default_color(const Vector4f & colorRGB)
	{
		defaultColor.set(colorRGB[X], colorRGB[Y], colorRGB[Z]);
	}
}
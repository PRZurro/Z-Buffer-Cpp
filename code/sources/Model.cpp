#include "Model.hpp"
#include "Mesh.hpp"
#include "Camera.hpp"

#define TINYOBJLOADER_IMPLEMENTATION
#include <tiny_obj_loader.h>
#include <glm.hpp>
#include <iostream>

namespace przurro
{
	Model::Model(const String & assetFolderPath, const String & assetName)
		: Scene_Object(assetName),
		defaultColor({0, 0, 0})
	{
		Attrib_t					attribs;
		std::vector< Shape_t   >	shapes;
		std::vector< Material_t >	materials;

		// Attempt to load an object

		if (!tinyobj::LoadObj(&attribs, &shapes, &materials, &error, assetFolderPath.c_str()) || !error.empty())
		{
			return;
		}

		// Checking if the data is valid
		if ((size_t)shapes.size() == 0) { error = String("There're no shapes in ") + assetFolderPath; return; }
		if ((size_t)attribs.vertices.size() == 0) { error = String("There're no vertices in ") + assetFolderPath; return; }
		if ((size_t)attribs.normals.size() == 0) { error = String("There're no normals in ") + assetFolderPath; return; }

		//Here is loaded the vertex and normal arrays 
		size_t indicesN = 0;
		for (Shape_t & shape : shapes)
		{
			indicesN += (size_t)shape.mesh.indices.size();
		}

		ovPositions.resize(indicesN);
		ovNormals.resize(indicesN);

		size_t verticesProcessedN = 0;
		for (Shape_t & shape : shapes)
		{
			const std::vector< Index_t > &	indices = shape.mesh.indices;
			const size_t					indicesMeshN = (size_t) indices.size();

			Mesh_sptr tempMesh(new Mesh(ovPositions, ovNormals, indicesMeshN, verticesProcessedN, shape.name));
			i_Buffer & tempMeshIndices = tempMesh->get_original_indices();

			// We loop through the faces index array 
			for (size_t index = 0; index < indicesMeshN; ++index)
			{
				tempMeshIndices[index] = verticesProcessedN;

				size_t vertexIndex = 3 * indices[index].vertex_index; // Storing the index of the first component of the vertex (X)
				size_t normalIndex = 3 * indices[index].normal_index; // Storing the index of the first component of the normal (X)

				ovPositions[verticesProcessedN] = Point4f
				({
					attribs.vertices[vertexIndex + X],
					attribs.vertices[vertexIndex + Y],
					attribs.vertices[vertexIndex + Z],
					1.f
				});

				ovNormals[verticesProcessedN++] = Vector4f
				({
					attribs.normals[normalIndex + X],
					attribs.normals[normalIndex + Y],
					attribs.normals[normalIndex + Z],
					0.f
				});
			}

			meshes[shape.name] = tempMesh;
		}
	}

	void Model::update(Camera * activeCamera, Light * inputLight)
	{
		//transform global = transform_local * local scale 

		Matrix44f cameraMatrix = activeCamera->look_at(); // get the camera coordinates

		Vector4f lightV = cameraMatrix * Matrix41f(inputLight->get_direction()); // cache:  calculate the light vector from the camera coordinates
		vec4 lightglmV = normalize(vec4(lightV[X], lightV[Y], lightV[Z], lightV[W])); 

		cameraMatrix = cameraMatrix * Matrix44f(globalTransform); // cache: get the model camera coordinates
		Matrix44f projectedTransformation = Matrix44f(activeCamera->get_projection_matrix()) * cameraMatrix; // cache: pre calculate the projected transformation

		if (inputLight)
		{
			for (auto & mesh : meshes)
				mesh.second->update(cameraMatrix, projectedTransformation, lightglmV, inputLight->get_intensity());
		}
		else
		{
			for (auto & mesh : meshes)
				mesh.second->update(cameraMatrix, projectedTransformation, lightglmV);
		}
			
	}

	void Model::draw(Rasterizer<Color_Buff> & rasterizer)
	{
		for (auto & mesh : meshes)
			mesh.second->draw(rasterizer);
	}

	bool Model::set_mesh_color(String & meshName, const Vector4i & colorV)
	{
		auto iterator = meshes.find(meshName);

		if (iterator == meshes.end())
			return false;

		meshes[meshName]->set_color(colorV);

		return true;
	}
	void Model::set_default_color(const Vector4i & colorRGB)
	{
		defaultColor.set(colorRGB[X], colorRGB[Y], colorRGB[Z]);

		for (auto & meshIterator : meshes)
			meshIterator.second->set_color(defaultColor);
	}
}
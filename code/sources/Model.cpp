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
		: 
		Scene_Object(assetName),
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
		tvPositions.resize(indicesN);
		ovNormals.resize(indicesN);
		tvNormals.resize(indicesN);
		vertexIntensities.resize(indicesN);

		size_t verticesProcessedN = 0;
		for (Shape_t & shape : shapes)
		{
			const std::vector< Index_t > &	indices = shape.mesh.indices;
			const size_t					indicesMeshN = (size_t) indices.size();

			Mesh_sptr tempMesh(new Mesh(ovPositions,tvPositions, ovNormals, tvNormals, indicesMeshN, verticesProcessedN, shape.name));
			// We loop through the faces index array 
			for (size_t index = 0; index < indicesMeshN; ++index)
			{
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

	void Model::update(Camera * activeCamera, Rasterizer<Color_Buff> & rasterizer, Light * inputLight)
	{
		//transform global = transform_local * local scale 
		
		update_vertex_buffers(activeCamera, inputLight);
		
		Vector4f_Buffer cameraFrustrumPlanes = activeCamera->get_frustrum_planes();

		for (auto & mesh : meshes)
		{
			mesh.second->update(vertexIntensities, rasterizer, cameraFrustrumPlanes);
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

	void Model::update_vertex_buffers(Camera * activeCamera, Light * inputLight, float ambientalLightI)
	{
		tvPositions.resize(ovPositions.size());

		Matrix41f lightVector;

		if (!inputLight)
			lightVector = Matrix41f(Vector4f({1.f, -1.f, 0.f, 0.f}));
		else
			lightVector = Matrix41f(inputLight->get_direction());

		Matrix44f cameraCoordinates = activeCamera->look_at(); // get the camera coordinates

		Vector4f lightV = cameraCoordinates * lightVector; // cache:  calculate the light vector from the camera coordinates
		vec4 lightglmV = normalize(vec4(lightV[X], lightV[Y], lightV[Z], lightV[W]));

		Matrix44f modelToCameraTransformation = cameraCoordinates * Matrix44f(globalTransform); // cache: get the model camera coordinates
		Matrix44f modelProjectedTransformation = Matrix44f(activeCamera->get_projection_matrix()) * modelToCameraTransformation; // cache: pre-calculate the projected transformation

		for (size_t i = 0, numberOfIndices = ovPositions.size(); i < numberOfIndices; ++i)
		{
			//---------------------------------------Light calculation----------------------------------------------------
			Vector4f normal = modelToCameraTransformation * Matrix41f(ovNormals[i]); //Calculate the transformed normal vector

			vec4 glmNormal({normal[X], normal[Y], normal[Z], normal[W]});
			float intensity = std::max(dot(lightglmV, glmNormal), 0.f) + ambientalLightI;
			vertexIntensities[i] = std::min(intensity, 1.f);

			tvNormals[i] = normal;

			//---------------------------------------Projected vertex coordinates calculation-----------------------------
			tvPositions[i] = modelProjectedTransformation * Matrix41f(ovPositions[i]); //Calculate the transformed vertex position
		}
	}
}
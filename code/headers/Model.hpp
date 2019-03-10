/**
 * @file Model.hpp
 * @author Pablo Rodríguez Zurro (przuro@gmail.com)
 * @brief 
 * @version 0.1
 * @date 2019-01-27
 * 
 * @copyright Copyright (c) 2019
 * 
 */

#pragma once

#include "declarations/Internal.hpp"
#include "declarations/External_Tools.hpp"
#include "Mesh.hpp"
#include "Light.hpp"

#include "Scene_Object.hpp"
#include "Rasterizer.hpp"

namespace przurro
{
	/**
	 * @brief Class in charge of store the vertices, normals and normals of it's meshes and manage their attributes.
	 */
	class Model : public Scene_Object
	{
	private:

		Mesh_Map meshes;

		Point4f_Buffer		ovPositions; 		//Original vertex positions of all meshes
		Vector4f_Buffer		ovNormals; 			//Original vertices normals of all meshes, 4th component must be set to '0.f'

		f_Buffer			vertexIntensities;	//Vertex intensities 
		Point4f_Buffer		tvPositions; 		//Transformed Vertex Positions
		Vector4f_Buffer		tvNormals;			//Transformed Vertex Normals

		Color defaultColor;
		
	private:

		String error; //Stores the errors occured loading this model

	public:

		/**
		 * @brief Construct a new Model object
		 * 
		 * @param assetFolderPath 
		 * @param assetName 
		 */
		Model(const String & assetFolderPath, const String & assetName);

		/**
		 * @brief Destroy the Model object
		 * 
		 */
		~Model()
		{
			ovPositions.clear();
			tvPositions.clear();
			ovNormals.clear();
			tvNormals.clear();
			
			vertexIntensities.clear();
		}

	public:

		/**
		 * @brief Update the model members
		 * 
		 * @param activeCamera 
		 * @param rasterizer 
		 * @param inputLight 
		 */
		void update(Camera * activeCamera, Rasterizer<Color_Buff> & rasterizer, Light * inputLight);

		/**
		 * @brief Draw the model 
		 * 
		 * @param rasterizer 
		 */
		void draw(Rasterizer<Color_Buff> & rasterizer);
		
	private:

		/**
		 * @brief Updates the transformed vertices buffers by calculating the first phases of the rendering pipeline
		 * 
		 * @param activeCamera 
		 * @param inputLight 
		 * @param ambientalLightI 
		 */
		void update_vertex_buffers(Camera * activeCamera, Light * inputLight, float ambientalLightI = 0.3f);

	public:

		/**
		 * @brief Set the mesh color object
		 * 
		 * @param meshName 
		 * @param colorV 
		 * @return true 
		 * @return false 
		 */
		bool set_mesh_color(String & meshName, const Vector4i & colorV);

		/**
		 * @brief Set the default color object
		 * 
		 * @param colorRGB 
		 */
		void set_default_color(const Vector4i & colorRGB);

	public:
		
		/**
		 * @brief return any failures ocurred
		 * 
		 * @return true 
		 * @return false 
		 */
		bool is_ok()
		{
			return error.empty();
		}

		/**
		 * @brief Get the errors occured
		 * 
		 * @return const String 
		 */
		const String get_error() const
		{
			return error;
		}

		/**
		 * @brief Get the default color object
		 * 
		 * @return const Color 
		 */
		const Color get_default_color()const
		{
			return defaultColor;
		}
	};
}
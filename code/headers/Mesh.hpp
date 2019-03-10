/**
 * @file Mesh.hpp
 * @author Pablo Rodríguez Zurro (przuro@gmail.com)
 * @brief 
 * @version 0.1
 * @date 2019-01-29
 * 
 * @copyright Copyright (c) 2019
 * 
 */

#pragma once

#include "declarations/Internal.hpp"
#include "declarations/Containers.hpp"
#include "declarations/Math.hpp"

#include "Rasterizer.hpp"
#include "Color_Buffer_Rgba8888.hpp"

namespace przurro
{
	/**
	 * @brief Class in charge of store the indices to it's respective vertices, determine which are visible and prepare 
	 * 		  it's visible vertices to be drawn by the rasterizer
	 * 
	 */
	class Mesh
	{
	private:

		Point4f_Buffer  &	ovPositions; 	// Original Vertex Positions 
		Point4f_Buffer	&	tvPositions; 	// Transformed Vertex Positions
		Vector4f_Buffer	&	ovNormals; 		// original vertices normals, 4th component must be set to '0.f'
		Vector4f_Buffer	&	tvNormals; 		// Trasformed Vertex Normals

		CColor_Buff			tvColors; // transformed vertices colors

		TriangleI_Buffer	displayTriangleIndices; // display triangle indices
		Point4i_Buffer		displayVerticesPositions;

		Color				color; // Main color of the vertices

		size_t fIndex, lIndex; //first index and last index
		//std::vector<toolkit::Point2f>			texcoords;  // 'vt'

	private:

		String name;

	private: 

		i_Buffer cacheIndices; //Cache array to store the actual indices to this class buffers. It's necessary to add the 4 position because is required in the fill convex polygon function 

	public:

		/**
		 * @brief Construct a new Mesh object
		 * 
		 * @param vertexBuffer 
		 * @param transformedVertexBuffer 
		 * @param normalBuffer 
		 * @param transformedNormalBuffer 
		 * @param nVertices 
		 * @param meshFIndex 
		 * @param meshName 
		 */
		Mesh
		(
			Point4f_Buffer & vertexBuffer, 
			Point4f_Buffer & transformedVertexBuffer, 
			Vector4f_Buffer & normalBuffer, 
			Vector4f_Buffer & transformedNormalBuffer, 
			size_t nVertices, 
			size_t meshFIndex, String & 
			meshName
		);
		
		/**
		 * @brief Destroy the Mesh object, clearing it's buffers
		 * 
		 */
		~Mesh()
		{
			displayTriangleIndices.clear();
			displayVerticesPositions.clear();

			tvPositions.clear();
			tvNormals.clear();
			tvColors.clear();

			cacheIndices.clear();
		}

	public:

		/**
		 * @brief Update the vertex buffers and prepare it's buffers to be drawn in the rasterizer. There it is calculated
		 * 		  the last phases of the rendering pipeline (Light calculation, Culling, Clipping with frustum, NDC and Viewport Coordinates)
		 * 
		 * @param lightIntensities 
		 * @param rasterizer 
		 * @param fPlanes 
		 */
		void update(f_Buffer & lightIntensities, Rasterizer<Color_Buff> & rasterizer, Vector4f_Buffer & fPlanes);

		/**
		 * @brief 
		 * 
		 * @param rasterizer 
		 */
		void draw(Rasterizer<Color_Buff> & rasterizer);

	public:

		/**
		 * @brief Set the mesh color
		 * 
		 * @param colorV 
		 */
		void set_color(const Vector4i & colorV)
		{
			color.set_indices(colorV[X], colorV[Y], colorV[Z]);
		}

		/**
		 * @brief Set the mesh color
		 * 
		 * @param inputColor 
		 */
		void set_color(const Color & inputColor)
		{
			color = inputColor;
		}

	private:

		/**
		 * @brief Auxiliar method to multiply the mesh color by the input intensity and set it in the input color 
		 * 
		 * @param index 
		 * @param intensity 
		 */
		inline void scale_color(Color * index, float intensity)
		{
			index->set_indices((int)((float)color.data.component.r * intensity), (int)((float)color.data.component.g * intensity), (int)((float)color.data.component.b * intensity));
		}
	};
}
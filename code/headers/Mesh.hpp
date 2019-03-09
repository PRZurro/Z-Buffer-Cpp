#pragma once

#include "declarations/Internal.hpp"
#include "declarations/Containers.hpp"
#include "declarations/Math.hpp"

#include "Rasterizer.hpp"
#include "Color_Buffer_Rgba8888.hpp"

namespace przurro
{
	class Mesh
	{
	private:

		Point4f_Buffer  &	ovPositions;
		Point4f_Buffer	&	tvPositions;
		Vector4f_Buffer	&	ovNormals; // original vertices normals, 4th component must be set to '0.f'
		Vector4f_Buffer	&	tvNormals;

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
		void update(f_Buffer & lightIntensities, Rasterizer<Color_Buff> & rasterizer, Vector4f_Buffer & fPlanes);
		void draw(Rasterizer<Color_Buff> & rasterizer);

	public:

		void set_color(const Vector4i & colorV)
		{
			color.set_indices(colorV[X], colorV[Y], colorV[Z]);
		}

		void set_color(const Color & inputColor)
		{
			color = inputColor;
		}

	private:

		inline void scale_color(Color * index, float intensity)
		{
			index->set_indices((int)((float)color.data.component.r * intensity), (int)((float)color.data.component.g * intensity), (int)((float)color.data.component.b * intensity));
		}
	};
}
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

		TriangleI_Buffer	dti; // display triangle indices
		Point4i_Buffer		displayVertices;

		CColor_Buff			tvColors; // transformed vertices colors

		Color				color; // Main color of the vertices

		size_t fIndex, lIndex; //first index and last index
		//std::vector<toolkit::Point2f>			texcoords;  // 'vt'

	private:

		String name;

		int cacheIndices[4];//Cache array to store the actual indices to this class buffers. It's necessary to add the 4 position because is required in the rasterizer
		
	public:

		Mesh(Point4f_Buffer & vertexBuffer, Point4f_Buffer & transformedVertexBuffer, Vector4f_Buffer & normalBuffer, Vector4f_Buffer & transformedNormalBuffer, size_t nVertices, size_t meshFIndex, String & meshName);
		
		~Mesh()
		{
			dti.clear();

			tvPositions.clear();
			tvNormals.clear();
			tvColors.clear();
		}

	public:
		void update(f_Buffer & lightIntensities, Rasterizer<Color_Buff> & rasterizer, Vector4f_Buffer & frustrumPlanes);
		void draw(Rasterizer<Color_Buff> & rasterizer);

	public:

		bool is_frontface(const Point4f * const projected_vertices, const int * const indices);

		void set_color(const Vector4i & colorV)
		{
			color.set(colorV[X], colorV[Y], colorV[Z]);
		}

		void set_color(const Color & inputColor)
		{
			color = inputColor;
		}


	public: // Clipping

		int clip_with_viewport_3D(const Point4f * vertices, int * first_index, int * last_index, Point4f_Buffer clipped_vertices, const Vector4f_Buffer & frustrumPlanes);
		int clip_with_plane_3D(const Point4f * vertices, int * first_index, int * last_index, Point4f_Buffer clipped_vertices, const Vector4f & plane);
		Point4f intersect_plane(const Vector4f & plane, const Point4f & point0, const Point4f & point1);

	private:

		inline void scale_color(Color * index, float intensity)
		{
			index->set((int)((float)color.data.component.r * intensity), (int)((float)color.data.component.g * intensity), (int)((float)color.data.component.b * intensity));
		}
	};
}
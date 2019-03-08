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

		TriangleI_Buffer	displayTriangleI; // display triangle indices
		Point4i_Buffer		displayVertices;

		CColor_Buff			tvColors; // transformed vertices colors

		Color				color; // Main color of the vertices

		size_t fIndex, lIndex; //first index and last index
		//std::vector<toolkit::Point2f>			texcoords;  // 'vt'

	private:

		String name;

	public:

		Mesh(Point4f_Buffer & vertexBuffer, Point4f_Buffer & transformedVertexBuffer, Vector4f_Buffer & normalBuffer, Vector4f_Buffer & transformedNormalBuffer, size_t nVertices, size_t meshFIndex, String & meshName);
		
		~Mesh()
		{
			displayTriangleI.clear();

			tvPositions.clear();
			tvNormals.clear();
			tvColors.clear();
		}

	public:
		void update(f_Buffer & lightIntensities, Rasterizer<Color_Buff> & rasterizer, Vector4f_Buffer & fPlanes);
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

		int clip_with_viewport(Point4f * vertices, Vector4f_Buffer & fPlanes, Point4f_Buffer & _tvPositions, TriangleI_Buffer & triangles, const size_t index);
		int clip_with_plane(Point4f * vertices, Point4f * outputVertices, int * firstIndex, int * lastIndex, const Vector4f & plane);
		Point4f intersect_plane(const Vector4f & plane, const Point4f & point0, const Point4f & point1);
		void triangulate_polygon(int * firstI, int * lastI, TriangleI_Buffer & triangleIndices);

	private:

		inline void scale_color(Color * index, float intensity)
		{
			index->set((int)((float)color.data.component.r * intensity), (int)((float)color.data.component.g * intensity), (int)((float)color.data.component.b * intensity));
		}
	};
}
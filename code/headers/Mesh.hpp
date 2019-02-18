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
		Vector4f_Buffer	&	ovNormals; // original vertices normals, 4th component must be set to '0.f'
		i_Buffer			ovIndices;

		Point4f_Buffer		tvPositions;
		CColor_Buff			tvColors; // transformed vertices colors

		Point4i_Buffer		displayVertices;

		Color				color; // Main color of the vertices

		//std::vector<toolkit::Point2f>			texcoords;  // 'vt'

	private:

		String name;
		
	public:

		Mesh(Point4f_Buffer & vertexBuffer, Vector4f_Buffer & normalBuffer, size_t nVertex, String & meshName);
		
		~Mesh()
		{
			tvPositions.clear();
			ovIndices.clear();
			displayVertices.clear();
		}

	public:

		void update(Transform_Matrix3f modelTransform);
		void draw(Rasterizer<Color_Buff> & rasterizer);

	public:

		bool is_frontface(const Point4f * const projected_vertices, const int * const indices);

		void set_color(const Vector4f & colorV)
		{
			color.set(colorV[X], colorV[Y], colorV[Z]);
		}


	public: // Clipping

		int Mesh::clip_with_viewport_2d(const Point4f * vertices, int * first_index, int * last_index, Point4f * clipped_vertices);
		int clip_with_line_2d(const Point4f * vertices, int * first_index, int * last_index, Point4f * clipped_vertices, float a, float b, float c);
		Point4f intersect(float a, float b, float c, const Point4f & point0, const Point4f & point1);

	public:

		i_Buffer & get_original_indices()
		{
			return ovIndices;
		}
		
	};
}
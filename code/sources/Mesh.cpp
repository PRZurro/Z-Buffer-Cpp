#include "Mesh.hpp"

namespace przurro
{
	Mesh::Mesh(Point4f_Buffer & vertexBuffer, Point4f_Buffer & transformedVertexBuffer, Vector4f_Buffer & normalBuffer, Vector4f_Buffer & transformedNormalBuffer, size_t nVertices, size_t meshFIndex, String & meshName)
		: name(meshName),
		ovPositions(vertexBuffer), 
		ovNormals(normalBuffer), 
		tvPositions(transformedVertexBuffer),
		tvNormals(transformedNormalBuffer),
		tvColors(nVertices),
		displayVertices(nVertices),
		fIndex(meshFIndex),
		lIndex(fIndex + nVertices - 1),
		color({0, 0, 0})
	{}

	void Mesh::update(f_Buffer & lightIntensities, Rasterizer<Color_Buff> & rasterizer, Vector4f_Buffer & frustrumPlanes)
	{
		Color * vertexColor = tvColors.data();
		for (size_t index = fIndex; index < lIndex; index += 3)
		{
			//---------------------------------------Triangle vertex colors----------------------------------------------
		
			scale_color(vertexColor++, lightIntensities[index + X]);
			scale_color(vertexColor++, lightIntensities[index + Y]);
			scale_color(vertexColor++, lightIntensities[index + Z]);
	
			//---------------------------------------Clipping-------------------------------------------------------------
			
			Point4f_Buffer verticesToClip({ tvPositions[index + X], tvPositions[index + Y], tvPositions[index + Z]});

			clip_with_viewport_3D(tvPositions.data(),)

			for (size_t i = 0; i < verticesToClip.size(); i++)
			{
				if (i < 3)
				{

				}
				else
				{

				}
			}

		}

		displayVertices.resize(tvPositions.size());

		float widthHalf = (float)rasterizer.get_color_buffer().get_width() / 2;
		float heightHalf = (float)rasterizer.get_color_buffer().get_height() / 2;

		Transform_Matrix3f	viewportTransformation = Translation_Matrix3f(widthHalf, heightHalf, 0.f) * Scale_Matrix3f(widthHalf, heightHalf, 100000000.f);

		for (size_t index = 0, nTriangles = dti.size(); index < nTriangles; ++index)
		{
			Point4f & tvp0 = tvPositions[dti[index].v0], & tvp1 = tvPositions[dti[index].v1], & tvp2 = tvPositions[dti[index].v2];

			//---------------------------------------NDC Coordinates------------------------------------------------------
			float oneByW0 = (1.f / tvp0[W]), oneByW1 = (1.f / tvp1[W]), oneByW2 = (1.f / tvp2[W]);

			//Final vertex positions
			Point4f fvp0({ tvp0[X] * oneByW0, tvp0[Y] * oneByW0, tvp0[Z] * oneByW0, 1.f }),
					fvp1({ tvp1[X] * oneByW1, tvp1[Y] * oneByW1, tvp1[Z] * oneByW1, 1.f }),
					fvp2({ tvp2[X] * oneByW2, tvp2[Y] * oneByW2, tvp2[Z] * oneByW2, 1.f });

			//--------------------------------------Viewport Coordinates + Display Vertices Assignation-------------------

			displayVertices[index + X] = Point4i(Matrix44f(viewportTransformation) * Matrix41f(fvp0));
			displayVertices[index + X] = Point4i(Matrix44f(viewportTransformation) * Matrix41f(fvp1));
			displayVertices[index + X] = Point4i(Matrix44f(viewportTransformation) * Matrix41f(fvp2));
		}
	}

	void Mesh::draw(Rasterizer<Color_Buff> & rasterizer)
	{
		// Se borra el frameb�ffer y se dibujan los tri�ngulos:

		rasterizer.clear();

		for (size_t index = fIndex; index < lIndex; index += 3)
		{	
			cacheIndices[X] = index + X; cacheIndices[Y] = index + Y; cacheIndices[Z] = index + Z; cacheIndices[W] = index + W;

			if (is_frontface(tvPositions.data(), cacheIndices))
			{
				rasterizer.set_color(tvColors[index]); //The color of the polygon is established from the color of its first vertex

				rasterizer.fill_convex_polygon_z_buffer(displayVertices.data(), cacheIndices, cacheIndices + W); //The polygon is filled
			}
		}
	}

	bool Mesh::is_frontface(const Point4f * const projected_vertices, const int * const indices)
	{
		const Point4f & v0 = projected_vertices[indices[X]];
		const Point4f & v1 = projected_vertices[indices[Y]];
		const Point4f & v2 = projected_vertices[indices[Z]];

		// Se asumen coordenadas proyectadas y pol�gonos definidos en sentido horario.
		// Se comprueba a qu� lado de la l�nea que pasa por v0 y v1 queda el punto v2:

		return ((v1[0] - v0[0]) * (v2[1] - v0[1]) - (v2[0] - v0[0]) * (v1[1] - v0[1]) > 0.f);
	}

	int Mesh::clip_with_viewport_3D(const Point4f * vertices, int * first_index, int * last_index, Point4f_Buffer clipped_vertices, const Vector4f_Buffer & frustrumPlanes)
	{
		/*Point4f         aux_vertices[20];
		static const int aux_indices[20] = { 0, 1, 2, 3, 4, 5, 6, 7,};

		int count = clip_with_line_2d
		(
			vertices,
			first_index,
			last_index,
			aux_vertices,
			-1,
			0,
			0
		);

		if (count < 3) return count;

		count = clip_with_line_2d(aux_vertices, aux_indices, aux_indices + count, clipped_vertices, ? , ? , ? );

		if (count < 3) return count;

		count = clip_with_line_2d
		(clipped_vertices, aux_indices, aux_indices + count, aux_vertices, ? , ? , ? );

		if (count < 3) return count;

		return clip_with_line_2d
		(
			aux_vertices,
			aux_indices,
			aux_indices + count,
			clipped_vertices,
			? ,
			? ,
			?
		);*/

		return 0;
	}
	
	int Mesh::clip_with_plane_3D(const Point4f * vertices, int * first_index, int * last_index, Point4f_Buffer clipped_vertices, const Vector4f & plane)
	{
		Point4f current_vertex;
		Point4f    next_vertex;

		float a = plane[X], b = plane[Y], c = plane[Z], d = plane[W];

		int clipped_vertex_count = 0;

		for (int * index = first_index; index < last_index; )
		{
			current_vertex = vertices[*(index++)];
			next_vertex = vertices[*index];

			int current_value =
				a * current_vertex[0] + b * current_vertex[1] + c > 0;

			int    next_value =
				a * next_vertex[0] + b * next_vertex[1] + c > 0;

			switch ((current_value << 1) | next_value)
			{
			case 1:		// EL PRIMERO FUERA Y EL SEGUNDO DENTRO
				clipped_vertices[clipped_vertex_count++] = intersect_plane(plane, current_vertex, next_vertex);
				clipped_vertices[clipped_vertex_count++] = next_vertex;
				break;
			case 2:		// EL PRIMERO DENTRO Y EL SEGUNDO FUERA
				clipped_vertices[clipped_vertex_count++] = intersect_plane(plane, current_vertex, next_vertex);
				break;
			case 3:		// LOS DOS DENTRO
				clipped_vertices[clipped_vertex_count++] = next_vertex;
				break;
			}
		}

		return clipped_vertex_count;
	}

	Point4f Mesh::intersect_plane(const Vector4f & plane, const Point4f & point0, const Point4f & point1)
	{
		//plane_n = Vector_Normalise(plane_n);
		float plane_d = -Vector_DotProduct(plane_n, plane_p);
		float ad = Vector_DotProduct(lineStart, plane_n);
		float bd = Vector_DotProduct(lineEnd, plane_n);
		float t = (-plane_d - ad) / (bd - ad);
		vec3d lineStartToEnd = Vector_Sub(lineEnd, lineStart);
		vec3d lineToIntersect = Vector_Mul(lineStartToEnd, t);
		return Vector_Add(lineStart, lineToIntersect);
		return Point4f();
	}
}
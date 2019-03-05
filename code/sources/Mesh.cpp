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
			
			Point4f_Buffer clippedVertices({ tvPositions[index + X], tvPositions[index + Y], tvPositions[index + Z]});
			i_Buffer clippedIndices({(int)index + X, (int)index + Y, (int)index + Z});

			clip_with_viewport_3D(clippedVertices, clippedIndices, frustrumPlanes);

			Triangulate(clippedIndices, dti);
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

		int cacheIndices[4];//Cache array to store the actual indices to this class buffers. It's necessary to add the 4 position because is required in the rasterizer
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

	int Mesh::clip_with_viewport_3D(Point4f_Buffer & clippedVertices, i_Buffer & clippedIndices, const Vector4f_Buffer & frustrumPlanes)
	{
		int count = 0, verticesClipped = 0;
		
		size_t nPlanes = frustrumPlanes.size();

		//Clipping with near and far plane
		for (size_t i = nPlanes - 2; i < nPlanes; ++i)
		{
			verticesClipped = clip_with_plane_3D(clippedVertices, clippedIndices, frustrumPlanes[i]);

			// The triangle is outside of the frustrum
			if (verticesClipped == 0)
				return 0;

			count += verticesClipped;
		}

		verticesClipped = 0;

		//Clipping with left, top, right and bottom planes
		for (size_t i = 0, lastPlane = nPlanes - 2 ; i < lastPlane; ++i)
		{
			count += clip_with_plane_3D(clippedVertices, clippedIndices, frustrumPlanes[i]);
		}


		//Clipping 
		/*int count = clip_with_plane_3D
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

		count = clip_with_plane_3D(aux_vertices, aux_indices, aux_indices + count, clipped_vertices, ? , ? , ? );

		if (count < 3) return count;

		count = clip_with_plane_3D
		(clipped_vertices, aux_indices, aux_indices + count, aux_vertices, ? , ? , ? );

		if (count < 3) return count;

		return clip_with_plane_3D
		(
			aux_vertices,
			aux_indices,
			aux_indices + count,
			clipped_vertices,
			? ,
			? ,
			?
		);*/

		return count;
	}
	
	int Mesh::clip_with_plane_3D(Point4f_Buffer & clippedVertices, i_Buffer & clippedIndices, const Vector4f & plane)
	{
		int clippedVertexN = 0;
		Point4f currentVertex, nextVertex;
		float a = plane[X], b = plane[Y], c = plane[Z], d = plane[W];
		
		for (size_t i = 0, size = clippedVertices.size(); i < size;)
		{
			currentVertex = clippedVertices[i++];
			nextVertex = clippedVertices[i];

			//In which side are on the evaluated vertices?

			int current_value =
				a * currentVertex[0] + b * currentVertex[1] + c * currentVertex[2] + d > 0;

			int next_value =
				a * nextVertex[0] + b * nextVertex[1] + +c * nextVertex[2] + d > 0;

			// Depending of the current sides of the evaluated vertices...
			switch ((current_value << 1) | next_value)
			{
			case 1:		// First outside and second inside
				clippedVertices[clippedVertexN++] = intersect_plane(plane, currentVertex, nextVertex);
				clippedVertices[clippedVertexN++] = nextVertex;
				break;
			case 2:		// First inside and second outside
				clippedVertices[clippedVertexN++] = intersect_plane(plane, currentVertex, nextVertex);
				break;
			}
		}

		return clippedVertexN;
	}

	Point4f Mesh::intersect_plane(const Vector4f & plane, const Point4f & point0, const Point4f & point1)
	{
		/*Vector3 ba = b - a;
		float nDotA = Vector3::dotProduct(n, a);
		float nDotBA = Vector3::dotProduct(n, ba);

		a + (((d - nDotA) / nDotBA) * ba);
		*/
		return Point4f();
	}

	void Triangulate(i_Buffer & indices, TriangleI_Buffer & triangleIndices)
	{
		for (size_t v0 = 0, tmpv1 = 1, tmpv2 = 2, size = indices.size(); tmpv2 < size; ++tmpv1, ++tmpv2)
		{
			triangleIndices.push_back(Triangle_Index(v0, tmpv1, tmpv2));
		}
	}
}
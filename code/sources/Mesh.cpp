#include "Mesh.hpp"

namespace przurro
{
	Mesh::Mesh(Point4f_Buffer & vertexBuffer, Vector4f_Buffer & normalBuffer, size_t nVertex, String & meshName)
		: name(meshName),
		ovPositions(vertexBuffer), 
		ovNormals(normalBuffer), 
		ovIndices(nVertex),
		tvPositions(nVertex),
		tvNormals(nVertex),
		tvColors(nVertex),
		displayVertices(nVertex),
		color({0, 0, 0})
	{
	}

	void Mesh::update(Matrix44f & cameraMatrix, Matrix44f & projectedTransform, vec4 & lightVector, float lightIntensity, float ambientalIntensity)
	{
		for (size_t index = 0, number_of_vertices = tvPositions.size(); index < number_of_vertices; index += 3)
		{
			cacheIndices[X] = index + X; cacheIndices[Y] = index + Y; cacheIndices[Z] = index + Z; cacheIndices[W] = index + W;

			//---------------------------------------Light calculation----------------------------------------------------
			Vector4f vn0 = cameraMatrix * Matrix41f(ovNormals[ovIndices[index + X]]); //Calculate the transformed normal vector
			Vector4f vn1 = cameraMatrix * Matrix41f(ovNormals[ovIndices[index + Y]]);
			Vector4f vn2 = cameraMatrix * Matrix41f(ovNormals[ovIndices[index + Z]]);

			vec4 glmNormal({ vn0[X], vn0[Y], vn0[Z], vn0[W]});

			float intensity = std::max(dot(lightVector, glmNormal), 0.f) + ambientalIntensity;
			intensity = std::min(intensity, 1.f);
			Color & tColor = tvColors[index];
			tColor.set
			(
				(int)((float)color.data.component.r * intensity), 
				(int)((float)color.data.component.g * intensity), 
				(int)((float)color.data.component.b * intensity)
			);

			vn0[W] = vn1[W] = vn2[W] = 0.f;
			tvNormals[index + X] = vn0; tvNormals[index + Y] = vn1; tvNormals[index + Z] = vn2;

			//---------------------------------------Projected vertex coordinates calculation-----------------------------
			Point4f vp0 = projectedTransform * cameraMatrix * Matrix41f(ovPositions[ovIndices[index + X]]); //Calculate the transformed vertex position
			Point4f vp1 = projectedTransform * cameraMatrix * Matrix41f(ovPositions[ovIndices[index + Y]]);
			Point4f vp2 = projectedTransform * cameraMatrix * Matrix41f(ovPositions[ovIndices[index + Z]]);

			//---------------------------------------Clipping-------------------------------------------------------------



			//---------------------------------------NDC Coordinates------------------------------------------------------
			float oneByW0 = (1.f / vp0[W]), float oneByW1 = (1.f / vp1[W]), float oneByW2 = (1.f / vp2[W]);

			tvPositions[index + X] = Point4f({vp0[X] * oneByW0, vp0[Y] * oneByW0, vp0[Z] * oneByW0, 1.f}); 
			tvPositions[index + Y] = Point4f({vp1[X] * oneByW1, vp1[Y] * oneByW1, vp1[Z] * oneByW1, 1.f});
			tvPositions[index + Z] = Point4f({vp2[X] * oneByW2, vp2[Y] * oneByW2, vp2[Z] * oneByW2, 1.f});

			
		}
	}

	void Mesh::draw(Rasterizer<Color_Buff> & rasterizer)
	{
		/*Se convierten las coordenadas transformadas y proyectadas a coordenadas
			de recorte (-1 a +1) en coordenadas de pantalla con el origen centrado.
			La coordenada Z se escala a un valor suficientemente grande dentro del
			rango de int (que es lo que espera fill_convex_polygon_z_buffer).*/

		float widthHalf = (float)rasterizer.get_color_buffer().get_width() / 2;
		float heightHalf = (float)rasterizer.get_color_buffer().get_height() / 2;

		Scale_Matrix3f			scaling = Scale_Matrix3f(widthHalf, heightHalf, 100000000.f);
		Translation_Matrix3f    translation = Translation_Matrix3f(widthHalf, heightHalf, 0.f);
		Transform_Matrix3f		transformation = translation * scaling;

		// Se borra el frameb�ffer y se dibujan los tri�ngulos:

		for (size_t index = 0, number_of_vertices = tvPositions.size(); index < number_of_vertices; index += 3)
		{
			Point4i & d0 = displayVertices[index + X] = Point4i(Matrix44f(transformation) * Matrix41f(tvPositions[index + X]));
			Point4i & d1 = displayVertices[index + Y] = Point4i(Matrix44f(transformation) * Matrix41f(tvPositions[index + Y]));
			Point4i & d2 = displayVertices[index + Z] = Point4i(Matrix44f(transformation) * Matrix41f(tvPositions[index + Z]));
		}
		
		rasterizer.clear();

		for (size_t index = 0, number_of_vertices = tvPositions.size(); index < number_of_vertices; index += 3)
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

	int Mesh::clip_with_viewport_2d(const Point4f * vertices, int * first_index, int * last_index, Point4f * clipped_vertices)
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
	
	int Mesh::clip_with_line_2d(const Point4f * vertices, int * first_index, int * last_index, Point4f * clipped_vertices, float a, float b, float c)
	{
		Point4f current_vertex;
		Point4f    next_vertex;

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
				clipped_vertices[clipped_vertex_count++] = intersect_rect(a, b, c, current_vertex, next_vertex);
				clipped_vertices[clipped_vertex_count++] = next_vertex;
				break;
			case 2:		// EL PRIMERO DENTRO Y EL SEGUNDO FUERA
				clipped_vertices[clipped_vertex_count++] = intersect_rect(a, b, c, current_vertex, next_vertex);
				break;
			case 3:		// LOS DOS DENTRO
				clipped_vertices[clipped_vertex_count++] = next_vertex;
				break;
			}
		}

		return clipped_vertex_count;
	}

	Point4f Mesh::intersect_rect(float a, float b, float c, const Point4f & point0, const Point4f & point1)
	{
		/*plane_n = Vector_Normalise(plane_n);
		float plane_d = -Vector_DotProduct(plane_n, plane_p);
		float ad = Vector_DotProduct(lineStart, plane_n);
		float bd = Vector_DotProduct(lineEnd, plane_n);
		float t = (-plane_d - ad) / (bd - ad);
		vec3d lineStartToEnd = Vector_Sub(lineEnd, lineStart);
		vec3d lineToIntersect = Vector_Mul(lineStartToEnd, t);
		return Vector_Add(lineStart, lineToIntersect);*/
		return Point4f();
	}
}
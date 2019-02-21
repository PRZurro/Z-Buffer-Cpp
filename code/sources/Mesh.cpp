#include "Mesh.hpp"

namespace przurro
{
	Mesh::Mesh(Point4f_Buffer & vertexBuffer, Vector4f_Buffer & normalBuffer, size_t nVertex, String & meshName)
		: name(meshName), ovPositions(vertexBuffer), ovNormals(normalBuffer)
	{
	}

	void Mesh::initialize()
	{
		tvPositions.resize(ovIndices.size());
		tvColors.resize(ovIndices.size());
	}

	void Mesh::update(Transform_Matrix3f modelTransform)
	{
		for (size_t index = 0, number_of_vertices = tvPositions.size(); index < number_of_vertices; index++)
		{
			// Se multiplican todos los vértices originales con la matriz de transformación y
			// se guarda el resultado en otro vertex buffer:
			
			Point4f vertexPos = Matrix44f(modelTransform) * Matrix41f(ovPositions[index]);

			// La matriz de proyección en perspectiva hace que el último componente del vector
			// transformado no tenga valor 1.0, por lo que hay que normalizarlo dividiendo:

			float divisor = 1.f / vertexPos[W];

			vertexPos[X] *= divisor;
			vertexPos[Y] *= divisor;
			vertexPos[Z] *= divisor;
			vertexPos[W] = 1.f;
		}
	}

	void Mesh::draw(Rasterizer<Color_Buff> & rasterizer)
	{
		// Se convierten las coordenadas transformadas y proyectadas a coordenadas
			// de recorte (-1 a +1) en coordenadas de pantalla con el origen centrado.
			// La coordenada Z se escala a un valor suficientemente grande dentro del
			// rango de int (que es lo que espera fill_convex_polygon_z_buffer).

		float widthHalf = size_t(rasterizer.get_color_buffer().get_width()) / 2.f;
		float heightHalf = size_t(rasterizer.get_color_buffer().get_height()) / 2.f;

		Scale_Matrix3f			scaling = Scale_Matrix3f(widthHalf, heightHalf, 100000000.f);
		Translation_Matrix3f    translation = Translation_Matrix3f(widthHalf, heightHalf, 0.f);
		Transform_Matrix3f		transformation = translation * scaling;

		for (size_t index = 0, number_of_vertices = tvPositions.size(); index < number_of_vertices; index++)
		{
			displayVertices[index] = Point4i(Matrix44f(transformation) * Matrix41f(tvPositions[index]));
		}

		// Se borra el framebúffer y se dibujan los triángulos:

		rasterizer.clear();

		for (int * indices = ovIndices.data(), *end = indices + ovIndices.size(); indices < end; indices += 3)
		{
			if (is_frontface(tvPositions.data(), indices))
			{
				// Se establece el color del polígono a partir del color de su primer vértice:

				rasterizer.set_color(tvColors[*indices]);

				// Se rellena el polígono:

				rasterizer.fill_convex_polygon_z_buffer(displayVertices.data(), indices, indices + 3);
			}
		}
	}

	bool Mesh::is_frontface(const Point4f * const projected_vertices, const int * const indices)
	{
		const Point4f & v0 = projected_vertices[indices[0]];
		const Point4f & v1 = projected_vertices[indices[1]];
		const Point4f & v2 = projected_vertices[indices[2]];

		// Se asumen coordenadas proyectadas y polígonos definidos en sentido horario.
		// Se comprueba a qué lado de la línea que pasa por v0 y v1 queda el punto v2:

		return ((v1[0] - v0[0]) * (v2[1] - v0[1]) - (v2[0] - v0[0]) * (v1[1] - v0[1]) > 0.f);
	}

	int Mesh::clip_with_viewport_2d(const Point4f * vertices, int * first_index, int * last_index, Point4f * clipped_vertices)
	{
		/*Point4f         aux_vertices[20];
		static const int aux_indices[20] = { 0, 1, 2, 3, 4, 5, 6, 7, ... };

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

		count = clip_with_line_2d(aux_vertices,aux_indices,aux_indices + count,clipped_vertices,? ,? ,?);

		if (count < 3) return count;

		count = clip_with_line_2d
		(clipped_vertices,aux_indices,aux_indices + count,aux_vertices, ? ,? ,?);

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

		return 0; // borrar
	}
	
	int Mesh::clip_with_line_2d(const Point4f * vertices, int * first_index, int * last_index, Point4f * clipped_vertices, float a, float b, float c)
	{
		Point4f current_vertex;
		Point4f    next_vertex;

		int clipped_vertex_count = 9;

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
			case 3:		// LOS DOS DENTRO
				clipped_vertices[clipped_vertex_count++] = next_vertex;
				break;
			case 2:		// EL PRIMERO DENTRO Y EL SEGUNDO FUERA
				clipped_vertices[clipped_vertex_count++] =
					intersect(a, b, c, current_vertex, next_vertex);
				break;
			case 1:		// EL PRIMERO FUERA Y EL SEGUNDO DENTRO
				clipped_vertices[clipped_vertex_count++] =
					intersect(a, b, c, current_vertex, next_vertex);
				clipped_vertices[clipped_vertex_count++] = next_vertex;
				break;
			}
		}

		return clipped_vertex_count;
	}

	Point4f Mesh::intersect(float a, float b, float c, const Point4f & point0, const Point4f & point1)
	{
		return Point4f();
	}
}
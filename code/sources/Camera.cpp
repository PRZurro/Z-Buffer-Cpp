#include "Camera.hpp"

using namespace glm;

namespace przurro
{
	Camera::Camera(float nearPlaneD, float farPlaneD, float fovDegrees, size_t inputWidth, size_t inputHeight, Vector3f * target)
		: Scene_Object("camera-undefined"),
		width(inputWidth), 
		height(inputHeight), 
		projectionMatrix(nearPlaneD, farPlaneD, fovDegrees, width* (1.f/height)),
		viewTarget(target),
		defaultViewDirection({0.f, 0.f, 10000.f})
	{}

	Matrix44f Camera::look_at(const Vector3f & at)
	{
		vec3 tmp = vec3({ 0.f, 1.f, 0.f }); // Y axis vector

		vec3 cameraPos ({position[X], position[Y], position[Z]});
		vec3 target ({at[X], at[Y], at[Z]});

		vec3 forward = normalize(cameraPos - target);
		vec3 right = cross(normalize(tmp), forward);
		vec3 up = cross(forward, right);

		Matrix44f camToWorld = Matrix44f::identity;

		camToWorld[0][0] = right.x; 
		camToWorld[0][1] = right.y;
		camToWorld[0][2] = right.z;
		camToWorld[1][0] = up.x;
		camToWorld[1][1] = up.y;
		camToWorld[1][2] = up.z;
		camToWorld[2][0] = forward.x;
		camToWorld[2][1] = forward.y;
		camToWorld[2][2] = forward.z;

		camToWorld[3][0] = cameraPos.x;
		camToWorld[3][1] = cameraPos.y;
		camToWorld[3][2] = cameraPos.z;

		return camToWorld;
	}
	Vector4f_Buffer Camera::extract_frustrum_planes(bool normalize)
	{
		// As this link refers: https://www.gamedevs.org/uploads/fast-extraction-viewing-frustum-planes-from-world-view-projection-matrix.pdf
		Matrix44f m = projectionMatrix * look_at(); // World space frustrum
		Vector4f_Buffer planes(6); //planes

		//Left clipping plane
		planes[0][0] = m[3][0] + m[0][0];
		planes[0][1] = m[3][2] + m[0][2];
		planes[0][2] = m[3][2] + m[0][2];
		planes[0][3] = m[3][3] + m[0][3];
		// Right clipping plane
		planes[1][0] = m[3][0] - m[0][0];
		planes[1][1] = m[3][1] - m[0][1];
		planes[1][2] = m[3][2] - m[0][2];
		planes[1][3] = m[3][3] - m[0][3];
		// Top clipping plane
		planes[2][0] = m[3][0] - m[1][0];
		planes[2][1] = m[3][1] - m[1][1];
		planes[2][2] = m[3][2] - m[1][2];
		planes[2][3] = m[3][3] - m[1][3];
		// Bottom clipping plane
		planes[3][0] = m[3][0] + m[1][0];
		planes[3][1] = m[3][1] + m[1][1];
		planes[3][2] = m[3][2] + m[1][2];
		planes[3][3] = m[3][3] + m[1][3];
		// Near clipping plane
		planes[4][0] = m[3][0] + m[2][0];
		planes[4][1] = m[3][1] + m[2][1];
		planes[4][2] = m[3][2] + m[2][2];
		planes[4][3] = m[3][3] + m[2][3];
		// Far clipping plane
		planes[5][0] = m[3][0] - m[2][0];
		planes[5][1] = m[3][1] - m[2][1];
		planes[5][2] = m[3][2] - m[2][2];
		planes[5][3] = m[3][3] - m[2][3];

		if (normalize)
		{
			for (size_t i = 0; i < planes.size(); ++i)
			{
				planes[i] = normalizePlane(planes[i]);
			}
		}

		return planes;
	}
}
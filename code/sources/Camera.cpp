#include "Camera.hpp"

using namespace glm;

namespace przurro
{
	Camera::Camera(float nearPlaneD, float farPlaneD, float fovDegrees, size_t inputWidth, size_t inputHeight, Vector3f * target)
		: Scene_Object("camera-undefined"),
		width(inputWidth), 
		height(inputHeight), 
		projectionMatrix(nearPlaneD, farPlaneD, fovDegrees, width* (1/height)),
		viewTarget(target),
		defaultViewDirection({0.f, 3.f, 10000.f})
	{}

	Matrix44f Camera::look_at(const Vector3f & at)
	{
		vec3 tmp = vec3({ 0.f, 1.f, 0.f }); // Y axis vector

		vec3 cameraPos ({position[X], position[Y], position[Z]});
		vec3 target ({at[X], at[Y], at[Z]});

		vec3 forward = normalize(cameraPos - target);
		vec3 right = cross(normalize(tmp), forward);
		vec3 up = cross(forward, right);

		Matrix44f camToWorld;

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
}
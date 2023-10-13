#pragma once
#include "../ew/ewMath/vec3.h"
#include "../ew/ewMath/mat4.h"
#include "../nb/transformations.h"

namespace nb {
	struct Camera {
		ew::Vec3 position; // Camera body position
		ew::Vec3 target; // Position to look at
		float fov; // Vertical field of view in degrees
		float aspectRatio; // Screen width/screen height
		float nearPlane; // Near plane distance (+Z)
		float farPlane; // Far plan distance (+Z)
		bool orthographic; // Perspective or orthographic?
		float orthoSize; // Height of orthographic frustum
		// World -> View
		inline ew::Mat4 ViewMatrix() {
			return LookAt(position, target, ew::Vec3(0, 1, 0));
		};
		// View -> Clip
		inline ew::Mat4 ProjectionMatrix() {
			if (orthographic) {
				return Orthographic(orthoSize, aspectRatio, nearPlane, farPlane);
			}
			else {
				return Perspective(fov, aspectRatio, nearPlane, farPlane);
			}
		};
	};

}
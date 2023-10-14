#pragma once
#include "../ew/ewMath/mat4.h"
#include "../ew/ewMath/vec3.h"
#include "../ew/ewMath/ewMath.h"

namespace nb {
	// Identity matrix
	inline ew::Mat4 Identity() {
		return ew::Mat4(
			1, 0, 0, 0,
			0, 1, 0, 0,
			0, 0, 1, 0,
			0, 0, 0, 1
		);
	};
	// Scale on x, y, z axes
	inline ew::Mat4 Scale(ew::Vec3 s) {
		return ew::Mat4(
			s.x, 0, 0, 0,
			0, s.y, 0, 0,
			0, 0, s.z, 0,
			0, 0, 0, 1
		);
	};
	// Rotation around X axis (pitch) in radians
	inline ew::Mat4 RotateX(float rad) {
		return ew::Mat4(
			1, 0, 0, 0,
			0, cos(rad), -sin(rad), 0,
			0, sin(rad), cos(rad), 0,
			0, 0, 0, 1
		);
	};
	// Rotation around Y axis (yaw) in radians
	inline ew::Mat4 RotateY(float rad) {
		return ew::Mat4(
			cos(rad), 0, sin(rad), 0,
			0, 1, 0, 0,
			-sin(rad), 0, cos(rad), 0,
			0, 0, 0, 1
		);
	};
	// Rotation aroudn Z axis (roll) in radians
	inline ew::Mat4 RotateZ(float rad) {
		return ew::Mat4(
			cos(rad), -sin(rad), 0, 0,
			sin(rad), cos(rad), 0, 0,
			0, 0, 1, 0,
			0, 0, 0, 1
		);
	};
	// Translate x, y, z
	inline ew::Mat4 Translate(ew::Vec3 t) {
		return ew::Mat4(
			1, 0, 0, t.x,
			0, 1, 0, t.y,
			0, 0, 1, t.z,
			0, 0, 0, 1
		);
	};

	// Creates a righthanded view space
	// eye = camera position
	// target = position to look at
	// up = up axis (usually (0,1,0))
	inline ew::Mat4 LookAt(ew::Vec3 eye, ew::Vec3 target, ew::Vec3 up) {
		ew::Vec3 forward = ew::Normalize(eye - target);
		ew::Vec3 right = ew::Normalize(ew::Cross(up, forward));
		ew::Vec3 newUp = ew::Normalize(ew::Cross(forward, right));

		return ew::Mat4(
			right.x,   right.y,   right.z,   -(ew::Dot(right, eye)),
			newUp.x,   newUp.y,   newUp.z,   -(ew::Dot(newUp, eye)),
			forward.x, forward.y, forward.z, -(ew::Dot(forward, eye)),
			0,         0,         0,         1
		);
	};
	// Orthographic projection
	inline ew::Mat4 Orthographic(float height, float aspect, float near, float far) {
		float width = aspect * height;
		float right = width / 2;
		float left = -right;
		float top = height / 2;
		float bot = -top;
		return ew::Mat4(
			2 / (right - left), 0,               0,                   -((right + left) / (right - left)),
			0,                  2 / (top - bot), 0,                   -((top + bot) / (top - bot)),
			0,                  0,               -(2 / (far - near)), -((far + near) / (far - near)),
			0, 0, 0, 1
		);
	};
	// Perspective projection
	inline ew::Mat4 Perspective(float fov, float aspect, float near, float far) {
		float rad = ew::Radians(fov);
		return ew::Mat4(
			1 / (tan(rad / 2) * aspect), 0,                  0,                           0,
			0,                           1 / (tan(rad / 2)), 0,                           0,
			0,                           0,                  (near + far) / (near - far), (2 * far * near) / (near - far),
			0,                           0,                  -1,                          0
		);
	};

	struct Transform {
		ew::Vec3 position = ew::Vec3(0.0f, 0.0f, 0.0f);
		ew::Vec3 rotation = ew::Vec3(0.0f, 0.0f, 0.0f); // Euler angles (degrees)
		ew::Vec3 scale = ew::Vec3(1.0f, 1.0f, 1.0f);
		ew::Mat4 getModelMatrix() const {
			return (Identity() * nb::Translate(position) * nb::RotateY(rotation.y) * nb::RotateX(rotation.x) * nb::RotateZ(rotation.z) * nb::Scale(scale));
		}
	};
}
#include "procGen.h"
#include "../ew/mesh.h"

namespace nb {
	ew::MeshData createSphere(float radius, int numSegments) {
		ew::MeshData mesh;

		// Vertices
		float thetaStep = (2 * ew::PI) / numSegments;
		float phiStep = ew::PI / numSegments;
		for (int row = 0; row <= numSegments; row++) {
			float phi = row * phiStep;
			for (int col = 0; col <= numSegments; col++) {
				float theta = col * thetaStep;
				ew::Vertex v;
				v.pos.x = radius * sin(phi) * sin(theta);
				v.pos.y = radius * cos(phi);
				v.pos.z = radius * sin(phi) * cos(theta);
				mesh.vertices.push_back(v);
			}
		}

		return mesh;
	}

	ew::MeshData createCylinder(float height, float radius, int numSegments) {
		ew::MeshData mesh;

		// Vertices
		float topY = height / 2;
		float botY = -topY;
		
		mesh.vertices.push_back({ {0, topY, 0}, 0, 0}); // Top center
		
		float thetaStep = (2 * ew::PI) / numSegments;
		for (int i = 0; i <= numSegments; i++) {
			float theta = i * thetaStep;
			ew::Vertex v;
			v.pos.x = cos(theta) * radius;
			v.pos.z = sin(theta) * radius;
			v.pos.y = topY;
			mesh.vertices.push_back(v);
		}

		for (int i = 0; i <= numSegments; i++) {
			float theta = i * thetaStep;
			ew::Vertex v;
			v.pos.x = cos(theta) * radius;
			v.pos.z = sin(theta) * radius;
			v.pos.y = botY;
			mesh.vertices.push_back(v);
		}

		mesh.vertices.push_back({ {0, botY, 0}, 0, 0}); // Bottom cneter

		return mesh;
	}

	ew::MeshData createPlane(float width, float height, int subDivisions) {
		ew::MeshData mesh;

		// Vertices
		for (int row = 0; row <= subDivisions; row++) {
			for (int col = 0; col <= subDivisions; col++) {
				ew::Vertex v;
				v.pos.x = width * (col / subDivisions);
				v.pos.y = 0;
				v.pos.z = -height * (row / subDivisions);
				v.normal = ew::Vec3(0, 1, 0);
				v.uv.x = col / subDivisions;
				v.uv.y = row / subDivisions;
				mesh.vertices.push_back(v);
			}
		}

		// Indices
		int columns = subDivisions + 1;
		for (int row = 0; row < subDivisions; row++) {
			for (int col = 0; col < subDivisions; col++) {
				float start = row * columns + col;
				mesh.indices.push_back(start);
				mesh.indices.push_back(start + 1);
				mesh.indices.push_back(start + columns + 1);

				mesh.indices.push_back(start + columns + 1);
				mesh.indices.push_back(start + columns);
				mesh.indices.push_back(start);
			}
		}

		return mesh;
	}
}
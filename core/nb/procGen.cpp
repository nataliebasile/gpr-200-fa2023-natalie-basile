#include "procGen.h"
#include "../ew/mesh.h"

namespace nb {
	ew::MeshData createSphere(float radius, int numSegments) {
		ew::MeshData mesh;

		// Vertices
		float thetaStep = (2 * ew::PI) / (float)numSegments;
		float phiStep = ew::PI / (float)numSegments;
		for (int row = 0; row <= (float)numSegments; row++) {
			float phi = row * phiStep;
			for (int col = 0; col <= (float)numSegments; col++) {
				float theta = col * thetaStep;
				ew::Vertex v;
				v.pos.x = radius * cos(theta) * sin(phi);
				v.pos.y = radius * cos(phi);
				v.pos.z = radius * sin(theta) * sin(phi);
				v.normal = ew::Normalize(v.pos);
				v.uv.x = 1 - (col / (float)numSegments);
				v.uv.y = 1 - (row / (float)numSegments);
				mesh.vertices.push_back(v);
			}
		}

		// Indices
		float poleStart = 0;
		float sideStart = numSegments + 1;
		for (int i = 0; i < numSegments; i++) { // Top cap indices
			mesh.indices.push_back(sideStart + i);
			mesh.indices.push_back(poleStart + i);
			mesh.indices.push_back(sideStart + i + 1);
		}
		float columns = numSegments + 1;
		for (int row = 1; row < numSegments - 1; row++) { // Row indices
			for (int col = 0; col < numSegments; col++) {
				float start = row * columns + col;
				mesh.indices.push_back(start);
				mesh.indices.push_back(start + 1);
				mesh.indices.push_back(start + columns);
				mesh.indices.push_back(start + 1);
				mesh.indices.push_back(start + columns + 1);
				mesh.indices.push_back(start + columns);
			}
		}
		poleStart = mesh.vertices.size() - numSegments - 1;
		sideStart = poleStart - numSegments - 1;
		for (int i = 0; i < numSegments; i++) { // Top cap indices
			mesh.indices.push_back(sideStart + i + 1);
			mesh.indices.push_back(poleStart + i);
			mesh.indices.push_back(sideStart + i);
		}

		return mesh;
	}

	ew::MeshData createCylinder(float height, float radius, int numSegments) {
		ew::MeshData mesh;

		// Vertices
		float topY = height / 2;
		float botY = -topY;
		
		mesh.vertices.push_back({ {0, topY, 0}, {0, 1, 0}, {0.5, 0.5} }); // Top center
		float thetaStep = (2 * ew::PI) / numSegments;
		for (int i = 0; i <= numSegments; i++) { // Top cap ring
			float theta = i * thetaStep;
			ew::Vertex v;
			v.pos.x = cos(theta) * radius;
			v.pos.z = sin(theta) * radius;
			v.pos.y = topY;
			v.normal = ew::Vec3(0, 1, 0);
			v.uv = ew::Vec2(cos(theta), sin(theta));
			v.uv = (v.uv + 1) / 2;
			mesh.vertices.push_back(v);
		}
		for (int i = 0; i <= numSegments; i++) { // Top side ring
			float theta = i * thetaStep;
			ew::Vertex v;
			v.pos.x = cos(theta) * radius;
			v.pos.z = sin(theta) * radius;
			v.pos.y = topY;
			v.normal = ew::Vec3(cos(theta), 0, sin(theta));
			v.uv.x = i / (float)numSegments;
			v.uv.y = 1;
			mesh.vertices.push_back(v);
		}

		for (int i = 0; i <= numSegments; i++) { // Bot side ring
			float theta = i * thetaStep;
			ew::Vertex v;
			v.pos.x = cos(theta) * radius;
			v.pos.z = sin(theta) * radius;
			v.pos.y = botY;
			v.normal = ew::Vec3(cos(theta), 0, sin(theta));
			v.uv.x = i / (float)numSegments;
			v.uv.y = 0;
			mesh.vertices.push_back(v);
		}
		for (int i = 0; i <= numSegments; i++) { // Bot cap ring
			float theta = i * thetaStep;
			ew::Vertex v;
			v.pos.x = cos(theta) * radius;
			v.pos.z = sin(theta) * radius;
			v.pos.y = botY;
			v.normal = ew::Vec3(0, -1, 0);
			v.uv = ew::Vec2(cos(theta), sin(theta));
			mesh.vertices.push_back(v);
		}
		mesh.vertices.push_back({ {0, botY, 0}, 0, 0}); // Bottom center

		// Indices
		float start = 1;
		float center = 0;
		for (int i = 0; i < numSegments; i++) { // Top cap indices
			mesh.indices.push_back(start + i);
			mesh.indices.push_back(center);
			mesh.indices.push_back(start + i + 1);
		}
		float sideStart = numSegments + 1; 
		int columns = numSegments + 1;
		for (int i = 0; i < columns; i++) { // Side indices
			start = sideStart + i;
			mesh.indices.push_back(start);
			mesh.indices.push_back(start + 1);
			mesh.indices.push_back(start + columns);
			mesh.indices.push_back(start + 1);
			mesh.indices.push_back(start + columns + 1);
			mesh.indices.push_back(start + columns);
		}
		center = mesh.vertices.size() - 1;
		start = center - columns;
		for (int i = 0; i < numSegments; i++) { // Bot cap indices
			mesh.indices.push_back(start + i + 1);
			mesh.indices.push_back(center);
			mesh.indices.push_back(start + i);
		}

		return mesh;
	}

	ew::MeshData createPlane(float width, float height, int subDivisions) {
		ew::MeshData mesh;

		// Vertices
		for (int row = 0; row <= subDivisions; row++) {
			for (int col = 0; col <= subDivisions; col++) {
				ew::Vertex v;
				v.pos.x = width * (col / (float)subDivisions);
				v.pos.y = 0;
				v.pos.z = -height * (row / (float)subDivisions);
				v.normal = ew::Vec3(0, 1, 0);
				v.uv.x = col / (float)subDivisions;
				v.uv.y = row / (float)subDivisions;
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
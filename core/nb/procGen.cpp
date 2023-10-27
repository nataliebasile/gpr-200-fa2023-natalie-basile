#include "procGen.h"
#include "../ew/mesh.h"

namespace nb {
	ew::MetaData createSphere(float radius, int numSegments) {

	}

	ew::MetaData createCylinder(float height, float radius, int numSegments) {

	}

	ew::MeshData createPlane(float width, float height, int subdivisions) {
		ew::MeshData mesh;
		for (int row = 0; row <= subdivisions; row++) {
			for (int col = 0; col <= subdivisions; col++) {
				ew::Vertex v;
				v.x = width * (col / subdivisions);
				v.y = 0;
				v.z = -height * (row / subdivisions);
				vertices.push_back(v);
			}
		}
	}
}
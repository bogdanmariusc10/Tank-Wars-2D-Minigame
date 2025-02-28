#pragma once

#include <string>

#include "core/gpu/mesh.h"
#include "utils/glm_utils.h"

using namespace std;


namespace obj2D
{
	Mesh* CreateTriangle(const string& name, glm::vec3 corner1, glm::vec3 corner2, glm::vec3 corner3, glm::vec3 color, bool fill);
	Mesh* CreateTrapezoid(const string& name, glm::vec3 leftBottomCorner, float topLength, float bottomLength, float height, glm::vec3 color, bool fill = false);
	Mesh* CreateUpsideDownTrapezoid(const string& name, glm::vec3 leftBottomCorner, float topLength, float bottomLength, float height, glm::vec3 color, bool fill = false);
	Mesh* CreateArc(const string& name, glm::vec3 center, float radius, float startAngle, float endAngle, glm::vec3 color, int segments);
	Mesh* CreateRectangle(const string& name, glm::vec3 leftBottomCorner, float width, float height, glm::vec3 color, bool fill = false);
	Mesh* CreateLineSegment(const string& name, const glm::vec3& start, const glm::vec3& end, glm::vec3 color);
	Mesh* CreateCircle(const string& name, glm::vec3 center, float radius, glm::vec3 color, int segments, bool fill);
}

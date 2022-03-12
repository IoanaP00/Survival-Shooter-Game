#pragma once

#include <vector>
#include <list>
#include <string>

#include "core/gpu/mesh.h"
#include "utils/glm_utils.h"


namespace object2D
{
    Mesh* CreateSquare(const std::string &name, glm::vec3 leftBottomCorner, float length, glm::vec3 color, bool fill = false);
    Mesh* CreateRectangle(const std::string& name, glm::vec3 leftBottomCorner, float width, float height, glm::vec3 color, bool fill = true);
    Mesh* CreateCircle(const std::string& name, glm::vec3 leftBottomCorner, float radius, glm::vec3 color, glm::vec3 center);

}

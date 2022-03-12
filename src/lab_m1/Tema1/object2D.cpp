#include "object2D.h"

#include <vector>

#include "core/engine.h"
#include "utils/gl_utils.h"

#define PI 3.14159265359

Mesh* object2D::CreateSquare(
    const std::string &name,
    glm::vec3 leftBottomCorner,
    float length,
    glm::vec3 color,
    bool fill)
{
    glm::vec3 corner = leftBottomCorner;

    std::vector<VertexFormat> vertices =
    {
        VertexFormat(corner + glm::vec3(-length/2, -length/2, 0), color),
        VertexFormat(corner + glm::vec3(length/2, -length/2, 0), color),
        VertexFormat(corner + glm::vec3(length/2, length/2, 0), color),
        VertexFormat(corner + glm::vec3(-length/2, length/2, 0), color)
    };

    Mesh* square = new Mesh(name);
    std::vector<unsigned int> indices = { 0, 1, 2, 3 };

    if (!fill) {
        square->SetDrawMode(GL_LINE_LOOP);
    } else {
        indices.push_back(0);
        indices.push_back(2);
    }

    square->InitFromData(vertices, indices);
    return square;
}

Mesh* object2D::CreateRectangle(
    const std::string& name,
    glm::vec3 leftBottomCorner,
    float width,
    float height,
    glm::vec3 color,
    bool fill)
{
    glm::vec3 corner = leftBottomCorner;

    std::vector<VertexFormat> vertices =
    {
        VertexFormat(corner + glm::vec3(-width / 2, -height / 2, 0), color),
        VertexFormat(corner + glm::vec3(width / 2, -height / 2, 0), color),
        VertexFormat(corner + glm::vec3(width / 2, height / 2, 0), color),
        VertexFormat(corner + glm::vec3(-width / 2, height / 2, 0), color)
    };

    Mesh* rectangle = new Mesh(name);
    std::vector<unsigned int> indices = { 0, 1, 2, 3 };

    if (!fill) {
        rectangle->SetDrawMode(GL_LINE_LOOP);
    }
    else {
        indices.push_back(0);
        indices.push_back(2);
    }

    rectangle->InitFromData(vertices, indices);
    return rectangle;
}


Mesh* object2D::CreateCircle(
    const std::string& name, 
    glm::vec3 leftBottomCorner, 
    float radius, 
    glm::vec3 color, 
    glm::vec3 center) {

    std::vector<VertexFormat> vertices;
    std::vector<unsigned int> indices;
    glm::vec3 corner = leftBottomCorner;

    vertices.push_back(VertexFormat(corner + center, color));


    for (int i = 0; i < 50; i++) {
        float x = 2 * PI * i / 50;
        vertices.push_back(VertexFormat(corner + center + glm::vec3(radius * cos(x), radius * sin(x), 0), color));

        indices.push_back(i);
    }
    indices.push_back(50);
    indices.push_back(1);

    Mesh* circle = new Mesh(name);
    circle->InitFromData(vertices, indices);
    return circle;
}
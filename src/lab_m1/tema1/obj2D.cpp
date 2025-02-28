#include "obj2D.h"

#include <vector>

#include "core/engine.h"
#include "utils/gl_utils.h"

using namespace std;

Mesh* obj2D::CreateTriangle(
    const std::string& name,
    glm::vec3 corner1,
    glm::vec3 corner2,
    glm::vec3 corner3,
    glm::vec3 color,
    bool fill)
{
    std::vector<VertexFormat> vertices =
    {
        VertexFormat(corner1, color),
        VertexFormat(corner2, color),
        VertexFormat(corner3, color)
    };

	Mesh* triangle = new Mesh(name);
    std::vector<unsigned int> indices = { 0, 1, 2 };

    if (!fill)
    {
        triangle->SetDrawMode(GL_LINE_LOOP);
    }

    triangle->InitFromData(vertices, indices);

    return triangle;
}

Mesh* obj2D::CreateTrapezoid(
    const std::string& name,
    glm::vec3 bottomLeftCorner,
    float bottomLength,
    float topLength,
    float height,
    glm::vec3 color,
    bool fill)
{
    glm::vec3 corner = bottomLeftCorner;

    float halfDiff = (topLength - bottomLength) / 2.0f;

    vector<VertexFormat> vertices =
    {
        VertexFormat(corner, color),
        VertexFormat(corner + glm::vec3(bottomLength, 0, 0), color),
        VertexFormat(corner + glm::vec3(halfDiff + bottomLength, height, 0), color),
        VertexFormat(corner + glm::vec3(-halfDiff, height, 0), color)
    };

    Mesh* trapezoid = new Mesh(name);
    std::vector<unsigned int> indices = { 0, 1, 2, 3 };

    if (!fill)
    {
        trapezoid->SetDrawMode(GL_LINE_LOOP);
    }
    else
    {
        indices.push_back(0);
        indices.push_back(2);
    }

    trapezoid->InitFromData(vertices, indices);
    return trapezoid;
}

Mesh* obj2D::CreateUpsideDownTrapezoid(
    const std::string& name,
    glm::vec3 topLeftCorner,
    float topLength,
    float bottomLength,
    float height,
    glm::vec3 color,
    bool fill)
{
    glm::vec3 corner = topLeftCorner;

    float halfDiff = (bottomLength - topLength) / 2.0f;
    vector<VertexFormat> vertices =
    {
        VertexFormat(corner, color),
        VertexFormat(corner + glm::vec3(topLength, 0, 0), color),
        VertexFormat(corner + glm::vec3(bottomLength - halfDiff, -height, 0), color),
        VertexFormat(corner + glm::vec3(-halfDiff, -height, 0), color)
    };

    Mesh* trapezoid = new Mesh(name);
    std::vector<unsigned int> indices = { 0, 1, 2, 3 };

    if (!fill)
    {
        trapezoid->SetDrawMode(GL_LINE_LOOP);
    }
    else
    {
        indices.push_back(0);
        indices.push_back(2);
    }

    trapezoid->InitFromData(vertices, indices);
    return trapezoid;
}


Mesh* obj2D::CreateArc(
    const std::string& name,
    glm::vec3 center,
    float radius,
    float startAngle,
    float endAngle,
    glm::vec3 color,
    int segments)
{
    std::vector<VertexFormat> vertices;
    std::vector<unsigned int> indices;

    vertices.emplace_back(center, color);

    for (int i = 0; i <= segments; i++)
    {
        float angle = startAngle + i * (endAngle - startAngle) / segments;

        angle += glm::radians(90.0f);

        glm::vec3 point = center + glm::vec3(radius * cos(angle), radius * sin(angle), 0);

        vertices.emplace_back(point, color);

        if (i > 0)
        {
            indices.push_back(0);
            indices.push_back(i);
            indices.push_back(i - 1);
        }
    }

    Mesh* arc = new Mesh(name);
    arc->SetDrawMode(GL_TRIANGLES);
    arc->InitFromData(vertices, indices);

    return arc;
}


Mesh* obj2D::CreateRectangle(
    const std::string& name,
    glm::vec3 bottomLeftCorner,
    float width,
    float height,
    glm::vec3 color,
    bool fill)
{
    glm::vec3 corner = bottomLeftCorner;

    vector<VertexFormat> vertices =
    {
        VertexFormat(corner, color),
        VertexFormat(corner + glm::vec3(width, 0, 0), color),
        VertexFormat(corner + glm::vec3(width, height, 0), color),
        VertexFormat(corner + glm::vec3(0, height, 0), color)
    };

    Mesh* rectangle = new Mesh(name);
    std::vector<unsigned int> indices;

    if (fill)
    {
        indices = { 0, 1, 2, 2, 3, 0 };
        rectangle->SetDrawMode(GL_TRIANGLES);
    }
    else
    {
        indices = { 0, 1, 2, 3 };
        rectangle->SetDrawMode(GL_LINE_LOOP);
    }

    rectangle->InitFromData(vertices, indices);
    return rectangle;
}

Mesh* obj2D::CreateLineSegment(
    const std::string& name,
    const glm::vec3& start,
    const glm::vec3& end,
    glm::vec3 color)
{
    std::vector<VertexFormat> vertices;
    std::vector<unsigned int> indices;

    vertices.emplace_back(start, color);
    vertices.emplace_back(end, color);

    indices.push_back(0);
    indices.push_back(1);

    Mesh* line = new Mesh(name);
    line->SetDrawMode(GL_LINES);
    line->InitFromData(vertices, indices);

    return line;
}

Mesh* obj2D::CreateCircle(const std::string& name, glm::vec3 center, float radius, glm::vec3 color, int segments, bool fill)
{
    std::vector<VertexFormat> vertices;
    std::vector<unsigned int> indices;

    vertices.emplace_back(center, color);

    for (int i = 0; i <= segments; ++i) {
        float angle = i * 2.0f * glm::pi<float>() / segments;
        glm::vec3 point = center + glm::vec3(radius * cos(angle), radius * sin(angle), 0);
        vertices.emplace_back(point, color);
        if (i > 0)
        {
            indices.push_back(0);
            indices.push_back(i);
            indices.push_back(i - 1);
        }
    }

    Mesh* circle = new Mesh(name);
    circle->SetDrawMode(GL_TRIANGLE_FAN);
    circle->InitFromData(vertices, indices);

    return circle;
}

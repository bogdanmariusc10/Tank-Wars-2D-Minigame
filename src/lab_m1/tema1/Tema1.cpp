#include "lab_m1/tema1/Tema1.h"
#include <vector>
#include <iostream>
#include <algorithm>
#include <cmath>
#include "lab_m1/tema1/obj2D.h"

using namespace std;
using namespace m1;

Tema1::Tema1()
{
}

Tema1::~Tema1()
{
}

void Tema1::Init()
{
    glm::ivec2 resolution = window->GetResolution();
    auto camera = GetSceneCamera();
    camera->SetOrthographic(0, (float)resolution.x, 0, (float)resolution.y, 0.01f, 400);
    camera->SetPosition(glm::vec3(0, 0, 50));
    camera->SetRotation(glm::vec3(0, 0, 0));
    camera->Update();
    GetCameraInput()->SetActive(false);

    terrainWidth = 1280;
    spacing = 5;

    tank1PosX = 100.0f;
    tank1PosY = 0.0f;
    tank2PosX = 1150.0f;
    tank2PosY = 0.0f;
    tank1Name = "Tank1";
    tank2Name = "Tank2";
    turret1Rotation = 0.0f;
    turret2Rotation = 0.0f;

    tank1BottomColor = glm::vec3(0.4f, 0.1f, 0.6f);
    tank1TopColor = glm::vec3(0.6f, 0.3f, 0.8f);
    tank1TurretColor = glm::vec3(0.3f, 0.0f, 0.5f);
    tank2BottomColor = glm::vec3(0.6f, 0.1f, 0.1f);
    tank2TopColor = glm::vec3(0.8f, 0.3f, 0.3f);
    tank2TurretColor = glm::vec3(0.5f, 0.0f, 0.0f);

    tankBottomTrapBottomBaseLength = 50.0f;
    tankBottomTrapTopBaseLength = 60.0f;
    tankBottomTrapHeight = 10.0f;
    tankTopTrapBottomBaseLength = 90.0f;
    tankTopTrapTopBaseLength = 65.0f;
    tankTopTrapHeight = 20.0f;
    turretRadius1 = 18.0f;
    startAngle1 = -glm::pi<float>() / 2.0f;
    endAngle1 = glm::pi<float>() / 2.0f;
    barrelWidth1 = 7.0f;
    barrelHeight1 = 28.0f;
    turretOffsetX1 = 5.0f;
    turretOffsetY1 = -1.0f;
    barrelOffsetX1 = 4.5f;
    barrelOffsetY1 = turretOffsetY1 + turretRadius1 - 2.5f;

    tank1Health = 100.0f;
    tank2Health = 100.0f;
    tankCollisionRadius = 50.0f;

    clouds.push_back({ 200.0f, 600.0f, 1.5f, 50.0f });
    clouds.push_back({ 150.0f, 650.0f, 1.5f, 50.0f });
    clouds.push_back({ 100.0f, 600.0f, 1.5f, 50.0f });

    clouds.push_back({ 700.0f, 450.0f, 1.5f, 60.0f });
    clouds.push_back({ 650.0f, 500.0f, 1.5f, 60.0f });
    clouds.push_back({ 600.0f, 450.0f, 1.5f, 60.0f });

    clouds.push_back({ 1100.0f, 550.0f, 1.5f, 65.0f });
	clouds.push_back({ 1050.0f, 600.0f, 1.5f, 65.0f });
	clouds.push_back({ 1000.0f, 550.0f, 1.5f, 65.0f });

    heightMap.clear();
	trajectory.clear();

    gravity = 9.8f;
    initialProjectileSpeed = 70.0f;
    launchProjectile1 = false;
    launchProjectile2 = false;
    vector<Projectile> projectilesTank1;
    vector<Projectile> projectilesTank2;
    projectileRadius = 5.0f;
    threshold = 10.0f;
    epsilon = 5.0f;

    amplitudes = { 50.0f, 30.0f, 40.0f };
    frequencies = { 0.009f, 0.02f, 0.023f };
    verticalShift = 300.0f;

    for (float x = 0; x <= terrainWidth; x += spacing)
    {
        float y = amplitudes[0] * sin(frequencies[0] * x) +
            amplitudes[1] * sin(frequencies[1] * x) +
            amplitudes[2] * sin(frequencies[2] * x);
        y += verticalShift;
        heightMap.emplace_back(x, y, 0.0f);
    }
}

void Tema1::GenerateTerrain(float deltaTime)
{
    glm::vec3 color;
    float terrainLength = heightMap.size() - 1;

    for (float i = 0; i < heightMap.size() - 1; i++)
    {
        float t = i / terrainLength;

        color = glm::mix(
            glm::vec3(1.0f, 0.0f, 0.0f),
            glm::vec3(1.0f, 1.0f, 0.0f),
            t
        );

        color = glm::mix(
            color,
            glm::vec3(0.0f, 1.0f, 0.0f),
            t
        );

        color = glm::mix(
            color,
            glm::vec3(0.0f, 0.0f, 1.0f),
            t
        );

        color = glm::mix(
            color,
            glm::vec3(0.5f, 0.0f, 1.0f),
            t
        );

        color = glm::mix(
            color,
            glm::vec3(1.0f, 0.0f, 1.0f),
            t
        );

        Mesh* triangle1 = obj2D::CreateTriangle(
            "Triangle1",
            glm::vec3(heightMap[i].x, heightMap[i].y, 0),
            glm::vec3(heightMap[i + 1].x, heightMap[i + 1].y, 0),
            glm::vec3(heightMap[i].x, 0, 0),
            color,
            true
        );
        AddMeshToList(triangle1);

        Mesh* triangle2 = obj2D::CreateTriangle(
            "Triangle2",
            glm::vec3(heightMap[i + 1].x, heightMap[i + 1].y, 0),
            glm::vec3(heightMap[i].x, 0, 0),
            glm::vec3(heightMap[i + 1].x, 0, 0),
            color,
            true
        );
        AddMeshToList(triangle2);

        RenderMesh2D(triangle1, shaders["VertexColor"], glm::mat3(1));
        RenderMesh2D(triangle2, shaders["VertexColor"], glm::mat3(1));
    }
}

void Tema1::CreateTanks(
    const std::string& tankName,
    float tankPosX,
    float tankPosY,
    float tankRotation,
    float tankBottomTrapBottomBaseLength,
    float tankBottomTrapTopBaseLength,
    float tankBottomTrapHeight,
    const glm::vec3 tankBottomColor,
    float tankTopTrapBottomBaseLength,
    float tankTopTrapTopBaseLength,
    float tankTopTrapHeight,
    const glm::vec3 tankTopColor,
    float turretRadius,
	float turretRotation,
    float startAngle,
    float endAngle,
    float turretOffsetX,
    float turretOffsetY,
    const glm::vec3 tankTurretColor,
    float barrelWidth,
    float barrelHeight,
    float barrelOffsetX,
    float barrelOffsetY)
{
    tankPosY = GetTerrainHeight(tankPosX);

    Mesh* baseTrap1 = obj2D::CreateUpsideDownTrapezoid(
        tankName + "_base1",
        glm::vec3(tankPosX - tankBottomTrapBottomBaseLength / 2, tankPosY, 0),
        tankBottomTrapTopBaseLength, tankBottomTrapBottomBaseLength, tankBottomTrapHeight, tankBottomColor, true
    );
    AddMeshToList(baseTrap1);

    Mesh* baseTrap2 = obj2D::CreateTrapezoid(
        tankName + "_base2",
        glm::vec3(tankPosX - tankTopTrapBottomBaseLength / 2 + tankBottomTrapHeight / 2, tankPosY + tankBottomTrapHeight - 20 / 2, 0),
        tankTopTrapBottomBaseLength, tankTopTrapTopBaseLength, tankTopTrapHeight, tankTopColor, true
    );
    AddMeshToList(baseTrap2);

    Mesh* turret = obj2D::CreateArc(
        tankName + "_turret",
        glm::vec3(tankPosX + turretOffsetX, tankPosY + tankTopTrapHeight + turretOffsetY, 0),
        turretRadius, startAngle, endAngle, tankTopColor, 100
    );
    AddMeshToList(turret);

    Mesh* barrel = obj2D::CreateRectangle(
        tankName + "_barrel",
        glm::vec3(tankPosX - barrelWidth / 2 + barrelOffsetX, tankPosY + tankTopTrapHeight + barrelOffsetY, 0),
        barrelWidth, barrelHeight, tankTurretColor, true
    );
    AddMeshToList(barrel);

    float centerX = tankPosX + (tankBottomTrapBottomBaseLength / 2);
    float centerY = tankPosY + (tankBottomTrapHeight / 2);
    glm::mat3 translateToOrigin = glm::mat3(1.0f);
    translateToOrigin[2][0] = -tankPosX;
    translateToOrigin[2][1] = -tankPosY;

    glm::mat3 rotationMatrix = glm::mat3(1.0f);
    rotationMatrix[0][0] = cos(tankRotation);
    rotationMatrix[0][1] = sin(tankRotation);
    rotationMatrix[1][0] = -sin(tankRotation);
    rotationMatrix[1][1] = cos(tankRotation);

    glm::mat3 translateBack = glm::mat3(1.0f);
    translateBack[2][0] = tankPosX;
    translateBack[2][1] = tankPosY;

	float centerXBarrel = tankPosX + barrelOffsetX;
	float centerYBarrel = tankPosY + barrelOffsetY;
	float rotationAngleBarrel = turretRotation;

	glm::mat3 translateToOriginBarrel = glm::mat3(1.0f);
	translateToOriginBarrel[2][0] = -centerXBarrel;
	translateToOriginBarrel[2][1] = -centerYBarrel;

	glm::mat3 rotationMatrixBarrel = glm::mat3(1.0f);
	rotationMatrixBarrel[0][0] = cos(rotationAngleBarrel);
	rotationMatrixBarrel[0][1] = sin(rotationAngleBarrel);
	rotationMatrixBarrel[1][0] = -sin(rotationAngleBarrel);
	rotationMatrixBarrel[1][1] = cos(rotationAngleBarrel);

	glm::mat3 translateBackBarrel = glm::mat3(1.0f);
	translateBackBarrel[2][0] = centerXBarrel;
	translateBackBarrel[2][1] = centerYBarrel;

	glm::mat3 finalMatrixBarrel = translateBackBarrel * rotationMatrixBarrel * translateToOriginBarrel;

    glm::mat3 finalMatrix = translateBack * rotationMatrix * translateToOrigin;
    RenderMesh2D(baseTrap1, shaders["VertexColor"], finalMatrix);
    RenderMesh2D(baseTrap2, shaders["VertexColor"], finalMatrix);
    RenderMesh2D(turret, shaders["VertexColor"], finalMatrix);
    RenderMesh2D(barrel, shaders["VertexColor"], finalMatrix * finalMatrixBarrel);
}

float Tema1::CalculateSlope(float x)
{
    for (float i = 0; i < heightMap.size() - 1; i++)
    {
        if (heightMap[i].x <= x && heightMap[i + 1].x >= x)
        {
            float dx = heightMap[i + 1].x - heightMap[i].x;
            float dy = heightMap[i + 1].y - heightMap[i].y;
            return dy / dx;
        }
    }
    return 0.0f;
}

float Tema1::GetTerrainHeight(float x)
{
    for (float i = 0; i < heightMap.size() - 1; i++)
    {
        if (heightMap[i].x <= x && heightMap[i + 1].x >= x)
        {
            float x1 = heightMap[i].x;
            float y1 = heightMap[i].y;
            float x2 = heightMap[i + 1].x;
            float y2 = heightMap[i + 1].y;

            float interpolatedY = y1 + (x - x1) * (y2 - y1) / (x2 - x1) + 8;
            return interpolatedY;
        }
    }

    return 0.0f;
}

void Tema1::UpdateTankPositionAndRotation()
{

    float slope1 = CalculateSlope(tank1PosX);
    float slope2 = CalculateSlope(tank2PosX);

    tank1Rotation = atan(slope1);
	tank2Rotation = atan(slope2);

    tank1PosY = GetTerrainHeight(tank1PosX);
    tank2PosY = GetTerrainHeight(tank2PosX);
}

void Tema1::CreateTrajectory(
    float turretRotation,
    float tankRotation,
    float barrelHeight,
    float barrelOffsetX,
    float barrelOffsetY,
    float tankPosX,
    float tankPosY)
{
    float launchAngle = turretRotation + tankRotation + glm::pi<float>() / 2.0f;
    float initialX = tankPosX + cos(launchAngle) * (barrelHeight + barrelOffsetX);
    float initialY = tankPosY + 5 + sin(launchAngle) * (barrelHeight + barrelOffsetY + 3);

    float initialSpeed = 70.0f;
    float velocityX = initialSpeed * cos(launchAngle);
    float velocityY = initialSpeed * sin(launchAngle);

    float gravity = 9.8f;
    float timeStep = 0.1f;

    glm::vec3 previousPoint = glm::vec3(initialX, initialY, 0.0f);
    for (float t = 0.0f; t <= 100.0f; t += timeStep)
    {
        float x = initialX + velocityX * t;
        float y = initialY + velocityY * t - 0.5f * gravity * t * t;

        if (y < GetTerrainHeight(x)) break;

        glm::vec3 currentPoint = glm::vec3(x, y, 0.0f);
        trajectory.push_back(currentPoint);

        Mesh* lineSegment = obj2D::CreateLineSegment("TrajectorySegment", previousPoint, currentPoint, glm::vec3(1, 1, 1));
        AddMeshToList(lineSegment);
        RenderMesh2D(lineSegment, shaders["VertexColor"], glm::mat3(1));

        previousPoint = currentPoint;
    }
}

void Tema1::CreateProjectile(float deltaTime,
    std::vector<Projectile>& projectiles,
    float targetTankPosX,
    float targetTankPosY,
    float& targetTankHealth,
    float tankCollisionRadius,
    const std::string& meshName)
{
    for (int i = 0; i < projectiles.size(); i++)
    {
        auto& projectile = projectiles[i];

        if (projectile.isActive)
        {
            projectile.velocityY -= gravity * deltaTime;
            projectile.posX += projectile.velocityX * deltaTime;
            projectile.posY += projectile.velocityY * deltaTime;

            projectile.colorChangeTimer += deltaTime;
            float t = sin(projectile.colorChangeTimer * M_PI / 2.0f);
            glm::vec3 projectileColor = glm::mix(glm::vec3(1.0f, 1.0f, 0.0f), glm::vec3(0.5f, 0.0f, 0.5f), t);

            if (projectile.posY <= GetTerrainHeight(projectile.posX))
            {
                projectile.isActive = false;
                continue;
            }

            if (CheckCollision(projectile.posX, projectile.posY, projectileRadius,
                targetTankPosX, targetTankPosY + tankBottomTrapHeight / 2, tankCollisionRadius))
            {
                projectile.isActive = false;
                targetTankHealth -= 20;
                break;
            }

            glm::mat3 translateProjectile = glm::mat3(1.0f);
            translateProjectile[2][0] = projectile.posX;
            translateProjectile[2][1] = projectile.posY;

            Mesh* mesh = obj2D::CreateCircle(meshName, glm::vec3(0, 0, 0), projectileRadius, projectileColor, 100, true);
            RenderMesh2D(mesh, shaders["VertexColor"], translateProjectile);
        }
    }
}

void Tema1::DeformTerrain(vector<Projectile> projectilesTank, float deltaTime)
{
    for (int i = 0; i < projectilesTank.size(); i++)
    {
        float impactX = projectilesTank[i].posX;
        float impactY = projectilesTank[i].posY;
        float radius = 30.0f;
        if (impactY - epsilon <= GetTerrainHeight(impactX))
        {
            for (float i = 0; i < heightMap.size(); i++)
            {
                float deltaX = abs(heightMap[i].x - impactX);

                if (deltaX <= radius)
                {
                    float deltaY = sqrt(radius * radius - deltaX * deltaX);

                    float newHeight = impactY - deltaY;

                    if (heightMap[i].y > newHeight)
                    {
                        heightMap[i].y = newHeight;
                    }
                }
            }
            
            projectilesTank[i].isActive = false;
        }
        SimulateLandslide(deltaTime);
    }
}

void Tema1::SimulateLandslide(float deltaTime)
{
    for (float i = 0; i < heightMap.size() - 1; i++)
    {
        if (heightMap[i].x > 0 && heightMap[i].x < 1280)
        {
            float d = abs(heightMap[i + 1].y - heightMap[i].y);

            if (d > threshold)
            {
                if (heightMap[i].y > heightMap[i + 1].y)
                {
                    float transfer = epsilon * deltaTime;
                    heightMap[i].y -= transfer;
                    heightMap[i + 1].y += transfer;
                }
                else if (heightMap[i].y < heightMap[i + 1].y)
                {
                    float transfer = epsilon * deltaTime;
                    heightMap[i].y += transfer;
                    heightMap[i + 1].y -= transfer;
                }
            }
        }
    }
}

bool Tema1::CheckCollision(float x1, float y1, float radius1, float x2, float y2, float radius2)
{
    float dx = x1 - x2;
    float dy = y1 - y2;
    float distanceSquared = dx * dx + dy * dy;
    float radiusSum = radius1 + radius2;
    return distanceSquared <= radiusSum * radiusSum;
}

void Tema1::DrawHealthBar(float x, float y, float health, float maxHealth)
{
    float barWidth = 100.0f;
    float barHeight = 10.0f;
    float healthWidth = (health / maxHealth) * barWidth;

    glm::vec3 healthColor = glm::mix(glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(1.0f, 0.0f, 0.0f), 1.0f - (health / maxHealth));

    Mesh* emptyBar = obj2D::CreateRectangle("HealthBarEmpty", glm::vec3(x, y, 0), barWidth, barHeight, healthColor, false);
    AddMeshToList(emptyBar);
    RenderMesh2D(emptyBar, shaders["VertexColor"], glm::mat3(1));

    Mesh* filledBar = obj2D::CreateRectangle("HealthBarFilled", glm::vec3(x, y, 0), healthWidth, barHeight, healthColor, true);
    AddMeshToList(filledBar);
    RenderMesh2D(filledBar, shaders["VertexColor"], glm::mat3(1));
}

void Tema1::DrawClouds()
{
    for (const auto& cloud : clouds)
    {
        glm::vec3 cloudColor(0.7f, 0.7f, 0.7f);
        Mesh* cloudMesh = obj2D::CreateCircle("Cloud", glm::vec3(cloud.posX, cloud.posY, 0), cloud.size, cloudColor, 100, true);
        RenderMesh2D(cloudMesh, shaders["VertexColor"], glm::mat3(1));
    }
}

void Tema1::UpdateClouds(float deltaTime)
{
    for (auto& cloud : clouds)
    {
        cloud.posX += cloud.velocityX * deltaTime;

        if (cloud.posX > window->GetResolution().x)
        {
            cloud.posX = -cloud.size;
        }
    }
}

void Tema1::FrameStart()
{
    glClearColor(0.0f, 0.0f, 0.2f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glm::ivec2 resolution = window->GetResolution();
    glViewport(0, 0, resolution.x, resolution.y);
}

void Tema1::Update(float deltaTimeSeconds)
{
    GenerateTerrain(deltaTimeSeconds);
	DrawClouds();
	UpdateClouds(deltaTimeSeconds * 10);

    if (tank1Health > 0)
    {
        CreateTanks(
            tank1Name,
            tank1PosX,
            tank1PosY,
            tank1Rotation,
            tankBottomTrapBottomBaseLength,
            tankBottomTrapTopBaseLength,
            tankBottomTrapHeight,
            tank1BottomColor,
            tankTopTrapBottomBaseLength,
            tankTopTrapTopBaseLength,
            tankTopTrapHeight,
            tank1TopColor,
            turretRadius1,
            turret1Rotation,
            startAngle1,
            endAngle1,
            turretOffsetX1,
            turretOffsetY1,
            tank1TurretColor,
            barrelWidth1,
            barrelHeight1,
            barrelOffsetX1,
            barrelOffsetY1
        );

        UpdateTankPositionAndRotation();

        DrawHealthBar(tank1PosX - 50, tank1PosY + 60, tank1Health, 100);

        CreateTrajectory(
            turret1Rotation,
            tank1Rotation,
            barrelHeight1,
            barrelOffsetX1,
            barrelOffsetY1,
            tank1PosX,
            tank1PosY
        );

        CreateProjectile(10 * deltaTimeSeconds, projectilesTank1, tank2PosX, tank2PosY, tank2Health, tankCollisionRadius, "ProjectileTank1");

        DeformTerrain(projectilesTank1, deltaTimeSeconds * 2);
    }

    if (tank2Health > 0)
    {
        CreateTanks(
            tank2Name,
            tank2PosX,
            tank2PosY,
            tank2Rotation,
            tankBottomTrapBottomBaseLength,
            tankBottomTrapTopBaseLength,
            tankBottomTrapHeight,
            tank2BottomColor,
            tankTopTrapBottomBaseLength,
            tankTopTrapTopBaseLength,
            tankTopTrapHeight,
            tank2TopColor,
            turretRadius1,
            turret2Rotation,
            startAngle1,
            endAngle1,
            turretOffsetX1,
            turretOffsetY1,
            tank2TurretColor,
            barrelWidth1,
            barrelHeight1,
            barrelOffsetX1,
            barrelOffsetY1
        );

        UpdateTankPositionAndRotation();

        DrawHealthBar(tank2PosX - 50, tank2PosY + 60, tank2Health, 100);

        CreateTrajectory(
            turret2Rotation,
            tank2Rotation,
            barrelHeight1,
            barrelOffsetX1,
            barrelOffsetY1,
            tank2PosX,
            tank2PosY
        );

        CreateProjectile(10 * deltaTimeSeconds, projectilesTank2, tank1PosX, tank1PosY, tank1Health, tankCollisionRadius, "ProjectileTank2");

        DeformTerrain(projectilesTank2, deltaTimeSeconds * 2);
    }
}

void Tema1::FrameEnd()
{

}

void Tema1::OnInputUpdate(float deltaTime, int mods)
{
    float tankSpeed = 200.0f;
	float turretSpeed = 1.0f;

    if (window->KeyHold(GLFW_KEY_A))
    {
        tank1PosX -= deltaTime * tankSpeed * cos(tank1Rotation);
    }
    if (window->KeyHold(GLFW_KEY_D))
    {
        tank1PosX += deltaTime * tankSpeed * cos(tank1Rotation);
    }

    if (window->KeyHold(GLFW_KEY_LEFT))
    {
        tank2PosX -= deltaTime * tankSpeed * cos(tank2Rotation);
    }
    if (window->KeyHold(GLFW_KEY_RIGHT))
    {
        tank2PosX += deltaTime * tankSpeed * cos(tank2Rotation);
    }
	if (window->KeyHold(GLFW_KEY_W))
    {
		if (turret1Rotation < 1.3f)
        {
			turret1Rotation += deltaTime * turretSpeed;
		}
	}
	if (window->KeyHold(GLFW_KEY_S))
    {
		if (turret1Rotation > -1.3f)
        {
			turret1Rotation -= deltaTime * turretSpeed;
		}
	}
	if (window->KeyHold(GLFW_KEY_UP))
    {
        if (turret2Rotation < 1.3f)
        {
            turret2Rotation += deltaTime * turretSpeed;
        }
	}
	if (window->KeyHold(GLFW_KEY_DOWN))
    {
        if (turret2Rotation > -1.3f)
        {
            turret2Rotation -= deltaTime * turretSpeed;
        }
	}

    tank1PosY = GetTerrainHeight(tank1PosX);
    tank2PosY = GetTerrainHeight(tank2PosX);
}

void Tema1::OnKeyPress(int key, int mods)
{
    if (key == GLFW_KEY_SPACE)
    {
        float launchAngle = turret1Rotation + tank1Rotation + glm::pi<float>() / 2.0f;
        float initialX = tank1PosX + cos(launchAngle) * (barrelHeight1 + barrelOffsetX1);
        float initialY = tank1PosY + 5 + sin(launchAngle) * (barrelHeight1 + barrelOffsetY1 + 3);
        float velocityX = initialProjectileSpeed * cos(launchAngle);
        float velocityY = initialProjectileSpeed * sin(launchAngle);

        projectilesTank1.push_back({ initialX, initialY, velocityX, velocityY, true });
    }

    if (key == GLFW_KEY_ENTER)
    {
        float launchAngle = turret2Rotation + tank2Rotation + glm::pi<float>() / 2.0f;
        float initialX = tank2PosX + cos(launchAngle) * (barrelHeight1 + barrelOffsetX1);
        float initialY = tank2PosY + 5 + sin(launchAngle) * (barrelHeight1 + barrelOffsetY1 + 3);
        float velocityX = initialProjectileSpeed * cos(launchAngle);
        float velocityY = initialProjectileSpeed * sin(launchAngle);

        projectilesTank2.push_back({ initialX, initialY, velocityX, velocityY, true });
    }
}

void Tema1::OnKeyRelease(int key, int mods)
{
    if (key == GLFW_KEY_SPACE)
    {
		launchProjectile1 = false;
    }
	if (key == GLFW_KEY_ENTER)
	{
		launchProjectile2 = false;
	}
}

void Tema1::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{
}


void Tema1::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
}


void Tema1::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{
}


void Tema1::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY)
{
}


void Tema1::OnWindowResize(int width, int height)
{
}

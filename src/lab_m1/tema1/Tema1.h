#include "components/simple_scene.h"
#include <vector>

namespace m1
{
    class Tema1 : public gfxc::SimpleScene
    {
    public:
        Tema1();
        ~Tema1();

        void Init() override;

    private:
        struct Projectile {
            float posX, posY;
            float velocityX, velocityY;
            bool isActive;
            float colorChangeTimer;
        };

        void GenerateTerrain(float deltaTime);
        void CreateTanks(
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
            float barrelOffsetY);
		float CalculateSlope(float x);
		float GetTerrainHeight(float x);
		void UpdateTankPositionAndRotation();
        void CreateTrajectory(
            float turretRotation,
            float tankRotation,
            float barrelHeight,
            float barrelOffsetX,
            float barrelOffsetY,
            float tankPosX,
            float tankPosY
        );
        void CreateProjectile(float deltaTime,
            std::vector<Projectile>& projectiles,
            float targetTankPosX,
            float targetTankPosY,
            float& targetTankHealth,
            float tankCollisionRadius,
            const std::string& meshName);
        void DeformTerrain(std::vector<Projectile> projectilesTank, float deltaTime);
        void SimulateLandslide(float deltaTime);
        bool CheckCollision(float x1, float y1, float radius1, float x2, float y2, float radius2);
        void DrawHealthBar(float x, float y, float health, float maxHealth);
		void DrawClouds();
		void UpdateClouds(float deltaTime);
        void FrameStart() override;
        void Update(float deltaTimeSeconds) override;
        void FrameEnd() override;
        void OnInputUpdate(float deltaTime, int mods) override;
        void OnKeyPress(int key, int mods) override;
        void OnKeyRelease(int key, int mods) override;
        void OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY) override;
        void OnMouseBtnPress(int mouseX, int mouseY, int button, int mods) override;
        void OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods) override;
        void OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY) override;
        void OnWindowResize(int width, int height) override;

    protected:
        glm::mat3 modelMatrix;
        float terrainWidth;
        float spacing;
        std::vector<glm::vec3> heightMap;
        std::vector<float> amplitudes;
        std::vector<float> frequencies;
        float verticalShift;

        float tank1PosX;
        float tank1PosY;
        float tank2PosX;
        float tank2PosY;
        std::string tank1Name;
        std::string tank2Name;
		glm::vec3 tank1TopColor;
		glm::vec3 tank1BottomColor;
		glm::vec3 tank1TurretColor;
		glm::vec3 tank2TopColor;
		glm::vec3 tank2BottomColor;
		glm::vec3 tank2TurretColor;

        float tank1Rotation;
        float tank2Rotation;
        float turret1Rotation;
        float turret2Rotation;

        float tankBottomTrapBottomBaseLength;
        float tankBottomTrapTopBaseLength;
        float tankBottomTrapHeight;
        float tankTopTrapBottomBaseLength;
        float tankTopTrapTopBaseLength;
        float tankTopTrapHeight;
        float turretRadius1;
        float startAngle1;
        float endAngle1;
        float barrelWidth1;
        float barrelHeight1;
        float turretOffsetX1;
        float turretOffsetY1;
        float barrelOffsetX1;
        float barrelOffsetY1;
        std::vector<glm::vec3> trajectory;

        bool launchProjectile1;
        bool launchProjectile2;
        float initialProjectileSpeed;
        float gravity;

        std::vector<Projectile> projectilesTank1;
        std::vector<Projectile> projectilesTank2;

        float projectileRadius;
        float tank1Health;
        float tank2Health;
        float tankCollisionRadius;
        float threshold;
        float epsilon;

        struct Cloud {
            float posX, posY;
            float velocityX;
            float size;
        };

        std::vector<Cloud> clouds;
    };
}

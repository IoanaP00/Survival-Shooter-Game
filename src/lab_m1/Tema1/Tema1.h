#pragma once

#include "components/simple_scene.h"
#include <list>


namespace m1
{
    class Tema1 : public gfxc::SimpleScene
    {
    public:
        struct ViewportSpace
        {
            ViewportSpace() : x(0), y(0), width(1), height(1) {}
            ViewportSpace(int x, int y, int width, int height)
                : x(x), y(y), width(width), height(height) {}
            int x;
            int y;
            int width;
            int height;
        };

        struct LogicSpace
        {
            LogicSpace() : x(0), y(0), width(1), height(1) {}
            LogicSpace(float x, float y, float width, float height)
                : x(x), y(y), width(width), height(height) {}
            float x;
            float y;
            float width;
            float height;
        };

    public:
        Tema1();
        ~Tema1();

        void Init() override;

    private:
        void FrameStart() override;
        void Update(float deltaTimeSeconds) override;
        void FrameEnd() override;

        void DrawScene(glm::mat3 visMatrix, float deltaTimeSeconds);

        void OnInputUpdate(float deltaTime, int mods) override;
        void OnKeyPress(int key, int mods) override;
        void OnKeyRelease(int key, int mods) override;
        void OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY) override;
        void OnMouseBtnPress(int mouseX, int mouseY, int button, int mods) override;
        void OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods) override;
        void OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY) override;



        // Sets the logic space and view space
        // logicSpace: { x, y, width, height }
        // viewSpace: { x, y, width, height }
        glm::mat3 VisualizationTransf2DUnif(const LogicSpace& logicSpace, const ViewportSpace& viewSpace);

        void SetViewportArea(const ViewportSpace& viewSpace, glm::vec3 colorColor = glm::vec3(0), bool clear = true);

    protected:
        typedef struct Bullet {
            float bulletAngle;
            float x, y;
            float currX, currY;
            float bulletDistance;
        };

        typedef struct Enemy {
            float enemyAngle;
            float x, y;
            float enemySpeed;
            int hit;
        };

        typedef struct Wall {
            float xMin, xMax;
            float yMin, yMax;
        };

        float length;
        ViewportSpace viewSpace;
        LogicSpace logicSpace;
        glm::mat3 modelMatrix, visMatrix;
        glm::ivec2 resolution = window->GetResolution();
        double xpos, ypos;
        glm::ivec2 cursorPos;
        float angularStep = 0;

        //Bullets & Enemies
        struct Bullet* b = (struct Bullet*)calloc(1, sizeof(struct Bullet));
        Enemy* e = (struct Enemy*)calloc(1, sizeof(struct Enemy));
        std::list<struct Bullet*> bullets;
        std::list<struct Enemy*> enemies;
        float timeToShoot = 0;
        float timeToSpawn = 3;
        float dirX, dirY;
        int send = 0, nrBullets = 0;


        //Walls & Obstacles
        struct Wall* ob1 = (struct Wall*)calloc(1, sizeof(struct Wall));
        struct Wall* ob2 = (struct Wall*)calloc(1, sizeof(struct Wall));
        struct Wall* ob3 = (struct Wall*)calloc(1, sizeof(struct Wall));
        struct Wall* ob4 = (struct Wall*)calloc(1, sizeof(struct Wall));

        struct Wall* LWall = (struct Wall*)calloc(1, sizeof(struct Wall));
        struct Wall* RWall = (struct Wall*)calloc(1, sizeof(struct Wall));
        struct Wall* UWall = (struct Wall*)calloc(1, sizeof(struct Wall));
        struct Wall* DWall = (struct Wall*)calloc(1, sizeof(struct Wall));

        std::list<struct Wall*>walls = { ob1, ob2, ob3, ob4, LWall, RWall, UWall, DWall };


        //Score & Health
        int score = 0;
        int health = 100;
        int final = 1;


        //My fct
        void UpdateHealthBar(int health);
        void GenerateEnemy();
        void MoveEnemy(struct Enemy* e, float deltaTimeSeconds);
        void ShootBullet(struct Bullet* b, float deltaTimeSeconds);
        float intersect(float radius, float circleX, float circleY, float minX, float maxX, float minY, float maxY);
        void SetWallParams(struct Wall* wall, float x, float X, float y, float Y);
        void BulletEnemy(struct Enemy* e, std::list<struct Bullet*> bullets);



    };
}   // namespace m1
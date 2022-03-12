//Popescu Ioana-Maria, 334CBb

#include "lab_m1/Tema1/Tema1.h"

#include <vector>
#include <list>
#include <iostream>

#include "lab_m1/Tema1/transform2D.h"
#include "lab_m1/Tema1/object2D.h"

#define MIN(a,b) ((a) < (b) ? (a) : (b))
#define MAX(a,b) ((a) > (b) ? (a) : (b))
#define PI 3.14159265359
#define PlayerRadius 0.23
#define BulletRadius 0.045

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
    auto camera = GetSceneCamera();
    camera->SetPosition(glm::vec3(0, 0, 50));
    camera->SetRotation(glm::vec3(0, 0, 0));
    camera->Update();
    GetCameraInput()->SetActive(false);

    logicSpace.x = 0;       // logic x
    logicSpace.y = 0;       // logic y
    logicSpace.width = 4;   // logic width
    logicSpace.height = 4;  // logic height

    glm::vec3 corner = glm::vec3(logicSpace.width / 2, logicSpace.height / 2, 0);

    //Latura patrat
    length = 0.3f;

    //All meshes
    
    //Enemy
    Mesh* square1 = object2D::CreateSquare("square1", corner, length - 0.05, glm::vec3(1, 0.32, 0.32), true);
    AddMeshToList(square1);

    Mesh* square2 = object2D::CreateSquare("square2", corner, length, glm::vec3(1, 0, 0), true);
    AddMeshToList(square2);

    //Obstacles
    Mesh* obstacle0 = object2D::CreateRectangle("rectangle32", corner, 1.5, 1, glm::vec3(0, 0.23, 0.72));
    AddMeshToList(obstacle0);
    Mesh* obstacle1 = object2D::CreateRectangle("rectangle52", corner, 2.5, 1, glm::vec3(0, 0.23, 0.72));
    AddMeshToList(obstacle1);
    Mesh* obstacle2 = object2D::CreateRectangle("rectangle22", corner, 1, 1, glm::vec3(0, 0.23, 0.72));
    AddMeshToList(obstacle2);

    SetWallParams(ob1, -5, -3.5, 0.5, 1.5);
    SetWallParams(ob2, -3, -2, -3, -0.5);
    SetWallParams(ob3, 0.75, 3.25, 1, 2);
    SetWallParams(ob4, 3.5, 4.5, -2.25, -1.25);

    SetWallParams(LWall, -6, -6, -3, 3);
    SetWallParams(RWall, 6, 6, -3, 3);
    SetWallParams(UWall, -6, 6, 3, 3);
    SetWallParams(DWall, -6, 6, -3, -3);


    //Health Bar
    Mesh* outH = object2D::CreateRectangle("outH", corner, 8, 1, glm::vec3(1, 1, 1), false);
    AddMeshToList(outH);
    Mesh* fullH = object2D::CreateRectangle("fullH", corner, 8, 1, glm::vec3(0, 1, 0));
    AddMeshToList(fullH);
    Mesh* thirdH = object2D::CreateRectangle("thirdH", corner, 6, 1, glm::vec3(1, 1, 0));
    AddMeshToList(thirdH);
    Mesh* halfH = object2D::CreateRectangle("halfH", corner, 4, 1, glm::vec3(1, 0.45, 0));
    AddMeshToList(halfH);
    Mesh* quarterH = object2D::CreateRectangle("quarterH", corner, 2, 1, glm::vec3(1, 0, 0));
    AddMeshToList(quarterH);

    //Frame
    Mesh* frame = object2D::CreateRectangle("frame", corner, 12, 6, glm::vec3(0.3, 0.3, 0.3));
    AddMeshToList(frame);

    //Player
    Mesh* circle1 = object2D::CreateCircle("circle1", corner, 1.0, glm::vec3(0.56, 0.45, 0.75), glm::vec3(0, 0, 0));
    circle1->SetDrawMode(GL_TRIANGLE_FAN);
    AddMeshToList(circle1);

    Mesh* circle2 = object2D::CreateCircle("circle2", corner, 1.15, glm::vec3(0.2, 0.1, 0.32), glm::vec3(0, 0, 0));
    circle2->SetDrawMode(GL_TRIANGLE_FAN);
    AddMeshToList(circle2);

    Mesh* circle3 = object2D::CreateCircle("circle3", corner, 0.5, glm::vec3(0, 0, 0), glm::vec3(1, 1, 0));
    circle3->SetDrawMode(GL_TRIANGLE_FAN);
    AddMeshToList(circle3);

    Mesh* circle4 = object2D::CreateCircle("circle4", corner, 0.5, glm::vec3(1, 1, 1), glm::vec3(-1, -1, 0));
    circle4->SetDrawMode(GL_TRIANGLE_FAN);
    AddMeshToList(circle4);

    //Bullet
    Mesh* bullet = object2D::CreateSquare("bullet", corner, 0.09, glm::vec3(0.98, 0.72, 0.3), true);
    AddMeshToList(bullet);

    cout << "Score: " << score << endl;

}

// Uniform 2D visualization matrix (same scale factor on x and y axes)
glm::mat3 Tema1::VisualizationTransf2DUnif(const LogicSpace& logicSpace, const ViewportSpace& viewSpace)
{
    float sx, sy, tx, ty, smin;
    sx = viewSpace.width / logicSpace.width;
    sy = viewSpace.height / logicSpace.height;
    if (sx < sy)
        smin = sx;
    else
        smin = sy;
    tx = viewSpace.x - smin * logicSpace.x + (viewSpace.width - smin * logicSpace.width) / 2;
    ty = viewSpace.y - smin * logicSpace.y + (viewSpace.height - smin * logicSpace.height) / 2;

    return glm::transpose(glm::mat3(
        smin, 0.0f, tx,
        0.0f, smin, ty,
        0.0f, 0.0f, 1.0f));
}

void Tema1::SetViewportArea(const ViewportSpace& viewSpace, glm::vec3 colorColor, bool clear)
{
    glViewport(viewSpace.x, viewSpace.y, viewSpace.width, viewSpace.height);

    glEnable(GL_SCISSOR_TEST);
    glScissor(viewSpace.x, viewSpace.y, viewSpace.width, viewSpace.height);

    // Clears the color buffer (using the previously set color) and depth buffer
    glClearColor(colorColor.r, colorColor.g, colorColor.b, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glDisable(GL_SCISSOR_TEST);

    GetSceneCamera()->SetOrthographic((float)viewSpace.x, (float)(viewSpace.x + viewSpace.width), (float)viewSpace.y, (float)(viewSpace.y + viewSpace.height), 0.1f, 400);
    GetSceneCamera()->Update();
}

void Tema1::FrameStart()
{
    // Clears the color buffer (using the previously set color) and depth buffer
    glClearColor(0, 0, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Tema1::Update(float deltaTimeSeconds)
{
    if (deltaTimeSeconds > 0.02)
        deltaTimeSeconds = 0.02;

    glm::ivec2 resolution = window->GetResolution();

    // Sets the screen area where to draw
    viewSpace = ViewportSpace(0, 0, resolution.x, resolution.y);
    SetViewportArea(viewSpace, glm::vec3(0, 0, 0), true);

    // Compute the 2D visualization matrix
    visMatrix = glm::mat3(1);
    visMatrix *= VisualizationTransf2DUnif(logicSpace, viewSpace);

    cursorPos = window->GetCursorPosition();
    angularStep = (atan2(resolution.y / 2 - cursorPos.y, cursorPos.x - resolution.x / 2));

    DrawScene(visMatrix, deltaTimeSeconds);

}

//Position & Render all Meshes
void Tema1::DrawScene(glm::mat3 visMatrix, float deltaTimeSeconds)
{
    UpdateHealthBar(health);

    //Obstacles
    modelMatrix = visMatrix * transform2D::Translate(-4.25, 1);
    RenderMesh2D(meshes["rectangle32"], shaders["VertexColor"], modelMatrix);

    modelMatrix = visMatrix *
        transform2D::Translate(-2.5, -1.75) *
        transform2D::Translate(logicSpace.width / 2, logicSpace.height / 2) *
        transform2D::Rotate(PI / 2) *
        transform2D::Translate(-logicSpace.width / 2, -logicSpace.height / 2);
    RenderMesh2D(meshes["rectangle52"], shaders["VertexColor"], modelMatrix);

    modelMatrix = visMatrix * transform2D::Translate(2, 1.5);
    RenderMesh2D(meshes["rectangle52"], shaders["VertexColor"], modelMatrix);

    modelMatrix = visMatrix * transform2D::Translate(4, -1.75);
    RenderMesh2D(meshes["rectangle22"], shaders["VertexColor"], modelMatrix);


    //Jucator
    modelMatrix = visMatrix *
        transform2D::Translate(logicSpace.x, logicSpace.y) *
        transform2D::Translate(logicSpace.width / 2, logicSpace.height / 2) *
        transform2D::Rotate(angularStep) *
        transform2D::Scale(0.2, 0.2) *
        transform2D::Translate(-logicSpace.width / 2, -logicSpace.height / 2);

    //Body
    RenderMesh2D(meshes["circle1"], shaders["VertexColor"], modelMatrix);
    RenderMesh2D(meshes["circle2"], shaders["VertexColor"], modelMatrix);

    //Arms
    modelMatrix = visMatrix *
        transform2D::Translate(logicSpace.x, logicSpace.y) *
        transform2D::Translate(logicSpace.width / 2, logicSpace.height / 2) *
        transform2D::Rotate(angularStep + 2 * PI) *
        transform2D::Scale(0.16, 0.16) *
        transform2D::Translate(-logicSpace.width / 2, -logicSpace.height / 2);
    RenderMesh2D(meshes["circle3"], shaders["VertexColor"], modelMatrix);

    modelMatrix = visMatrix *
        transform2D::Translate(logicSpace.x, logicSpace.y) *
        transform2D::Translate(logicSpace.width / 2, logicSpace.height / 2) *
        transform2D::Rotate(angularStep + PI / 2) *
        transform2D::Scale(0.16, 0.16) *
        transform2D::Translate(-logicSpace.width / 2, -logicSpace.height / 2);
    RenderMesh2D(meshes["circle4"], shaders["VertexColor"], modelMatrix);

    //Check if game is over
    if (health > 0) {
        timeToShoot += deltaTimeSeconds;
        timeToSpawn += deltaTimeSeconds;

        //Spawn an enemy
        if (timeToSpawn >= 2) {
            GenerateEnemy();
            timeToSpawn = 0;
        }

        //Check PlayerEnemy coliision
        for (struct Enemy* e : enemies) {
            MoveEnemy(e, deltaTimeSeconds);
            if (intersect(PlayerRadius, logicSpace.x, logicSpace.y,
                e->x - length / 2, e->x + length / 2,
                e->y - length / 2, e->y + length / 2)) {
                e->hit = 1;
                health -= 25;
            }
        }
        std::list<struct Enemy*> aux;
        for (struct Enemy* e : enemies) {
            if (e->hit == 0) {
                aux.push_back(e);
            }
        }
        enemies = aux;
        aux.clear();

        //Shoot & Delete bullets
        for (struct Bullet* b : bullets)
        {
            ShootBullet(b, deltaTimeSeconds);
        }


        //Check Bullet - Enemy Collision
        for (struct Enemy* e : enemies) {
            BulletEnemy(e, bullets);
        }
        std::list<struct Bullet*> aux1;
        for (struct Bullet* b : bullets) {
            if (b->bulletDistance <= 2.5) {
                aux1.push_back(b);
            }
        }
        bullets = aux1;
        aux1.clear();
    }
    //Game over
    else if (final) {
        cout << "Final Score: " << score << endl;
        cout << "Try again!" << endl;
        final = 0;
    }
    //Frame
    modelMatrix = visMatrix;
    RenderMesh2D(meshes["frame"], shaders["VertexColor"], modelMatrix);
}

void Tema1::FrameEnd()
{
}

//Move & Render Bullet
void Tema1::ShootBullet(struct Bullet* b, float deltaTimeSeconds) {
    b->bulletDistance += deltaTimeSeconds;

    //Move & Rotate bullet
    modelMatrix = visMatrix *
        transform2D::Translate(b->x, b->y) *
        transform2D::Translate(logicSpace.width / 2, logicSpace.height / 2) *
        transform2D::Rotate(b->bulletAngle) *
        transform2D::Translate(b->bulletDistance, 0) *
        transform2D::Translate(-logicSpace.width / 2, -logicSpace.height / 2);

    //Increment the current coordinates of the bullet
    b->currX = b->x + b->bulletDistance * cos(b->bulletAngle);
    b->currY = b->y + b->bulletDistance * sin(b->bulletAngle);

    RenderMesh2D(meshes["bullet"], shaders["VertexColor"], modelMatrix);

}

//Generate random start position & speed
void Tema1::GenerateEnemy() {
    Enemy* enemy = (struct Enemy*)calloc(1, sizeof(struct Enemy));

    //Generate random movement speeds & start positions
    enemy->enemySpeed = (1 + (std::rand() % 6)) / 10.0;
    enemy->x = -6 + (std::rand() % 13);
    enemy->y = -3 + (std::rand() % 7);

    //Ensure enemy doesn't spawn of player
    if (logicSpace.x - 1 <= enemy->x &&
        enemy->x <= logicSpace.x + 1) {
        if (enemy->x) { enemy->x *= -1; }
        else { enemy->x += 2; }
    }
    if (logicSpace.y - 1 <= enemy->y &&
        enemy->y <= logicSpace.y + 1) {
        if (enemy->y) { enemy->y *= -1; }
        else { enemy->y += 2; }
    }

    enemies.push_back(enemy);
}

//Translate & Render Enemy
void Tema1::MoveEnemy(struct Enemy* e, float deltaTimeSeconds) {
    float posPlayerX = logicSpace.x;
    float posPlayerY = logicSpace.y;

    //Get the movement direction of the enemy
    //On Ox
    if (e->x > posPlayerX) { dirX = -1; }
    else if (e->x < posPlayerX) { dirX = 1; }
    else if (e->x == posPlayerX) { dirX = 0; }

    //On Oy
    if (e->y > posPlayerY) { dirY = -1; }
    else if (e->y < posPlayerY) { dirY = 1; }
    else if (e->y == posPlayerY) { dirY = 0; }

    modelMatrix = visMatrix *
        transform2D::Translate(e->x, e->y) *
        transform2D::Translate(dirX * e->enemySpeed * deltaTimeSeconds, dirY * e->enemySpeed * deltaTimeSeconds);

    //Current position of an enemy
    e->x += dirX * deltaTimeSeconds * e->enemySpeed;
    e->y += dirY * deltaTimeSeconds * e->enemySpeed;

    RenderMesh2D(meshes["square1"], shaders["VertexColor"], modelMatrix);
    RenderMesh2D(meshes["square2"], shaders["VertexColor"], modelMatrix);

}

//Render the proper health bar depending on the current amount of health
void Tema1::UpdateHealthBar(int health) {
    if (health == 100) {
        modelMatrix = visMatrix *
            transform2D::Translate(logicSpace.x, logicSpace.y) *
            transform2D::Translate(-2, 1.5) *
            transform2D::Translate(logicSpace.width / 2, logicSpace.height / 2) *
            transform2D::Scale(0.25, 0.25) *
            transform2D::Translate(-logicSpace.width / 2, -logicSpace.height / 2);
        RenderMesh2D(meshes["fullH"], shaders["VertexColor"], modelMatrix);
    }
    else if (health == 75) {
        modelMatrix = visMatrix *
            transform2D::Translate(logicSpace.x, logicSpace.y) *
            transform2D::Translate(-2.25, 1.5) *
            transform2D::Translate(logicSpace.width / 2, logicSpace.height / 2) *
            transform2D::Scale(0.25, 0.25) *
            transform2D::Translate(-logicSpace.width / 2, -logicSpace.height / 2);
        RenderMesh2D(meshes["thirdH"], shaders["VertexColor"], modelMatrix);
    }
    else if (health == 50) {
        modelMatrix = visMatrix *
            transform2D::Translate(logicSpace.x, logicSpace.y) *
            transform2D::Translate(-2.5, 1.5) *
            transform2D::Translate(logicSpace.width / 2, logicSpace.height / 2) *
            transform2D::Scale(0.25, 0.25) *
            transform2D::Translate(-logicSpace.width / 2, -logicSpace.height / 2);
        RenderMesh2D(meshes["halfH"], shaders["VertexColor"], modelMatrix);
    }
    else if (health == 25) {
        modelMatrix = visMatrix *
            transform2D::Translate(logicSpace.x, logicSpace.y) *
            transform2D::Translate(-2.75, 1.5) *
            transform2D::Translate(logicSpace.width / 2, logicSpace.height / 2) *
            transform2D::Scale(0.25, 0.25) *
            transform2D::Translate(-logicSpace.width / 2, -logicSpace.height / 2);
        RenderMesh2D(meshes["quarterH"], shaders["VertexColor"], modelMatrix);
    }

    modelMatrix = visMatrix *
        transform2D::Translate(logicSpace.x, logicSpace.y) *
        transform2D::Translate(-2, 1.5) *
        transform2D::Translate(logicSpace.width / 2, logicSpace.height / 2) *
        transform2D::Scale(0.25, 0.25) *
        transform2D::Translate(-logicSpace.width / 2, -logicSpace.height / 2);
    RenderMesh2D(meshes["outH"], shaders["VertexColor"], modelMatrix);
}

//Check collision between an enemy and all bullets on screen
void Tema1::BulletEnemy(struct Enemy* e, std::list<struct Bullet*> bullets) {
    for (struct Bullet* b : bullets) {
        if (intersect(BulletRadius, b->currX, b->currY,
            e->x - length / 2, e->x + length / 2,
            e->y - length / 2, e->y + length / 2)) {
            e->hit = 1;
            b->bulletDistance = 100;
            score += 100;
            cout << "Score: " << score << endl;
        }
    }
}

//Set wall parameters
void Tema1::SetWallParams(struct Wall* wall, float x, float X, float y, float Y) {
    wall->xMin = x;
    wall->xMax = X;
    wall->yMin = y;
    wall->yMax = Y;
}

//Collision circle-square
float Tema1::intersect(float radius, float circleX, float circleY, float minX, float maxX, float minY, float maxY) {
    float x = MAX(minX, MIN(circleX, maxX));
    float y = MAX(minY, MIN(circleY, maxY));

    float distance = sqrt((x - circleX) * (x - circleX) +
        (y - circleY) * (y - circleY));

    return distance < radius;
}

void Tema1::OnInputUpdate(float deltaTime, int mods)
{
    //Move Player
    
    //Ox
    if (window->KeyHold(GLFW_KEY_A))
    {
        float d = 0;

        for (struct Wall* w : walls) {
            d += intersect(PlayerRadius, logicSpace.x - deltaTime, logicSpace.y, w->xMin, w->xMax, w->yMin, w->yMax);
        }
        if (!d) {
            logicSpace.x -= deltaTime;
        }
    }

    if (window->KeyHold(GLFW_KEY_D))
    {
        float d = 0;
        for (struct Wall* w : walls) {
            d += intersect(PlayerRadius, logicSpace.x + deltaTime, logicSpace.y, w->xMin, w->xMax, w->yMin, w->yMax);
        }
        if (!d) {
            logicSpace.x += deltaTime;
        }
    }

    //Oy
    if (window->KeyHold(GLFW_KEY_W))
    {
        float d = 0;
        for (struct Wall* w : walls) {
            d += intersect(PlayerRadius, logicSpace.x, logicSpace.y + deltaTime, w->xMin, w->xMax, w->yMin, w->yMax);
        }
        if (!d) {
            logicSpace.y += deltaTime;
        }
        
    }

    if (window->KeyHold(GLFW_KEY_S))
    {
        float d = 0;
        for (struct Wall* w : walls) {
            d += intersect(PlayerRadius, logicSpace.x, logicSpace.y - deltaTime, w->xMin, w->xMax, w->yMin, w->yMax);
        }
        if (!d) {
            logicSpace.y -= deltaTime;
        }
    }
}


void Tema1::OnKeyPress(int key, int mods)
{
}


void Tema1::OnKeyRelease(int key, int mods)
{
}


void Tema1::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{
}


void Tema1::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
}


void Tema1::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{
    if (button == 1) {
        if (timeToShoot >= 0.3) {
            //Initialize new bullet
            b = (struct Bullet*)calloc(1, sizeof(struct Bullet));

            b->x = logicSpace.x;
            b->y = logicSpace.y;
            b->currX = logicSpace.x;
            b->currY = logicSpace.y;

            b->bulletAngle = angularStep;
            b->bulletDistance = 0;

            //Add bullet in list
            bullets.push_back(b);
            timeToShoot = 0;
        }
    }
}


void Tema1::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY)
{
}
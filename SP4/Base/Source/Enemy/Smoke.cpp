#include "Smoke.h"
#include "MeshBuilder.h"
#include "GraphicsManager.h"
#include "RenderHelper.h"
#include "../EntityManager.h"
#include "LoadTGA.h"

Smoke::Smoke(const Vector3 _pos, const int _roomID)
    : m_bSmokeActive(true)
{
    this->position = _pos;
    this->scale = Vector3(30, 30, 0);

    this->maxHealth = 10;
    this->health = this->maxHealth;

    this->SetCollider(true);
    int x = scale.x, y = scale.y;
    x = (x >> 1) - 5; y = (y >> 1) - 5;
    this->SetAABB(Vector3(x, y, 0), Vector3(-x, -y, 0));

    smoke = MeshBuilder::GetInstance()->GenerateSpriteAnimation(1, 5);
    smoke->textureID = LoadTGA("Image//Boss//reaper_smoke.tga");
    smoke->m_anim = new Animation;
    smoke->m_anim->Set(0, 4, 1, 1.f, true);

    roomID = _roomID;

    EntityManager::GetInstance()->AddEntity(this, roomID);
}

Smoke::~Smoke()
{
    delete smoke;
}

void Smoke::Update(double dt)
{
    if (health <= 0)
    {
        m_bSmokeActive = false;
        return;
    }

    Enemy2D::Update(dt);

    if (smoke)
    {
        smoke->m_anim->animActive = true;
        smoke->Update(dt * 1.f);
    }
}

void Smoke::Render(float& _renderOrder)
{
    Enemy2D::Render(_renderOrder);

    glEnable(GL_SAMPLE_ALPHA_TO_COVERAGE);

    MS& modelStack = GraphicsManager::GetInstance()->GetModelStack();

    modelStack.PushMatrix();
    modelStack.Translate(position.x, position.y + _renderOrder, position.z);
    modelStack.Rotate(90, -1, 0, 0);
    modelStack.Scale(scale.x, scale.y, scale.z);
    RenderHelper::RenderMesh(smoke);
    modelStack.PopMatrix();

    glDisable(GL_SAMPLE_ALPHA_TO_COVERAGE);
}
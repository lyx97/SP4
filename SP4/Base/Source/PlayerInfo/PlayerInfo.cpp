#include "PlayerInfo.h"

#include "MouseController.h"
#include "KeyboardController.h"
#include "Mtx44.h"
#include "../Projectile/Laser.h"
#include "../Lua/LuaInterface.h"
#include "../EntityManager.h"
#include "../Application.h"
#include "RenderHelper.h"
#include "GraphicsManager.h"
#include "MeshBuilder.h"
#include "../Level/Level.h"
#include "../Projectile/Laser.h"

// Allocating and initializing CPlayerInfo's static data member.
// The pointer is allocated but not the object's constructor.
CPlayerInfo *CPlayerInfo::s_instance = 0;

CPlayerInfo::CPlayerInfo(void)
	: GenericEntity(NULL)
	, prevIndex(Vector3(0, 0, 0))
	, prevPos(Vector3(0, 0, 0))
	, m_dSpeed(40.0)
	, m_dAcceleration(10.0)
	, keyMoveForward('W')
	, keyMoveBackward('S')
	, keyMoveLeft('A')
	, keyMoveRight('D')
	, isMoving(false)
	, isDashed(false)
	, usingTreasure(false)
	, usedTreasure(false)
	, maxHealth(100.0f)
	, health(90.0f)
	, maxSpeed(300.0f)
	, dashDistance(30.0f)
	, dashCooldown(0.6f)
	, dashCooldownTimer(0)
	, healthregen(2.5f)
	, healthregenCooldownTimer(0)
	, treasureDurationTimer(0)
	, defaultHealthRegenCooldown(5)
	, defaultSpeed(300.f)
	, defaultDamage(10)
	, defaultFirerate(0.15f)
	, dreamBar(MAX_DREAMBAR * 0.5f)
	, invincibleTimer(0)
	, invincible(false)
	, killCount(0)
	, rotateLeftLeg(Vector3(0, 0, 0))
	, rotateRightLeg(Vector3(0, 0, 0))
	, rotateLLUP(false)
	, onScreenUI(true)
	, UIScale(75)
	, prevHealth(0)
	, healthRatio(0)
	, healthScale(0)
	, dashScale(0)
	, dreambarScale(0)
	, treasureScale(0)
	, elapsedTime(0)
	, fire(true)
{
}

CPlayerInfo::~CPlayerInfo(void)
{
}

// Initialise this class instance
void CPlayerInfo::Init(void)
{
    m_eEntityType = ENTITY_TYPE::PLAYER;

	scale.Set(24, 30, 1);
	// Set the default values
	defaultPosition.Set(0, 0, 10);
	defaultTarget.Set(0, 0, 0);
	defaultUp.Set(0, 1, 0);

	// Set the current values
	//position.Set(0, 0, 10);
	position.x = CLuaInterface::GetInstance()->getIntValue("PlayerPos_x");
	position.z = CLuaInterface::GetInstance()->getIntValue("PlayerPos_z");
	target.Set(0, 0, 0);
	up.Set(0, 1, 0);
    direction.Set(0, 0, 0);

	// Set Boundary
	maxBoundary.Set(1, 1, 1);
	minBoundary.Set(-1, -1, -1);

	treasure = new Treasure();
	healthregenCooldown = defaultHealthRegenCooldown;
	maxSpeed = defaultSpeed;
	damage = defaultDamage;
	timeBetweenShots = defaultFirerate;

	playerMeshes[0] = (SpriteAnimation*)MeshBuilder::GetInstance()->GetMesh("player_walkleft");     // left
	playerMeshes[1] = (SpriteAnimation*)MeshBuilder::GetInstance()->GetMesh("player_walkright");    // right

	for (int i = 0; i < 2; ++i)
	{
		playerMeshes[i]->m_anim = new Animation;
		playerMeshes[i]->m_anim->Set(0, 6, 1, 1.f, true);
	}

    currentAnimation = playerMeshes[0];

    // Initialise the custom keyboard inputs
    keyMoveForward = CLuaInterface::GetInstance()->getCharValue("moveForward");
    keyMoveBackward = CLuaInterface::GetInstance()->getCharValue("moveBackward");
    keyMoveLeft = CLuaInterface::GetInstance()->getCharValue("moveLeft");
    keyMoveRight = CLuaInterface::GetInstance()->getCharValue("moveRight");

    this->SetCollider(true);
    int x = scale.x, y = scale.y;
    x = (x >> 1) - 5; y = (y >> 1) - 5;
    this->SetAABB(Vector3(x, y, 0), Vector3(-x, -y, 0));

    EntityManager::GetInstance()->AddEntity(this, roomID);

    // Init heatmap
    int xSize = CLevel::GetInstance()->GetRoom(roomID)->GetRoomXMax();
    int zSize = CLevel::GetInstance()->GetRoom(roomID)->GetRoomZMax();

    heatmap = new CHeatmap*[xSize];
    for (int x = 0; x <= xSize; ++x)
        heatmap[x] = new CHeatmap[zSize + 1];

    // Generate
    CGenerateHeatmap::GetInstance()->GenerateHeatmap(heatmap, xSize, zSize, index.x, index.z);
    CGenerateHeatmap::GetInstance()->CalculateDirection(heatmap, xSize, zSize);

    int xIndex = index.x;
    int zIndex = index.z;

    prevIndex = index;

    // Spawn location -- Fixed to room size of odd number
    SpawnLocation[0] = CLevel::GetInstance()->GetRoom(roomID)->GetSpatialPartition()->GetGridPos(xSize - 1, zSize >> 1);
    SpawnLocation[1] = CLevel::GetInstance()->GetRoom(roomID)->GetSpatialPartition()->GetGridPos(1, zSize >> 1);
    SpawnLocation[2] = CLevel::GetInstance()->GetRoom(roomID)->GetSpatialPartition()->GetGridPos(xSize >> 1, zSize - 1);
    SpawnLocation[3] = CLevel::GetInstance()->GetRoom(roomID)->GetSpatialPartition()->GetGridPos(xSize >> 1, 1);

    // Boundary
    minBoundary.Set(CLevel::GetInstance()->GetRoom(roomID)->GetSpatialPartition()->GetGridPos(1, 1) - Vector3(GRIDSIZE, 0, GRIDSIZE));
    maxBoundary.Set(CLevel::GetInstance()->GetRoom(roomID)->GetSpatialPartition()->GetGridPos(xSize - 1, zSize - 1) + Vector3(GRIDSIZE, 0, GRIDSIZE));

	float halfWindowWidth = Application::GetInstance().GetWindowWidth() << 1;
	float halfWindowHeight = Application::GetInstance().GetWindowHeight() << 1;

	for (int i = 0; i < 5; ++i)
	{
		textOBJ[i] = Create::Text2DObject(
			"text",
			Vector3(-halfWindowWidth + 10, halfWindowHeight - (fontSize * (i * 1.5f)), 0),
			"",
			Vector3(fontSize, fontSize, fontSize),
			Color(0.0f, 1.0f, 0.0f));
	}
}

// Set the boundary for the player info
void CPlayerInfo::SetBoundary(Vector3 max, Vector3 min)
{
	maxBoundary = max;
	minBoundary = min;
}

// Reset this player instance to default
void CPlayerInfo::Reset(void)
{
	// Set the current values to default values
	position = defaultPosition;
	target = defaultTarget;
	up = defaultUp;
}

/********************************************************************************
Player Update
********************************************************************************/
void CPlayerInfo::Update(double dt)
{
    position.y = 0;

    int xSize = CLevel::GetInstance()->GetRoom(roomID)->GetRoomXMax();
    int zSize = CLevel::GetInstance()->GetRoom(roomID)->GetRoomZMax();

    if (prevIndex != index)
    {
        CGenerateHeatmap::GetInstance()->GenerateHeatmap(heatmap, xSize, zSize, index.x, index.z);
        CGenerateHeatmap::GetInstance()->CalculateDirection(heatmap, xSize, zSize);
        prevIndex = index;
    }

	if (dreamBar > Math::EPSILON)
		this->dreamBar -= 0.1 * dt;

    if (currentAnimation)
    {
        currentAnimation->m_anim->animActive = true;
        currentAnimation->Update(dt * 1.5f);
    }

    if (prevPos != position)
    {
        prevPos = position;
    }

	position += velocity * (float)dt;
	if (!Application::GetInstance().GetWorldBasedMousePos().IsZero())
		front.Set(Vector3(position - Application::GetInstance().GetWorldBasedMousePos()).Normalized());
	Vector3 forceDir;
	isMoving = false;
	if (!isMoving)
	{
		velocity *= 0.9f;
	}

    // Change room
    if (CLevel::GetInstance()->GetRoom(roomID)->GetRoomCleared() &&
        CLevel::GetInstance()->GetRoom(roomID)->GetSpatialPartition()->GetGridType(index.x, index.z) == GRID_TYPE::DOOR)
    {
        int previousRoom = roomID;

        if (CLevel::GetInstance()->GetRoom(roomID)->GetRoomXMin() == index.x && (CLevel::GetInstance()->GetRoom(roomID)->GetRoomZMax() >> 1) == index.z)
        {
            roomID = CLevel::GetInstance()->GetRoom(roomID)->GetDoorToRoomID(0);
            position = SpawnLocation[0];
        }
        if (CLevel::GetInstance()->GetRoom(roomID)->GetRoomXMax() == index.x && (CLevel::GetInstance()->GetRoom(roomID)->GetRoomZMax() >> 1) == index.z)
        {
            roomID = CLevel::GetInstance()->GetRoom(roomID)->GetDoorToRoomID(1);
            position = SpawnLocation[1];
        }
        if ((CLevel::GetInstance()->GetRoom(roomID)->GetRoomXMax() >> 1) == index.x && CLevel::GetInstance()->GetRoom(roomID)->GetRoomZMin() == index.z)
        {
            roomID = CLevel::GetInstance()->GetRoom(roomID)->GetDoorToRoomID(2);
            position = SpawnLocation[2];
        }
        if ((CLevel::GetInstance()->GetRoom(roomID)->GetRoomXMax() >> 1) == index.x && CLevel::GetInstance()->GetRoom(roomID)->GetRoomZMax() == index.z)
        {
            roomID = CLevel::GetInstance()->GetRoom(roomID)->GetDoorToRoomID(3);
            position = SpawnLocation[3];
        }

        EntityManager::GetInstance()->GetSpatialPartition(roomID)->Add(this);
        EntityManager::GetInstance()->GetSpatialPartition(previousRoom)->Remove(this);
    }

    // Dash cooldown
	if (isDashed)
	{
		dashCooldownTimer -= dt;
		if (dashCooldownTimer <= 0)
		{
			dashCooldownTimer = 0;
			isDashed = false;
		}
	}
	// Health regeneration cooldown
	healthregenCooldownTimer -= dt;
	if (healthregenCooldownTimer <= 0)
	{
		if (health + healthregen > maxHealth)
			health = maxHealth;
		else
			health += healthregen;

		healthregenCooldownTimer = healthregenCooldown;
	}

	if (KeyboardController::GetInstance()->IsKeyDown(keyMoveForward))
	{
        forceDir.z -= 1;
	}
	if (KeyboardController::GetInstance()->IsKeyDown(keyMoveBackward))
	{
        forceDir.z += 1;
	}
	if (KeyboardController::GetInstance()->IsKeyDown(keyMoveLeft))
	{
        forceDir.x -= 1;
        currentAnimation = playerMeshes[0];
	}
	if (KeyboardController::GetInstance()->IsKeyDown(keyMoveRight))
	{
        forceDir.x += 1;
        currentAnimation = playerMeshes[1];
	}

	if (velocity.LengthSquared() < maxSpeed * maxSpeed)
	{
		isMoving = true;
		forceMagnitude = maxSpeed;
		this->ApplyForce(forceDir, forceMagnitude * dt);
	}

	if (MouseController::GetInstance()->IsButtonPressed(MouseController::RMB))
	{
		if (!forceDir.IsZero() && !isDashed)
		{
			dashCooldownTimer = dashCooldown;
			forceMagnitude = maxSpeed * dashDistance;
			this->ApplyForce(forceDir, forceMagnitude * dt);
			isDashed = true;
		}
	}

	for (auto entity : EntityManager::GetInstance()->GetEntityList())
	{
        if ((entity)->GetRoomID() == CPlayerInfo::GetInstance()->GetRoomID())
        {
            if (entity->GetEntityType() == TREASURE)
            {
                Treasure* newTreasure = dynamic_cast<Treasure*>(entity);
                if ((newTreasure->GetPosition() - this->position).LengthSquared() < 100 &&
                    KeyboardController::GetInstance()->IsKeyPressed('Q'))
                {
                    if (treasure->treasure_type == Treasure::NONE)
                    {
                        AddTreasures(newTreasure->random);
                        newTreasure->SetIsDone(true);
                    }
                    else
                    {
                        int tempType = this->treasure->treasure_type;
                        AddTreasures(newTreasure->random);
                        newTreasure->SetIsDone(true);
                        Treasure* spawnTreasure = new Treasure();
                        spawnTreasure->SpawnTreasure(this->position, tempType, roomID);
                    }
                }
            }
        }
	}

	if (KeyboardController::GetInstance()->IsKeyPressed('E'))
	{
		if (this->killCount >= treasure->GetCooldown())
		{
			treasureDurationTimer = 0;
			usingTreasure = true;
			this->killCount = 0;
		}
		else
		{
			cout << "NOT ENOUGH KILLS" << endl;
		}
	}
	if (usingTreasure)
	{
		if (!usedTreasure)
		{
			treasure->Effects();
			usedTreasure = true;
		}
		treasureDurationTimer += dt;
		if (treasureDurationTimer >= treasure->GetDuration())
		{
			usingTreasure = false;
			usedTreasure = false;
			treasureDurationTimer = 0;
			// set back to default values
			Revert();
		}
	}

	if (invincible && !usingTreasure)
	{
		invincibleTimer += dt;
		if (invincibleTimer >= 1.0f)
		{
			invincible = false;
			invincibleTimer = 0;
		}
	}

	if (KeyboardController::GetInstance()->IsKeyPressed('U'))
	{
		onScreenUI = !onScreenUI;
	}

	Constrain();

	if (rotateLeftLeg.z <= -10)
    {
        rotateLLUP = true;
    }
    else if (rotateLeftLeg.z >= 10)
    {
        rotateLLUP = false;
    }

    if (rotateLLUP)
    {
        rotateLeftLeg.z += dt * 100;
        rotateRightLeg.z -= dt * 100;
    }
    else if (!rotateLLUP)
    {
        rotateLeftLeg.z -= dt * 100;
        rotateRightLeg.z += dt * 100;
    }
	if (this->prevHealth != health)
	{
		prevHealth = health;
		healthRatio = health / maxHealth;
		healthScale = (health / maxHealth) * UIScale;
	}
	dashScale = ((dashCooldown - dashCooldownTimer) / dashCooldown) * fontSize;
	dreambarScale = (dreamBar / MAX_DREAMBAR) * UIScale;
	treasureScale = treasure->GetCooldown() - treasureDurationTimer;

	// handling projectile
	if (elapsedTime > timeBetweenShots)
	{
		fire = true;
		elapsedTime = 0.0;
	}
	else
		elapsedTime += dt;

	// DEBUGGING TOOLS
	if (KeyboardController::GetInstance()->IsKeyPressed('O'))
		this->health -= 10;
	if (KeyboardController::GetInstance()->IsKeyPressed('G'))
		this->dreamBar -= 10;
	if (KeyboardController::GetInstance()->IsKeyPressed('H'))
		this->dreamBar += 10;
	if (KeyboardController::GetInstance()->IsKeyPressed('R'))
		this->killCount += 5;
}

void CPlayerInfo::Render(float& _renderOrder)
{
	glEnable(GL_SAMPLE_ALPHA_TO_COVERAGE);

	MS& modelStack = GraphicsManager::GetInstance()->GetModelStack();

	modelStack.PushMatrix();
	modelStack.Translate(position.x, position.y + 1, position.z + 4);
	modelStack.Rotate(90, -1, 0, 0);
	modelStack.Rotate(Math::RadianToDegree(atan2(direction.z, direction.x)), 0, 0, -1);
	modelStack.Scale(15, 12, 0);
	RenderHelper::RenderMesh(MeshBuilder::GetInstance()->GetMesh("laserblaster"));
	modelStack.PopMatrix();

	glEnable(GL_BLEND);
	modelStack.PushMatrix();
	modelStack.Translate(position.x, position.y, position.z);
	modelStack.Rotate(90, -1, 0, 0);
	modelStack.Scale(scale.x, scale.y, scale.z);
	RenderHelper::RenderMesh(currentAnimation);
	modelStack.PopMatrix();

	if (!onScreenUI)
	{
		stats.str("");
		for (int i = 0; i < 5; i++)
			textOBJ[i]->SetText(stats.str());

		stats << killCount << " / " << treasure->GetCooldown() << endl;
		textOBJ[4]->SetScale(Vector3(40, 40, 40));
		textOBJ[4]->SetPosition(Vector3(
			-fontSize * 3.f,
			(Application::GetInstance().GetWindowHeight() * 0.5f) - fontSize,
			0));
		textOBJ[4]->SetText(stats.str());

		// HEALTH
		modelStack.PushMatrix();
		modelStack.Translate(position.x - (UIScale * 0.25f), 0.0f, position.z - 25.f);
		modelStack.Scale(0.5f, 0.5f, 1);
		modelStack.Translate(healthScale * 0.5f, 0, 0);
		modelStack.Scale(healthScale, fontSize * 0.5f, 1);
		if (invincible)
			RenderHelper::RenderMesh(MeshBuilder::GetInstance()->GetMesh("health_invin"));
		else if (healthRatio < 0.25f)
			RenderHelper::RenderMesh(MeshBuilder::GetInstance()->GetMesh("health_quad"));
		else if (healthRatio < 0.5f)
			RenderHelper::RenderMesh(MeshBuilder::GetInstance()->GetMesh("health_half"));
		else
			RenderHelper::RenderMesh(MeshBuilder::GetInstance()->GetMesh("health_full"));
		modelStack.PopMatrix();

		modelStack.PushMatrix();
		modelStack.Translate(position.x - (UIScale * 0.25f), 0.0f, position.z - 25.f);
		modelStack.Scale(0.5f, 0.5f, 1);
		modelStack.Translate(UIScale * 0.5f, 0, 0);
		modelStack.Scale(UIScale, fontSize * 0.5f, 1);
		RenderHelper::RenderMesh(MeshBuilder::GetInstance()->GetMesh("border"));
		modelStack.PopMatrix();

		// DREAMBAR
		modelStack.PushMatrix();
		modelStack.Translate(position.x - (UIScale * 0.25f), 0.0f, position.z - 15.f);
		modelStack.Scale(0.5f, 0.5f, 1);
		modelStack.Translate(dreambarScale * 0.5f, 0, 0);
		modelStack.Scale(dreambarScale, fontSize * 0.5f, 1);
		RenderHelper::RenderMesh(MeshBuilder::GetInstance()->GetMesh("dreambar"));
		modelStack.PopMatrix();

		modelStack.PushMatrix();
		modelStack.Translate(position.x - (UIScale * 0.25f), 0.0f, position.z - 15.f);
		modelStack.Scale(0.5f, 0.5f, 1);
		modelStack.Translate(UIScale * 0.5f, 0, 0);
		modelStack.Scale(UIScale, fontSize * 0.5f, 1);
		RenderHelper::RenderMesh(MeshBuilder::GetInstance()->GetMesh("border"));
		modelStack.PopMatrix();
	}

    glDisable(GL_SAMPLE_ALPHA_TO_COVERAGE);

    //for (int x = 0; x <= CLevel::GetInstance()->GetRoom(roomID)->GetRoomXMax(); ++x)
    //{
    //    for (int z = 0; z <= CLevel::GetInstance()->GetRoom(roomID)->GetRoomZMax(); ++z)
    //    {
    //        GraphicsManager::GetInstance()->GetModelStack().PushMatrix();
    //        Vector3 temp = CLevel::GetInstance()->GetRoom(roomID)->GetSpatialPartition()->GetGridPos(x, z);
    //        Vector3 dir = heatmap[x][z].GetDir();
    //        GraphicsManager::GetInstance()->GetModelStack().Translate(temp.x, 1, temp.z);
    //        GraphicsManager::GetInstance()->GetModelStack().Rotate(-90, 1, 0, 0);
    //        GraphicsManager::GetInstance()->GetModelStack().Rotate(Math::RadianToDegree(atan2(dir.x, dir.z)), 0, 0, 1);
    //        GraphicsManager::GetInstance()->GetModelStack().Scale(scale.x * 0.5f, scale.y* 0.5f, scale.z* 0.5f);
    //        RenderHelper::RenderMesh(MeshBuilder::GetInstance()->GetMesh("direction"));
    //        GraphicsManager::GetInstance()->GetModelStack().PopMatrix();
    //    }
    //}
}

void CPlayerInfo::RenderUI(void)
{
	MS& modelStack = GraphicsManager::GetInstance()->GetModelStack();
	if (onScreenUI)
	{
		for (int i = 0; i < 5; ++i)
		{
			textOBJ[i]->SetScale(Vector3(fontSize, fontSize, fontSize));
			textOBJ[i]->SetPosition(Vector3(
				-Application::GetInstance().GetWindowWidth() * 0.5f + fontSize * 2,
				(Application::GetInstance().GetWindowHeight() * 0.5f) - (fontSize * (i * 1.5f)) - fontSize,
				2.0f));
		}

		stats.precision(3);
		stats.str("");
		stats << health << " / " << maxHealth << endl;
		textOBJ[0]->SetText(stats.str());
		stats.str("");
		stats << dreamBar << " / " << MAX_DREAMBAR << endl;
		textOBJ[1]->SetText(stats.str());
		stats.str("");
		stats << maxSpeed << endl;
		textOBJ[2]->SetText(stats.str());
		stats.str("");
		textOBJ[3]->SetText(treasure->GetName());
		stats.str("");
		stats << killCount << " / " << treasure->GetCooldown() << endl;
		textOBJ[4]->SetText(stats.str());

		// HEALTH
		modelStack.PushMatrix();
		modelStack.Translate(
			-Application::GetInstance().GetWindowWidth() * 0.5f + fontSize,
			(Application::GetInstance().GetWindowHeight() * 0.5f) - fontSize,
			0.5f);
		modelStack.Scale(fontSize, fontSize, 1);
		RenderHelper::RenderMesh(MeshBuilder::GetInstance()->GetMesh("healthicon"));
		modelStack.PopMatrix();

		modelStack.PushMatrix();
		modelStack.Translate(
			-Application::GetInstance().GetWindowWidth() * 0.5f + (fontSize * 4.0f),
			(Application::GetInstance().GetWindowHeight() * 0.5f) - fontSize,
			1);
		modelStack.Translate(healthScale - fontSize * 2, 0, 0);
		modelStack.Scale(2, 2, 2);
		modelStack.Scale(healthScale, fontSize * 0.5f, 1);
		float healthRatio = this->health / this->maxHealth;
		if (invincible)
			RenderHelper::RenderMesh(MeshBuilder::GetInstance()->GetMesh("health_invin"));
		else if (healthRatio < 0.25f)
			RenderHelper::RenderMesh(MeshBuilder::GetInstance()->GetMesh("health_quad"));
		else if (healthRatio < 0.5f)
			RenderHelper::RenderMesh(MeshBuilder::GetInstance()->GetMesh("health_half"));
		else
			RenderHelper::RenderMesh(MeshBuilder::GetInstance()->GetMesh("health_full"));
		modelStack.PopMatrix();

		modelStack.PushMatrix();
		modelStack.Translate(
			-Application::GetInstance().GetWindowWidth() * 0.5f + (fontSize * 4.0f),
			(Application::GetInstance().GetWindowHeight() * 0.5f) - fontSize,
			0.5f);
		modelStack.Translate(UIScale - fontSize * 2, 0, 0);
		modelStack.Scale(2, 2, 2);
		modelStack.Scale(UIScale, fontSize * 0.5f, 1);
		RenderHelper::RenderMesh(MeshBuilder::GetInstance()->GetMesh("border"));
		modelStack.PopMatrix();

		// DREAMBAR
		modelStack.PushMatrix();
		modelStack.Translate(
			-Application::GetInstance().GetWindowWidth() * 0.5f + fontSize,
			(Application::GetInstance().GetWindowHeight() * 0.5f) - fontSize * 2.5f,
			0.5f);
		modelStack.Scale(fontSize, fontSize, 1);
		RenderHelper::RenderMesh(MeshBuilder::GetInstance()->GetMesh("dreambaricon"));
		modelStack.PopMatrix();

		modelStack.PushMatrix();
		modelStack.Translate(
			-Application::GetInstance().GetWindowWidth() * 0.5f + (fontSize * 4.0f),
			(Application::GetInstance().GetWindowHeight() * 0.5f) - fontSize * 2.5f,
			1);
		modelStack.Translate(dreambarScale - fontSize * 2, 0, 0);
		modelStack.Scale(2, 2, 2);
		modelStack.Scale(dreambarScale, fontSize * 0.5f, 1);
		RenderHelper::RenderMesh(MeshBuilder::GetInstance()->GetMesh("dreambar"));
		modelStack.PopMatrix();

		modelStack.PushMatrix();
		modelStack.Translate(
			-Application::GetInstance().GetWindowWidth() * 0.5f + (fontSize * 4.0f),
			(Application::GetInstance().GetWindowHeight() * 0.5f) - fontSize * 2.5f,
			0.5f);
		modelStack.Translate(UIScale - fontSize * 2, 0, 0);
		modelStack.Scale(2, 2, 2);
		modelStack.Scale(UIScale, fontSize * 0.5f, 1);
		RenderHelper::RenderMesh(MeshBuilder::GetInstance()->GetMesh("border"));
		modelStack.PopMatrix();

		// speed
		modelStack.PushMatrix();
		modelStack.Translate(
			-Application::GetInstance().GetWindowWidth() * 0.5f + fontSize,
			(Application::GetInstance().GetWindowHeight() * 0.5f) - fontSize * 4,
			0.5f);
		modelStack.Scale(fontSize, fontSize, 1);
		RenderHelper::RenderMesh(MeshBuilder::GetInstance()->GetMesh("speedicon"));
		modelStack.PopMatrix();

		// TREASURE
		modelStack.PushMatrix();
		modelStack.Translate(
			-Application::GetInstance().GetWindowWidth() * 0.5f + fontSize,
			(Application::GetInstance().GetWindowHeight() * 0.5f) - fontSize * 5.5f,
			0.5f);
		modelStack.Scale(fontSize, fontSize, 1);
		RenderHelper::RenderMesh(MeshBuilder::GetInstance()->GetMesh("treasureicon"));
		modelStack.PopMatrix();

		modelStack.PushMatrix();
		modelStack.Translate(
			-Application::GetInstance().GetWindowWidth() * 0.5f + (fontSize * 4.0f),
			(Application::GetInstance().GetWindowHeight() * 0.5f) - fontSize * 5.5f,
			1);
		modelStack.Translate(treasureScale - fontSize * 2, 0, 0);
		modelStack.Scale(2, 2, 2);
		modelStack.Scale(treasureScale, fontSize * 0.5f, 1);
		RenderHelper::RenderMesh(MeshBuilder::GetInstance()->GetMesh("dreambar"));
		modelStack.PopMatrix();
	}
	// DASH
	modelStack.PushMatrix();
	modelStack.Translate(
		Application::GetInstance().GetWindowWidth() * 0.5f - fontSize * 2,
		(-Application::GetInstance().GetWindowHeight() * 0.5f) + fontSize * 2,
		1.5f);
	modelStack.Scale(fontSize, fontSize, 1);
	RenderHelper::RenderMesh(MeshBuilder::GetInstance()->GetMesh("dashicon"));
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(
		Application::GetInstance().GetWindowWidth() * 0.5f - fontSize * 2,
		(-Application::GetInstance().GetWindowHeight() * 0.5f) + fontSize * 2,
		1);
	modelStack.Translate(0, dashScale - fontSize, 0);
	modelStack.Scale(2, 2, 2);
	modelStack.Scale(fontSize, dashScale, 1);
	RenderHelper::RenderMesh(MeshBuilder::GetInstance()->GetMesh("dash"));
	modelStack.PopMatrix();
}

// Constrain the position within the borders
void CPlayerInfo::Constrain(void)
{
	// Constrain player within the boundary
    if (position.x > maxBoundary.x - 1.0f)
    {
        position.x = maxBoundary.x - 1.0f; 
        velocity.x = 0;
    }
    if (position.z > maxBoundary.z - 1.0f)
    {
        position.z = maxBoundary.z - 1.0f; 
        velocity.z = 0;
    }
    if (position.x < minBoundary.x + 1.0f)
    {
        position.x = minBoundary.x + 1.0f;
        velocity.x = 0;
    }
    if (position.z < minBoundary.z + 1.0f)
    {
        position.z = minBoundary.z + 1.0f;
        velocity.z = 0;
    }
}

void CPlayerInfo::Shoot(Vector3 dir)
{
	if (fire)
	{
		Vector3 _direction = (dir - position).Normalized();
		Create::Laser(
			"laser",
			position,
			_direction,
			100.0f,
			2.0f,
			500.0f,
			damage);
		fire = false;
	}
}

void CPlayerInfo::RecoverHealth()
{ 
	if (health + 10 > maxHealth)
		health = maxHealth;
	else
		health += 10;
}

void CPlayerInfo::AddTreasures(int type)
{
	treasure->treasure_type = (Treasure::TREASURES_TYPE)type;
	this->treasure->SetValues();
}

void CPlayerInfo::Revert()
{
	healthregenCooldown = defaultHealthRegenCooldown;
	maxSpeed = defaultSpeed;
	damage = defaultDamage;
	timeBetweenShots = defaultFirerate;
}

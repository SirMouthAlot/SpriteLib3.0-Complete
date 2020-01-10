#include "ExampleBaseScene.h"
#include "Utilities.h"
#include "Timer.h"
#include "EffectManager.h"

ExampleBaseScene::ExampleBaseScene(std::string name)
	: Scene(name)
{
	//No gravity, this is top down
	m_gravity = b2Vec2(0.f, 0.f);
	m_physicsWorld->SetGravity(m_gravity);
}

void ExampleBaseScene::InitScene(float windowWidth, float windowHeight)
{
	//Effect stuffs
	EffectManager::CreateEffect(EffectType::Vignette, BackEnd::GetWindowWidth(), BackEnd::GetWindowHeight());
	VignetteEffect* effect = (VignetteEffect*)EffectManager::GetEffect(EffectManager::GetVignetteHandle());

	effect->SetOpacity(1.f);
	effect->SetInnerRadius(0.f);
	effect->SetOuterRadius(0.870f);

	//Dynamically allocates the register
	m_sceneReg = new entt::registry;

	//Attach the register
	ECS::AttachRegister(m_sceneReg);

	//Sets up aspect ratio for the camera
	float aspectRatio = windowWidth / windowHeight;

	//Setup Main Camera entity
	{
		//Creates Camera entity
		auto entity = ECS::CreateEntity();

		//Creates new orthographic camera
		ECS::AttachComponent<Camera>(entity);
		ECS::AttachComponent<HorizontalScroll>(entity);
		ECS::AttachComponent<VerticalScroll>(entity);
		//ECS::AttachComponent<HorizontalScroll>(entity);
		vec4 temp = vec4(-75.f, 75.f, -75.f, 75.f);
		ECS::GetComponent<Camera>(entity).SetOrthoSize(temp);
		ECS::GetComponent<Camera>(entity).SetWindowSize(vec2(float(windowWidth), float(windowHeight)));
		ECS::GetComponent<Camera>(entity).Orthographic(aspectRatio, temp.x, temp.y, temp.z, temp.w, -100.f, 100.f);

		//attaches the camera to the vertical scroll
		ECS::GetComponent<HorizontalScroll>(entity).SetCam(&ECS::GetComponent<Camera>(entity));
		ECS::GetComponent<VerticalScroll>(entity).SetCam(&ECS::GetComponent<Camera>(entity));

		//Sets up the Identifier
		unsigned int bitHolder = EntityIdentifier::CameraBit();
		ECS::SetUpIdentifier(entity, bitHolder, "Horizontal Scrolling Cam");
		ECS::SetIsMainCamera(entity, true);
	}

	//Setup new Entity
	{
		//Creates entity
		auto entity = ECS::CreateEntity();

		//Add components
		ECS::AttachComponent<Sprite>(entity);
		ECS::AttachComponent<Transform>(entity);
		ECS::AttachComponent<PhysicsBody>(entity);

		//Sets up components
		std::string fileName = "boxSprite.jpg";
		ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 30, 30);
		ECS::GetComponent<Transform>(entity).SetPosition(vec3(0.f, 0.f, 100.f));

		auto& tempSpr = ECS::GetComponent<Sprite>(entity);
		auto& tempPhsBody = ECS::GetComponent<PhysicsBody>(entity);

		//Physics body covers the entire sprite
		//ID type is player
		//Collides with Enemies and Environments
		//Moves
		float shrinkX = 0.f;
		float shrinkY = 0.f;

		b2Body* tempBody;
		b2BodyDef tempDef;
		tempDef.type = b2_dynamicBody;
		tempDef.position.Set(float32(-50.f), float32(5.f));

		tempBody = m_physicsWorld->CreateBody(&tempDef);

		tempPhsBody = PhysicsBody(tempBody, float(tempSpr.GetWidth() - shrinkX), float(tempSpr.GetHeight() - shrinkY),
			vec2(0.f, 0.f), false);

		//Sets up the Identifier
		unsigned int bitHolder = EntityIdentifier::SpriteBit() | EntityIdentifier::TransformBit() | EntityIdentifier::PhysicsBit();
		ECS::SetUpIdentifier(entity, bitHolder, "Main Box");
		ECS::SetIsMainPlayer(entity, true);
	}

	//Setup new Entity
	{
		//Creates entity
		auto entity = ECS::CreateEntity();

		//Add components
		ECS::AttachComponent<Sprite>(entity);
		ECS::AttachComponent<Transform>(entity);

		//Sets up components
		std::string fileName = "50Blessings.png";
		ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 100, 45);
		ECS::GetComponent<Transform>(entity).SetPosition(vec3(0.f, 0.f, 0.f));
	
		//Sets up the Identifier
		unsigned int bitHolder = EntityIdentifier::SpriteBit() | EntityIdentifier::TransformBit();
		ECS::SetUpIdentifier(entity, bitHolder, "Hotline Miami Logo");
	}

	//Makes the camera focus on this entity
	ECS::GetComponent<HorizontalScroll>(EntityIdentifier::MainCamera()).SetFocus(&ECS::GetComponent<Transform>(EntityIdentifier::MainPlayer()));
	ECS::GetComponent<VerticalScroll>(EntityIdentifier::MainCamera()).SetFocus(&ECS::GetComponent<Transform>(EntityIdentifier::MainPlayer()));
}

void ExampleBaseScene::Update()
{

	Aiming(m_mouseGL);

	AdjustScrollOffset();

	RainbowBackground();
}



void ExampleBaseScene::RainbowBackground()
{
	//Makes clear color smoothly blend between two colors, going back and forth.
	m_clearColor = Util::Lerp<vec4>(m_clearColor1, m_clearColor2, m_lerpVal);

	if (m_lerpVal >= 1.f)
	{
		vec4 temp = m_clearColor2;

		m_clearColor2 = m_clearColor1;
		m_clearColor1 = temp;

		m_lerpVal = 0.f;
	}

	m_lerpVal += Timer::deltaTime / m_repeatTime;
}

void ExampleBaseScene::AdjustScrollOffset()
{
	float maxSizeX = ECS::GetComponent<Camera>(EntityIdentifier::MainCamera()).GetOrthoSize().y;
	float maxSizeY = ECS::GetComponent<Camera>(EntityIdentifier::MainCamera()).GetOrthoSize().w;

	float playerHalfSize = ECS::GetComponent<Sprite>(EntityIdentifier::MainPlayer()).GetWidth() / 2.f;

	ECS::GetComponent<HorizontalScroll>(EntityIdentifier::MainCamera()).SetOffset((maxSizeX * BackEnd::GetAspectRatio()) - playerHalfSize);
	ECS::GetComponent<VerticalScroll>(EntityIdentifier::MainCamera()).SetOffset(maxSizeY - playerHalfSize);

}

void ExampleBaseScene::Aiming(vec2 mousePosition)
{
	vec3 playerLoc = ECS::GetComponent<Transform>(EntityIdentifier::MainPlayer()).GetPosition();

	vec2 convertedMouse = mousePosition;

	float dx = playerLoc.x - convertedMouse.x;
	float dy = playerLoc.y - convertedMouse.y;

	float playerDir = (atan2(dy, dx));

	if (ECS::GetComponent<EntityIdentifier>(EntityIdentifier::MainPlayer()).GetIdentifier()& EntityIdentifier::PhysicsBit())
	{
		auto body = ECS::GetComponent<PhysicsBody>(EntityIdentifier::MainPlayer()).GetBody();
		body->SetTransform(body->GetPosition(), playerDir - Transform::ToRadians(90.f));
	}
	else
	{
		ECS::GetComponent<Transform>(EntityIdentifier::MainPlayer()).SetRotationAngleZ(playerDir - Transform::ToRadians(90.f));
	}
}

void ExampleBaseScene::KeyboardHold()
{
	if (ECS::GetComponent<EntityIdentifier>(EntityIdentifier::MainPlayer()).GetIdentifier()& EntityIdentifier::PhysicsBit())
	{
		auto body = ECS::GetComponent<PhysicsBody>(EntityIdentifier::MainPlayer()).GetBody();

		b2Vec2 velocity = b2Vec2(0.f, 0.f);

		float inputVal = 15.f;

		if (Input::GetKey(Key::W))
		{
			velocity += b2Vec2(0.f, inputVal);
		}
		if (Input::GetKey(Key::S))
		{
			velocity += b2Vec2(0.f, -inputVal);
		}

		if (Input::GetKey(Key::A))
		{
			velocity += b2Vec2(-inputVal, 0.f);
		}
		if (Input::GetKey(Key::D))
		{
			velocity += b2Vec2(inputVal, 0.f);
		}

		body->SetLinearVelocity(velocity);
	}
}

void ExampleBaseScene::MouseMotion(SDL_MouseMotionEvent evnt)
{
	m_mouseGL = Util::ConvertToGL(m_sceneReg, vec2(float(evnt.x), float(evnt.y)));
}

void ExampleBaseScene::MouseClick(SDL_MouseButtonEvent evnt)
{
}

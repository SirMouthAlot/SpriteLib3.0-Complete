#ifndef __SCENE_H__
#define __SCENE_H__

#include "JSON.h"
#include "ECS.h"
#include "Xinput.h"
#include <SDL2/SDL.h>


class Scene
{
public:
	Scene() { m_sceneReg = new entt::registry(); m_physicsWorld = new b2World(m_gravity); }
	Scene(std::string name);
	~Scene() { }

	void Unload();
	//Saves the scene
	void SaveScene();

	//Each scene will need to have a different
	//init, as each scene's contents will be different
	virtual void InitScene(float windowWidth, float windowHeight);

	virtual void Update() {}

	//Gamepad Input
	//Because these are virtual you can override them in your inherited classes.
	//The same way you do for Update().
	virtual void GamepadStroke(XInputController* con) { };
	virtual void GamepadUp(XInputController* con) { };
	virtual void GamepadDown(XInputController* con) { };
	virtual void GamepadStick(XInputController* con) { };
	virtual void GamepadTrigger(XInputController* con) { };
	virtual void KeyboardHold() { };
	virtual void KeyboardDown() { };
	virtual void KeyboardUp() { };

	//Mouse Input
	//Because these are virtual you can override them in your inherited classes.
	//The same way you do for Update().
	virtual void MouseMotion(SDL_MouseMotionEvent evnt) { };
	virtual void MouseClick(SDL_MouseButtonEvent evnt) { };
	virtual void MouseWheel(SDL_MouseWheelEvent evnt) { };

	//Get the scene registry
	entt::registry* GetScene() const;
	//Set the scene registry
	void SetScene(entt::registry& scene);

	//Gets the background color of the scene
	vec4 GetClearColor() const;
	//Sets the background color of the scene
	void SetClearColor(vec4 clearColor);

	//Gets the name of the scene
	std::string GetName() const;
	//Sets the name of the scene
	void SetName(std::string name);
	
	//Gravity
	b2Vec2 GetGravity() const;
	void SetGravity(b2Vec2 grav);
	//Physics world
	b2World& GetPhysicsWorld();

	//Set window size (makes sure the camera aspect is proper)
	void SetWindowSize(float windowWidth, float windowHeight);
protected:
	b2World* m_physicsWorld = nullptr;
	b2Vec2 m_gravity = b2Vec2(float32(0.f), float32(0.f));

	vec4 m_clearColor = vec4(0.15f, 0.33f, 0.58f, 1.f);

	entt::registry* m_sceneReg = nullptr;	
	std::string m_name = "Default Name";
};

#ifndef __SCENEJSON_H__
#define __SCENEJSON_H__

//Sends animation TO json file
inline void to_json(nlohmann::json& j, const Scene& scene)
{
	j["SceneName"] = scene.GetName();

	auto view = scene.GetScene()->view<EntityIdentifier>();
	
	//keeps count of how many entities were stored
	unsigned int counter = 0;

	for (auto entity : view)
	{
		//Stores the entity identifier
		j[std::to_string(counter)]["Identifier"] = view.get(entity);

		//Identifier used to check what components are within this entity
		unsigned int identity = view.get(entity).GetIdentifier();
		
		//If Identity includes the Camera bit
			//This means the entity contains a camera
		if (identity & EntityIdentifier::CameraBit())
		{
			//Stores the camera
			j[std::to_string(counter)]["Camera"] = scene.GetScene()->get<Camera>(entity);
		}

		//If Identity includes the Sprite bit
			//This means the entity contains a Sprite
		if (identity & EntityIdentifier::SpriteBit())
		{
			//Stores the sprite
			j[std::to_string(counter)]["Sprite"] = scene.GetScene()->get<Sprite>(entity);
		}

		//If Identity includes the Animation Controller bit
			//This means that the entity contains an Animation Controller
		if (identity & EntityIdentifier::AnimationBit())
		{
			//Stores the Animation controllers
			j[std::to_string(counter)]["AnimationController"] = scene.GetScene()->get<AnimationController>(entity);
		}

		//If Identity includes the Transform bit
			//This means that the entity contains a Transform
		if (identity & EntityIdentifier::TransformBit())
		{
			//Stores the transform
			j[std::to_string(counter)]["Transform"] = scene.GetScene()->get<Transform>(entity);
		}

		if (identity & EntityIdentifier::PhysicsBit())
		{
			//Stores the physics body
			j[std::to_string(counter)]["PhysicsBody"] = scene.GetScene()->get<PhysicsBody>(entity);
		}

		//If you create new classes that you add as a component,
		//you need to #1 add a static (unique) bit for that class
		//And then add more if statements after this point

		if (identity & EntityIdentifier::HoriScrollCameraBit())
		{
			//Stores the horizontal scrolling camera
			j[std::to_string(counter)]["HoriScrollCam"] = scene.GetScene()->get<HorizontalScroll>(entity);
		}

		if (identity & EntityIdentifier::VertScrollCameraBit())
		{
			//Stores the vertical scrolling camera
			j[std::to_string(counter)]["VertScrollCam"] = scene.GetScene()->get<VerticalScroll>(entity);
		}

		//For each loop increase the counter
		counter++;
	}

	//Stores the number of entities (for your loops)
	j["NumEntities"] = counter;
}

//Reads animation in FROM json file
inline void from_json(const nlohmann::json& j, Scene& scene)
{
	//Stores number of entities
	unsigned int numEntities = j["Scene"]["NumEntities"];

	//Sets the scene name (so you can save over the same file)
	scene.SetName(j["Scene"]["SceneName"]);

	//Reference to the registry
	auto &reg = *scene.GetScene();

	//Is there a horizontal scroll?
	bool scrollHori = false;

	//Is there a vertical scroll?
	bool scrollVert = false;
	
	//Allows you to create each entity
	for (unsigned i = 0; i < numEntities; i++)
	{
		//Has animation
		bool hasAnim = false;

		//Creates new entity
		auto entity = reg.create();
		reg.assign<EntityIdentifier>(entity);
		//Sets the entity identifier correctly
		reg.get<EntityIdentifier>(entity) = j["Scene"][std::to_string(i)]["Identifier"];
		reg.get<EntityIdentifier>(entity).SetEntity(entity);
		
		//Just check if this entity is the main camera
		if (reg.get<EntityIdentifier>(entity).GetIsMainCamera())
		{
			//Sets main camera
			EntityIdentifier::MainCamera(entity);
		}
		if (reg.get<EntityIdentifier>(entity).GetIsMainPlayer())
		{
			//Sets main player
			EntityIdentifier::MainPlayer(entity);
		}

		unsigned int identity = reg.get<EntityIdentifier>(entity).GetIdentifier();

		//If Identity includes the Camera bit
			//This means the entity contains a camera
		if (identity & EntityIdentifier::CameraBit())
		{
			//Adds camera to the entity
			reg.assign<Camera>(entity);
			//Sets the camera to the saved version
			reg.get<Camera>(entity) = j["Scene"][std::to_string(i)]["Camera"];
		}

		//If Identity includes the Animation Controller bit
			//This means that the entity contains an Animation Controller
		if (identity & EntityIdentifier::AnimationBit())
		{
			//Adds Animation Controller to the entity
			reg.assign<AnimationController>(entity);
			//Sets the Animation Controller to the saved version
			reg.get<AnimationController>(entity) = j["Scene"][std::to_string(i)]["AnimationController"];

			hasAnim = true;

			//Animation controllers requires that we init the UVs with the filename
			reg.get<AnimationController>(entity).InitUVs(reg.get<AnimationController>(entity).GetFileName());
		}

		//If Identity includes the Sprite bit
			//This means the entity contains a Sprite
		if (identity & EntityIdentifier::SpriteBit())
		{
			AnimationController* anim = nullptr;
			if (hasAnim)
				anim = &reg.get<AnimationController>(entity);
			
			//Adds sprite to the entity
			reg.assign<Sprite>(entity);
			//Sets the sprite to the saved version
			reg.get<Sprite>(entity) = j["Scene"][std::to_string(i)]["Sprite"];

			//Sprite requires that we load in the sprite
			std::string fileName = reg.get<Sprite>(entity).GetFileName();
			reg.get<Sprite>(entity).LoadSprite(fileName, reg.get<Sprite>(entity).GetWidth(), reg.get<Sprite>(entity).GetHeight(), hasAnim, anim);
			if (reg.get<Sprite>(entity).GetIsSpriteSheet())
			{
				reg.get<Sprite>(entity).SetUVs(reg.get<Sprite>(entity).GetBottomLeft(), reg.get<Sprite>(entity).GetTopRight());
			}
		}

		//If Identity includes the Transform bit
			//This means that the entity contains a Transform
		if (identity & EntityIdentifier::TransformBit())
		{
			//Adds transform to the entity
			reg.assign<Transform>(entity);
			//Sets the transform to the saved version
			reg.get<Transform>(entity) = j["Scene"][std::to_string(i)]["Transform"];

			//Transforms require no further initialization
		}

		if (identity & EntityIdentifier::PhysicsBit())
		{
			//Adds physics body to entity
			reg.assign<PhysicsBody>(entity);
			//Sets the physics body to the saved version
			reg.get<PhysicsBody>(entity) = j["Scene"][std::to_string(i)]["PhysicsBody"];
		}

		//If Identity includes the vertical scrolling camera bit
			//This means that the entity contains a vertical scrolling camera
		if (identity & EntityIdentifier::HoriScrollCameraBit())
		{
			//Adds vertical scrolling camera to the entity
			reg.assign<HorizontalScroll>(entity);
			//Sets the vertical scrolling camera to our saved version
			reg.get<HorizontalScroll>(entity) = j["Scene"][std::to_string(i)]["HoriScrollCam"];

			scrollHori = true;
		}

		//If Identity includes the vertical scrolling camera bit
			//This means that the entity contains a vertical scrolling camera
		if (identity & EntityIdentifier::VertScrollCameraBit())
		{
			//Adds vertical scrolling camera to the entity
			reg.assign<VerticalScroll>(entity);
			//Sets the vertical scrolling camera to our saved version
			reg.get<VerticalScroll>(entity) = j["Scene"][std::to_string(i)]["VertScrollCam"];

			scrollVert = true;
		}
	}

	if (scrollHori)
	{
		//attaches the vertical scroll to the camera within the entity it's attached to
		reg.get<HorizontalScroll>(EntityIdentifier::MainCamera()).SetCam(&reg.get<Camera>(EntityIdentifier::MainCamera()));
		//Makes the camera focus on the MainPlayer
		reg.get<HorizontalScroll>(EntityIdentifier::MainCamera()).SetFocus(&reg.get<Transform>(EntityIdentifier::MainPlayer()));
	}
	if (scrollVert)
	{
		//attaches the vertical scroll to the camera within the entity it's attached to
		reg.get<VerticalScroll>(EntityIdentifier::MainCamera()).SetCam(&reg.get<Camera>(EntityIdentifier::MainCamera()));
		//Makes the camera focus on the MainPlayer
		reg.get<VerticalScroll>(EntityIdentifier::MainCamera()).SetFocus(&reg.get<Transform>(EntityIdentifier::MainPlayer()));
	}
}

#endif // !__TRANSFORMJSON_H__

#endif // !__SCENE_H__

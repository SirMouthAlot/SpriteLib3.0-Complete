#pragma once

#include "ECS.h"
#include "PhysicsBody.h"
#include "Timer.h"
#include "Shader.h"

struct Box
{
public:
	vec3 m_bottomLeft;
	vec3 m_bottomRight;
	vec3 m_topLeft;
	vec3 m_topRight;
	vec3 m_center;
};

struct Circle
{
public:
	vec3 m_center;
	float m_radius;
};

namespace PhysicsSystem
{
	static Shader physicsDrawShader;

	//Initalizes the system
	void Init();

	//Update the system
	void Update(entt::registry* reg, b2World& world);

	//Draw all entities with physics bodies
	//*only if the static "m_drawBodies" is true
	void Draw(entt::registry* reg);

	//Run our collision detection (will run different collision detection based on bodytype
	//Includes:
	//Box-Circle
	//Box-Box
	void Run(b2World& world);

	//Run our Box-Circle Collision detection system.
	bool BoxCircleCollision(std::pair<PhysicsBody&, Circle> group1, std::pair<PhysicsBody&, Box> group2);

	//Run our Box-Box Collision detection system
	bool BoxBoxCollision(std::pair<PhysicsBody&, Box> group1, std::pair<PhysicsBody&, Box> group2);
}
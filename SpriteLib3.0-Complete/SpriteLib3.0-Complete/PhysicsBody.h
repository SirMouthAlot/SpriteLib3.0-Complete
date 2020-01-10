#pragma once

#include <GL/glew.h>
#include <Box2D/Box2D.h>

#include "JSON.h"
#include "Vector.h"
#include "VertexManager.h"
#include "EntityIdentifier.h"
#include "Transform.h"

enum class BodyType
{
	BOX,
	CIRCLE,

	NUM_TYPES
};

struct CollisionIDs
{
public:
	//Gets the different IDs
	static unsigned int Player();
	static unsigned int Environment();
	static unsigned int Enemy();
private:
	//Holds the different IDs
	static unsigned int m_playerID;
	static unsigned int m_environmentID;
	static unsigned int m_enemyID;
};


class PhysicsBody
{
public:
	PhysicsBody() { };
	//Constructs a circle collider
	PhysicsBody(b2Body* body, float radius, vec2 centerOffset, bool isDynamic);
	//Constructs a box collider
	PhysicsBody(b2Body* body, float width, float height, vec2 centerOffset, bool isDynamic);

	void DeleteBody();

	//Initializes body for drawing
	void InitBody();

	void DrawBody();

	//Update physics stuffs
	void Update(Transform* trans);

	//Apply a force to the physics body
	void ApplyForce(vec3 force);
	//Add a new type of object to collide with
	void AddCollideID(unsigned int collideID);

	//Getters
	//Get the Box2D physics body
	b2Body* GetBody() const;
	//Get position of body
	b2Vec2 GetPosition() const;
	//Gets current force
	vec3 GetForce() const;
	//Gets the current acceleration
	vec3 GetAcceleration() const;
	//Gets the max velocity for each individual axis
	float GetMaxVelo() const;
	//Gets the current velocity
	vec3 GetVelocity() const;
	//Gets the coefficient of friction
	float GetFriction() const;

	//What is the current gravitational acceleration?
	static vec3 GetGravityAcceleration();
	//Is gravity applying to this object?
	bool GetGravity() const;

	//Set the mass of the phyics body
	float GetMass() const;

	//Get the body type enum
	BodyType GetBodyType() const;
	//Gets the center offset for the body
	//*if the offset is 0,0, then all corners will be relative to the
	//center of the actual sprite
	vec2 GetCenterOffset() const;
	//Gets the corners of the body
	vec2 GetBottomLeft() const;
	vec2 GetBottomRight() const;
	vec2 GetTopLeft() const;
	vec2 GetTopRight() const;
	//Gets the width of the physics body
	float GetWidth() const;
	//Gets the height of the physics body
	float GetHeight() const;
	//Gets the radius of the physics body
	float GetRadius() const;
	
	//Gets this body's type 
	//*Can be environment, enemy, player, friendly
	unsigned int GetBodyID() const;
	//What types does this body collide with?
	//*Can be environment, enemy, player, and friendly
	unsigned int GetCollideID() const;

	//Get whether or not this object is dynamic (aka moves)
	bool GetDynamic() const;

	//Get whether or not we are currently drawing our physics bodies
	static bool GetDraw();

	//Setters
	//Sets the pointer to the box2D body
	void SetBody(b2Body* body);
	//Set position (just sets the variable, doesn't actually set the position)
	void SetPosition(b2Vec2 bodyPos);
	//Sets force to be applied to this body
	void SetForce(vec3 force);
	//Sets the acceleration to be applied to this body
	void SetAcceleration(vec3 accel);
	//Sets Max Velocity for the individual axes
	void SetMaxVelo(float velo);
	//Sets the velocity of the phyiscs body
	void SetVelocity(vec3 velo);

	//Sets the coefficient of friction
	void SetFriction(float fric);

	//Set the gravitational acceleration
	static void SetGravityAcceleration(vec3 grav);
	//Set whether this body is affected by gravity
	void SetGravity(bool grav);

	//Set the mass of the physics body
	void SetMass(float mass);

	//If other body types were ever implemented we could set it here 
	//(obviously different types wouldn't use the whole BL, BR, TL, TR
	void SetBodyType(BodyType type);
	//Sets the center offset for the body
	//*if the offset is 0,0, then all corners will be relative to the
	//center of the actual sprite	void SetCenterOffset(vec2 cent);
	void SetCenterOffset(vec2 cent);
	//Sets the corners of the body
	void SetBottomLeft(vec2 BL);
	void SetBottomRight(vec2 BR);
	void SetTopLeft(vec2 TL);
	void SetTopRight(vec2 TR);
	//Sets the width of the body
	void SetWidth(float width);
	//Sets the height of the body
	void SetHeight(float height);
	//Sets the radius of the body
	void SetRadius(float radius);
	//Set whether the bodies are being drawn
	static void SetDraw(bool drawBodies);

	//Body ID
	void SetBodyID(unsigned int bodyID);
	//Collide ID
	void SetCollideID(unsigned int collideID);

	//Does the object not move?
	void SetDynamic(bool isDynamic);

private:
	//The actual box2D body
	b2Body* m_body = nullptr;
	b2Vec2 m_position = b2Vec2(0.f, 0.f);

	//The applied force
	vec3 m_appliedForce = vec3(0.f, 0.f, 0.f);
	//The frictional force
	vec3 m_frictionForce = vec3(0.f, 0.f, 0.f);
	//Stores the net force
	vec3 m_netForce = vec3(0.f, 0.f, 0.f);
	//Stores the acceleration
	vec3 m_acceleration = vec3(0.f, 0.f, 0.f);
	//Max velocity in each axes
	float m_maxVelo = 20.f;
	//Stores the velocity
	vec3 m_velocity = vec3(0.f, 0.f, 0.f);

	//Coefficient of friction
	float m_friction = 0.35f;

	//The acceleration due to gravity * mass
	static vec3 m_gravityAcceleration;
	//Is gravity being applied to this body?
	bool m_applyGravity = false;

	//Mass of the body
	float m_mass = 1.f;

	//Body type
	BodyType m_bodyType = BodyType::CIRCLE;
	//How far from the center of the sprite is it
	vec2 m_centerOffset = vec2();
	//Each corner
	vec2 m_bottomLeft = vec2();
	vec2 m_bottomRight = vec2();
	vec2 m_topLeft = vec2();
	vec2 m_topRight = vec2();
	//Width and height of the body
	float m_width = 0.f;
	float m_height = 0.f;

	float m_radius = 0.f;
	
	//Do you draw the bodies?
	static bool m_drawBodies;

	//The type of this physics body
	unsigned int m_bodyID = 0;
	//The types of bodies this body collides with
	unsigned int m_collideID = 0;

	//Is the object non-moving
	bool m_dynamic = false;

	//Physics body drawing stuff
	GLuint m_vao = GL_NONE;
	GLuint m_vboPos = GL_NONE;
};

//Sends body TO json file
inline void to_json(nlohmann::json& j, const PhysicsBody& phys)
{
	//Position
	j["BodyPosition"] = { phys.GetPosition().x, phys.GetPosition().y };
	//Stores body type
	j["BodyType"] = phys.GetBodyType();
	//Center offset
	j["CenterOffset"] = { phys.GetCenterOffset().x, phys.GetCenterOffset().y };
	//Stores corners
	j["BottomLeft"] = { phys.GetBottomLeft().x, phys.GetBottomLeft().y };
	j["BottomRight"] = { phys.GetBottomRight().x, phys.GetBottomRight().y };
	j["TopLeft"] = { phys.GetTopLeft().x, phys.GetTopLeft().y };
	j["TopRight"] = { phys.GetTopRight().x, phys.GetTopRight().y };
	//height and width
	j["Width"] = phys.GetWidth();
	j["Height"] = phys.GetHeight();
	//Radius
	j["Radius"] = phys.GetRadius();

	//Max velocity
	j["MaxVelo"] = phys.GetMaxVelo();

	//Friction strength
	j["Friction"] = phys.GetFriction();

	//Gravity enabled?
	j["Gravity"] = phys.GetGravity();

	//Collision type (body id) and what this body collides *with*
	j["BodyID"] = phys.GetBodyID();
	j["CollideID"] = phys.GetCollideID();

	//Sets whether the object is moving
	j["IsDynamic"] = phys.GetDynamic();
}

//Reads body in FROM json file
inline void from_json(const nlohmann::json& j, PhysicsBody& phys)
{
	//Set position
	phys.SetPosition(b2Vec2(j["BodyPosition"][0], j["BodyPosition"][1]));
	//Sets body type
	phys.SetBodyType(j["BodyType"]);
	//Set the center offset
	phys.SetCenterOffset(vec2(j["CenterOffset"][0], j["CenterOffset"][1]));
	//Sets corners
	phys.SetBottomLeft(vec2(j["BottomLeft"][0], j["BottomLeft"][1]));
	phys.SetBottomRight(vec2(j["BottomRight"][0], j["BottomRight"][1]));
	phys.SetTopLeft(vec2(j["TopLeft"][0], j["TopLeft"][1]));
	phys.SetTopRight(vec2(j["TopRight"][0], j["TopRight"][1]));
	//Set width and height
	phys.SetWidth(j["Width"]);
	phys.SetHeight(j["Height"]);
	//Set radius
	phys.SetRadius(j["Radius"]);

	//Sets Max velocity
	phys.SetMaxVelo(j["MaxVelo"]);

	//Sets Friction strength
	phys.SetFriction(j["Friction"]);

	//Set does gravity affect this body?
	phys.SetGravity(j["Gravity"]);

	//Set collision type (body id) and what this body collides *with*
	phys.SetBodyID(j["BodyID"]);
	phys.SetCollideID(j["CollideID"]);

	//Set whether or not the object is moving
	phys.SetDynamic(j["IsDynamic"]);
}
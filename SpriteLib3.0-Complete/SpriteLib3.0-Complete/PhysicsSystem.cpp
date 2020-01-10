#include "PhysicsSystem.h"

void PhysicsSystem::Init()
{
	physicsDrawShader.Load("./assets/shader/PhysicsDraw.vert", "./assets/shader/PhysicsDraw.frag");
}

void PhysicsSystem::Update(entt::registry * reg, b2World & world)
{
	auto view = reg->view<PhysicsBody, Transform>();

	//Runs the update for the physics bodies
	for (auto entity : view)
	{
		//Grabs references to each component within view
		auto& physBod = view.get<PhysicsBody>(entity);
		auto& trans = view.get<Transform>(entity);

		//Updates physics body
		physBod.Update(&trans);
	}

	//Runs the various things
	//(currently just checking collisions)
	Run(world);
}

void PhysicsSystem::Draw(entt::registry * reg)
{
	auto view = reg->view<PhysicsBody, Transform>();
	auto& cam = reg->get<Camera>(EntityIdentifier::MainCamera());

	if (PhysicsBody::GetDraw())
	{
		for (auto entity : view)
		{
			auto& physBody = view.get<PhysicsBody>(entity);
			auto& trans = view.get<Transform>(entity);

			//Temporary transform so we can actually draw the bodies
			Transform temp = trans;
			temp.SetScale(vec3(physBody.GetWidth(), physBody.GetHeight(), 1.f));
			//Sets the position so the center offset is still relevant
			temp.SetPosition(temp.GetPosition() + vec3(physBody.GetCenterOffset().x, physBody.GetCenterOffset().y, 0.f));
			//Puts the temporary transform for the physics body at the top z-layer
			temp.SetPositionZ(100.f);

			//Updates the transform to create model matrix
			temp.Update();

			std::string fileName = "Masks/";

			if (physBody.GetBodyType() == BodyType::BOX)
			{
				fileName += "SquareMask.png";
			}
			else if (physBody.GetBodyType() == BodyType::CIRCLE)
			{
				fileName += "CircleMask.png";
			}

			Texture* mask = TextureManager::FindTexture(fileName);

			//Binds the draw shader
			physicsDrawShader.Bind();

			//Sends the uniforms we need for drawing the bodies
			physicsDrawShader.SendUniform("uView", cam.GetView());
			physicsDrawShader.SendUniform("uProj", cam.GetProjection());
			physicsDrawShader.SendUniform("uModel", temp.GetLocalToWorldMatrix());
			physicsDrawShader.SendUniform("uColor", vec4(1.f, 0.f, 0.f, 0.3f));

			mask->Bind(0);

			//Draws the body
			physBody.DrawBody();

			mask->Unbind(0);

			//Unbinds the shader
			physicsDrawShader.Unbind();
		}
	}
}

void PhysicsSystem::Run(b2World & world)
{
	//Timestep is constant, regardless of deltatime
	float32 timeStep = 1.f / 60.f;

	//Box2D uses an algorithm called an integrator,
	//this simulates the physics algorithms at discrete points in time
	//we need to give it iterations for the velocity and position, along with
	//our timestep
	int32 velocityIterations = 8;
	int32 positionIterations = 3;

	//steps through the world
	world.Step(timeStep, velocityIterations, positionIterations);
}

bool PhysicsSystem::BoxCircleCollision(std::pair<PhysicsBody&, Circle> group1, std::pair<PhysicsBody&, Box> group2)
{
	//Doesn't work.
	////Circle position
	//vec2 circlePos = vec2(group1.second.m_center.x, group1.second.m_center.y);
	////Circle Radius
	//float circleRad = group1.second.m_radius;
	////Box position
	//vec2 boxPos = vec2(group2.second.m_center.x, group2.second.m_center.y);
	////Box width and height
	//float boxWidth = group2.first.GetWidth();
	//float boxHeight = group2.first.GetHeight();
	////Edges//
	////Left edge
	//float left = boxPos.x - ((1.f / 2.f) * boxWidth);
	////Right edge
	//float right = boxPos.x + ((1.f / 2.f) * boxWidth);
	////Top edge
	//float top = boxPos.y + ((1.f / 2.f) * boxHeight);
	////Bottom edge
	//float bottom = boxPos.y - ((1.f / 2.f) * boxHeight);

	//vec2 edgeBot 


	////Makes line from circle center to the test edge
	////Then gets the length of the line.
	//vec2 circleToEdge = circlePos - testVec;
	//float distance = circleToEdge.GetMagnitude();

	////If the distance is less than or equal to the radius, there's a collision.
	//if (distance <= circleRad)
	//{
	//	return true;
	//}

	////If the previous statement isn't true, there is no collision.
	//return false;
	return false;
}

bool PhysicsSystem::BoxBoxCollision(std::pair<PhysicsBody&, Box> group1, std::pair<PhysicsBody&, Box> group2)
{
	//if body 1 actually collides with body 2
		//Perform AABB bounding box checks
	//else
		//There's no collision
	if (group1.first.GetCollideID() & group2.first.GetBodyID())
	{
		//Perform bounding box checks
		//Are the x-axes colliding?
		bool axisXCollide = group1.second.m_bottomRight.x >= group2.second.m_bottomLeft.x &&
			group2.second.m_bottomRight.x >= group1.second.m_bottomLeft.x;

		//Are the y-axes colliding?
		bool axisYCollide = group1.second.m_topLeft.y >= group2.second.m_bottomLeft.y &&
			group2.second.m_topLeft.y >= group1.second.m_bottomLeft.y;

		//If both axes are overlapping, it means the bodies are colliding
		//If not, then they're not colliding
		return axisXCollide && axisYCollide;
	}
	else
	{
		return false;
	}
}

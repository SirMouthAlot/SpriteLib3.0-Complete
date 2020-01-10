#include "RenderingSystem.h"

void RenderingSystem::Init()
{
	//Initializes the vertex plane
	VertexManager::InitPlaneData();
	//Loads the drawShader
	drawShader.Load("./assets/shader/StaticGeometry.vert", "./assets/shader/PassThrough.frag");
}

void RenderingSystem::Update(entt::registry* reg)
{
	//Clear the buffers
	EffectManager::ClearBuffers();

	{
		//Creates a view of all entities consisting of both
		//*Sprite AND Transform
		auto view = reg->view<Sprite, Transform>();

		//Loops through all the entities within view
		for (auto entity : view)
		{
			//Grabs a reference to the Sprite component (in x entity)
			Sprite& spr = view.get<Sprite>(entity);
			//Updates the height and width of the unit plane, to match the width and height of the Sprite
			view.get<Transform>(entity).SetScale(vec3(float(spr.GetWidth()), float(spr.GetHeight()), 1.f));
			//Updates the transform of x entity
			view.get<Transform>(entity).Update();
		}
	}

	{
		//Creates a view consisting of all entityies containing horizontal scroll
		auto view = reg->view<HorizontalScroll>();

		//Loops through all the entities within view2_1
		for (auto entity : view)
		{
			//Grabs a reference to the scroll component
			auto& scroll = view.get(entity);

			//Updates the scroll
			scroll.Update();
		}
	}

	{
		//Creates a view of all entities consisting of vertical scroill
		auto view = reg->view<VerticalScroll>();

		//Loops through all the entities within view2
		for (auto entity : view)
		{
			//Grabs a reference to the Camera component (in x entity)
			auto& scroll = view.get(entity);

			//Updates the camera
			scroll.Update();
		}
	}

	{
		//Creates a view of all entities consisting of Camera
		auto view = reg->view<Camera>();

		//Loops through all the entities within view2
		for (auto entity : view)
		{
			//Grabs a reference to the Camera component (in x entity)
			auto& cam = view.get(entity);

			//Updates the camera
			cam.Update();

			//Binds the shader
			drawShader.Bind();
			//Sends the view matrix to the shader
			drawShader.SendUniform("uView", cam.GetView());
			//Sends the projection matrix to the shader
			drawShader.SendUniform("uProj", cam.GetProjection());
			//Unbinds the shader
			drawShader.Unbind();
		}
	}
}

struct
{
	bool operator()(std::pair<Transform*, Sprite> a, std::pair<Transform*, Sprite> b) const
	{
		return ((a.first->GetPosition().z) < (b.first->GetPosition().z));
	}
} customSort;


void RenderingSystem::Draw(entt::registry* reg)
{
	//Creates a view of all entities consisting of both
	//*Sprite AND Transform
	auto view = reg->view<Sprite, Transform>();

	//are there any other buffers within the manager? (meaning other buffers exist
	//and will need a render of the scene on another framebuffer)
	//*if so, we make sure everything draws to the basic buffer

	//Binds the draw shader
	drawShader.Bind();

	if (EffectManager::GetEffectInit())
		EffectManager::BindBasicEffect();

	std::vector<std::pair<Transform*, Sprite>> sortedSprites;

	//Loops through all the entities within view
	for (auto entity : view)
	{
		//Grabs a reference to the Transform component (in x entity)
		auto &trans = view.get<Transform>(entity);
		//Grabs a reference to the Sprite component (in x entity)
		auto &spr = view.get<Sprite>(entity);

		sortedSprites.push_back(std::pair<Transform*, Sprite>(&trans, spr));
		//spr.Draw();
	}

	std::sort(sortedSprites.begin(), sortedSprites.end(), customSort);

	for (auto pair : sortedSprites)
	{
		drawShader.SendUniform("uModel", pair.first->GetLocalToWorldMatrix());

		pair.second.Draw();
	}


	//Unbinds the basic buffer
	EffectManager::UnbindBasicEffect();
	//Unbinds the draw shader
	drawShader.Unbind();
	
	//If the binding happened for the basic buffer, we make sure to unbind it and then draw all the buffers
	if (EffectManager::GetEffectInit())
	{
		//Draws all buffers
		EffectManager::DrawEffects();
	}
}

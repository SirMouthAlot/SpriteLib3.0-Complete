#include "AnimationSystem.h"

void AnimationSystem::Update(float dt, entt::registry* reg)
{
	//Creates a view consisting of all entities that have both 
	//*Animated Sprite AND Transform
	auto view = reg->view<Sprite, AnimationController, Transform>();

	//Loops through all entities within view
	for (auto entity : view)
	{
		//Grabs and stores a reference to the sprite within view (at x entity)
		auto &spr = view.get<Sprite>(entity);
		//Grabs and stores a reference to the Transform within view (at x entity)
		auto &trans = view.get<Transform>(entity);
		//Grabs and stores a refernce to the Animation Controller
		auto &anim = view.get<AnimationController>(entity);

		//TODO: Change the size of the sprite to accomodate bigger animations

		//Sets transform scale based on the width and height of the sprite
		trans.SetScale(vec3(float(spr.GetWidth()), float(spr.GetHeight()), 1.f));
		//Updates transform (creates model matrix)
		trans.Update();

		//Updates AnimatedSprite
		anim.Update(dt);
	}
}
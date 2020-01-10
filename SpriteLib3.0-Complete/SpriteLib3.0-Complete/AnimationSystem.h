#ifndef __ANIMATIONSYSTEM_H__
#define __ANIMATIONSYSTEM_H__

#include <entt/entity/registry.hpp>
#include "Transform.h"
#include "Timer.h"
#include "Input.h"
#include "Sprite.h"
#include "AnimationController.h"

//A system utilizing ECS, the namespace just holds all the different functions
namespace AnimationSystem
{
	//States for the knight's animations
	enum KnightStates
	{
		WalkLeft,
		WalkRight,
		WalkUp,
		WalkDown,
		LeftLook,
		RightLook,
		UpLook,
		DownLook
	};

	//A few quick bools for knight controller
	static bool animationSet = false;
	static bool lookStarted = false;
	static bool moveUp = true;
	static bool moveDown = false;

	//Updates the Animated Sprite data using the registry
	//*Things like updating the transform scale
	//*Updating the transform
	//*Triggering the animated sprite update
	void Update(float dt, entt::registry* reg);
}

#endif // !__ANIMATIONSYSTEM_H__

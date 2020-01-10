#ifndef __CAMERASYSTEM_H__
#define __CAMERASYSTEM_H__

#include <entt/entity/registry.hpp>
#include "Camera.h"

//A system utilizing ECS, the namespace just holds the functions
//*System is Depreciated
namespace CameraSystem
{
	//Performs an update
	//*Updates all entities with Camera component in registry
	//*Function is Depreciated
	void Update(entt::registry& reg);
}

#endif // !__CAMERASYSTEM_H__


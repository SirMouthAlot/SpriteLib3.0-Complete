#include "CameraSystem.h"

void CameraSystem::Update(entt::registry& reg)
{
	//Creates a view with all entities that have a component of type Camera
	auto view = reg.view<Camera>();

	//Loops through all the entities within view
	for (auto entity : view)
	{
		//Grabs and stores a reference to the Camera component (at x entity)
		auto &camera = view.get(entity);

		//Performs an update on the camera
		camera.Update();
	}
}

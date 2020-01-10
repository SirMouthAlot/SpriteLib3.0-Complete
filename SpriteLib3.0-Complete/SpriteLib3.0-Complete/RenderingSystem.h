#ifndef __RENDERINGSYSTEM_H__
#define __RENDERINGSYSTEM_H__

#include <entt/entity/registry.hpp>

#include "Shader.h"
#include "VertexManager.h"
#include "EffectManager.h"
#include "VerticalScroll.h"

//A system utilizing ECS, the namespace just holds all the different functions
namespace RenderingSystem
{
	//The shader used for rendering every sprite
	static Shader drawShader;

	//Runs all the things required within each specific class, in order to allow for rendering
	void Init();

	//Updates everything drawable within the registry
	void Update(entt::registry* reg);

	//Draws everything drawable within the registry
	void Draw(entt::registry* reg);

}

#endif // !__RENDERINGSYSTEM_H__


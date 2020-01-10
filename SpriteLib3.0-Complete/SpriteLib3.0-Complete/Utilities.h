#pragma once

#include "Vector.h"
#include "Camera.h"
#include "BackEnd.h"
#include "EntityIdentifier.h"

#include <entt/entity/registry.hpp>

namespace Util
{
	//Convert from Windowspace to GLspace
	vec2 ConvertToGL(entt::registry* reg, vec2 clickCoord);


	//Templated functions
	template<typename T>
	T Lerp(T v0, T v1, float t);


	template<typename T>
	T Lerp(T v0, T v1, float t)
	{
		return (v0 * (1-t)) + (v1 * (t));
	}
}
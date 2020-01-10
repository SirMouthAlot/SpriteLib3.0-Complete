#ifndef __SCENEEDITOR_H__
#define __SCENEEDITOR_H__

#include <entt/entity/registry.hpp>

#include "Scene.h"
#include "UI.h"
#include "Camera.h"
#include "Transform.h"
#include "EntityIdentifier.h"
#include "Selectable.h"

namespace SceneEditor
{
	static std::string m_currentEntityName = "None Selected";
	static unsigned int m_currentEntityNum = 0;
	static bool m_selected = false;
	static Selectable m_selectable("Scene Editor");

	void ResetEditor();
	void CreateTab(entt::registry* reg, Scene* scene);
	void CreateEditor(entt::registry* reg, Scene* scene);
}

#endif // !__SCENEEDITOR_H__

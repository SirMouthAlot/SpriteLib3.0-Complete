#include "ECS.h"

entt::registry* ECS::m_reg = nullptr;

void ECS::AttachRegister(entt::registry * reg)
{
	//Attach reference to the registry
	m_reg = reg;
}

void ECS::UnattachRegister()
{
	//Unattaches the register
	m_reg = nullptr;
}

unsigned ECS::CreateEntity()
{
	//create the entity
	auto entity = m_reg->create();

	//Attach Entity Identifier to the entity
	m_reg->assign<EntityIdentifier>(entity);

	//Return entity ID
	return entity;
}

void ECS::DestroyEntity(unsigned entity)
{
	//Destroys the entity
	m_reg->destroy(entity);
}

void ECS::SetUpIdentifier(unsigned entity, unsigned componentID, std::string name)
{
	//Gets reference to the component
	auto& id = GetComponent<EntityIdentifier>(entity);
	//Set the entity
	id.SetEntity(entity);
	//Set the entity identifier unsigned int
	id.AddComponent(componentID);
	//Set the name of the entity
	id.SetName(name);
}

void ECS::SetIsMainCamera(unsigned entity, bool mainCamera)
{
	//Gets reference to the component
	auto& id = GetComponent<EntityIdentifier>(entity);
	//Sets whether this entity is the main camera
	id.SetIsMainCamera(mainCamera);

	EntityIdentifier::MainCamera(entity);
}

void ECS::SetIsMainPlayer(unsigned entity, bool mainPlayer)
{
	//Gets reference to the component
	auto& id = GetComponent<EntityIdentifier>(entity);
	//Sets whether this entity is the main player
	id.SetIsMainPlayer(mainPlayer);

	EntityIdentifier::MainPlayer(entity);
}

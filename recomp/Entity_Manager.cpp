#include "Entity_Manager.h"



void EntityManager::Update()
{
	for (Entity* ent : addNextFrame) {
		generalContainer.push_back(ent);
		typeBasedContainer[ent->GetType()].push_back(ent);
	}

	addNextFrame.clear();
	RemoveInactive();
}
void EntityManager::RemoveInactive()
{
	generalContainer.erase(std::remove_if(generalContainer.begin(), generalContainer.end(), [](Entity* ent) {
		return !ent->IsActive() || ent == nullptr;
		}), generalContainer.end());


	for (auto& [type, vector] : typeBasedContainer) {
		vector.erase(std::remove_if(vector.begin(), vector.end(), [](Entity* ent) {
			return !ent->IsActive() || ent == nullptr;
			}), vector.end());
	}
}
void EntityManager::ResteatType(int entityID, EntityType currentType, const EntityType changeTo) {

	for (auto& [type, vector] : typeBasedContainer) {
		if (type != currentType) {
			continue;
		}

		auto it = std::remove_if(vector.begin(), vector.end(), [&](Entity* ent) {
			if (ent->GetID() == entityID) {
				ent->type = changeTo;
				typeBasedContainer[changeTo].push_back(ent);
				return true;
			}
			return false;
			});

		vector.erase(it, vector.end());
	}

}

const std::vector<Entity*>& EntityManager::GetEntities()const {
	return generalContainer;
}
const std::vector<Entity*>& EntityManager::GetEntities(const EntityType type) {
	return typeBasedContainer[type];
}
const std::map<EntityType, std::vector<Entity*>>& EntityManager::GetEntitiesMap()const {
	return typeBasedContainer;
}



void EntityManager::AddEntity(const nlohmann::json& entityConfig, EntityType type, const TextureManager& txtm) {

	SDL_FRect bb;
	bb.x = entityConfig["posx"];
	bb.y = entityConfig["posy"];
	bb.w = entityConfig["width"];
	bb.h = entityConfig["height"];

	SDL_Texture* txt = txtm.GetTexture(entityConfig["texture"].get<std::string>());

	Entity* newEntry = new Entity(totalEntities++, type, bb, txt);
	
	newEntry->Enable();//if not manually called, entity is marked as dead and it be immediately cleaned up, shittest kind of

	addNextFrame.push_back(newEntry);
}

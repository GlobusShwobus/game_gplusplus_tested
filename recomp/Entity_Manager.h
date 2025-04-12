#pragma once

#include "Entity.h"
#include "Texture_Manager.h"

class EntityManager {

	std::vector<Entity*> addNextFrame;
	std::vector<Entity*> generalContainer;
	std::map<EntityType, std::vector<Entity*>> typeBasedContainer;

	size_t totalEntities = 0;
	void RemoveInactive();

public:

	EntityManager() {}

	void Update();
	void ResteatType(int entityID, EntityType currentType, const EntityType changeTo);


	void AddEntity(const nlohmann::json& entityConfig, EntityType type, const TextureManager& txtm);

	const std::vector<Entity*>& GetEntities()const;
	const std::vector<Entity*>& GetEntities(const EntityType type);
	const std::map<EntityType, std::vector<Entity*>>& GetEntitiesMap()const;

	/*
	bool PosIsFree(vec2& pos)const {

		while (true) {
			bool isFree = true;

			for (const auto& each : all) {
				const sf::FloatRect& bounds = each->shape.getGlobalBounds();

				if (bounds.contains(pos.x, pos.y)) {

					isFree = false;

					pos.x = bounds.left + bounds.width;


					if (pos.x >= window_size.x) {
						pos.x = 0;
						pos.y += bounds.height;
					}

					if (pos.y >= window_size.y) {
						return false;
					}
					break;
				}
			}

			if (isFree) {
				return true;
			}
		}
	}
	*/
};
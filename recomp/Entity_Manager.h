#pragma once

#include "Entity.h"
#include "Texture_Manager.h"

class EntityManager {

	std::vector<std::shared_ptr<Entity>> add_next_frame;
	std::vector<std::shared_ptr<Entity>> all;
	std::map<EntityType, std::vector<std::shared_ptr<Entity>>> per_type;

	size_t total_entities = 0;
	void RemoveInactive();

public:

	EntityManager() {}

	void Update();
	void ResteatType(int entity_id, EntityType current_type, const EntityType change_to);


	void AddEntity(const nlohmann::json& entity_config, EntityType type, const TextureManager& txtm);

	const std::vector<std::shared_ptr<Entity>>& GetEntities()const;
	const std::vector<std::shared_ptr<Entity>>& GetEntities(const EntityType t);
	const std::map<EntityType, std::vector<std::shared_ptr<Entity>>>& GetEntitiesMap()const;

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
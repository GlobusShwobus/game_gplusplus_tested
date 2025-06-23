#pragma once

#include "SDL3/SDL.h"
#include "SDL3_image/SDL_image.h"
#include "json.hpp"

#include "Sprite.h"
#include "HashKeys.h"
#include "CCPRect.h"
#include "State.h"


/*
* Data initalized by the factory should NEVER be cleaned up during runtime as entities will depend on the basic components
*/
namespace badEngine {

	class Player;
	class Enemy;

	class EntityFactory {

		std::map<HKey::ENTITY_TYPE, SDL_Texture*>             textures;     //requires cleanup in destructor
		std::map<HKey::ENTITY_TYPE, TSA::TTransfer>           ttransfers;
		std::map<HKey::ENTITY_TYPE, std::vector<TSA::Reel>>   animations;
		std::map<HKey::ENTITY_TYPE, CCP::HitBox>              hitboxes;

		bool isInit = false;

		bool componentValidationPhase(const nlohmann::json* const entityConfig, SDL_Renderer* renderer);
		void componentConstructionPhase(const nlohmann::json& data, SDL_Renderer* renderer, const HKey::ENTITY_TYPE key, const std::string& keyStr);


		bool	  constructComponent_Texture(const nlohmann::json& data, SDL_Renderer* renderer, const HKey::ENTITY_TYPE key, const std::string& keyStr);
		void	  constructComponent_TTransfer(const nlohmann::json& data, const HKey::ENTITY_TYPE key);
		SDL_FRect constructComponent_TTransfer(const nlohmann::json& data, const char* field);
		void	  constructComponent_Animations(const nlohmann::json& data, const HKey::ENTITY_TYPE key, const std::string& keyStr);
		void	  constructComponent_Hitbox(const nlohmann::json& data, const HKey::ENTITY_TYPE key, const std::string& keyStr);

	public:

		EntityFactory(const nlohmann::json* const entityConfig, SDL_Renderer* renderer);
		bool isInitalized()const;
		void wipeMemory();
		Player* createPlayer(HKey::ENTITY_TYPE key)const;			//caller is the owner, can return nullptr
		Enemy* createEnemy(HKey::ENTITY_TYPE key)const;			//caller is the owner, can return nullptr

		SDL_Texture* getTexture(HKey::ENTITY_TYPE key)const;   //returns a pointer to, reciever is not the owner
		TSA::TTransfer                getTTransfer(HKey::ENTITY_TYPE key)const;
		const std::vector<TSA::Reel>* getAnimation(HKey::ENTITY_TYPE key)const;	//returns a pointer to, reciever is not the owner
		CCP::HitBox                   getHitbox(HKey::ENTITY_TYPE key)const;

		~EntityFactory();
	public:
		EntityFactory(const EntityFactory&) = delete;
		EntityFactory(EntityFactory&&)noexcept = delete;
		EntityFactory& operator=(const EntityFactory&) = delete;
		EntityFactory& operator=(EntityFactory&&)noexcept = delete;
	};

	class Player {

	public:

		TSA::Sprite sprite;
		CCP::HitBox hitbox;
		SMS::State state;

		Player(const TSA::Sprite& sprite, const CCP::HitBox& hitbox);

		bool WASD_PlayerVelocity(const float moveSpeed);
	};

	class Enemy {
	public:

		TSA::Sprite sprite;
		CCP::HitBox hitbox;

		Enemy(const TSA::Sprite& sprite, const CCP::HitBox& hitbox);
	};

	void setCoordinates(CCP::HitBox& hitbox);
	SMS::Facing facingDirection(const SDL_FPoint& velocity);
	bool isMoving(const SDL_FPoint& velocity);
	TSA::AnimationID animationIDTable(const SMS::State& state);
}
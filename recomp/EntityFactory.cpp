#include "EntityFactory.h"
namespace badEngine {

	namespace EFM {
		EntityFactory::EntityFactory(const nlohmann::json* const entityConfig, SDL_Renderer* renderer)
		{
			isInit = componentValidationPhase(entityConfig, renderer);
		}
		EntityFactory::~EntityFactory()
		{
			for (auto& [id, texture] : textures) {
				SDL_DestroyTexture(texture);
			}
		}
		bool EntityFactory::componentValidationPhase(const nlohmann::json* const entityConfig, SDL_Renderer* renderer)
		{
			if (!entityConfig) {//if null ptr, kill it
				printf("\nentityConfig nullptr, cannot initalize components\n");
				return false;
			}

			for (const auto& entry : *entityConfig) {

				if (!entry.contains("type")) {
					printf("\nentity missing type specification");
					continue;
				}
				std::string keyStr = entry["type"].get<std::string>();
				HKey::ENTITY_TYPE key = HKey::buildEntityTypeID(keyStr.c_str());

				if (!HKey::isValidID(key)) {
					printf("\ninvalid entity type unknown");
					continue;
				}

				componentConstructionPhase(entry, renderer, key, keyStr);
			}
			return true;
		}
		void EntityFactory::componentConstructionPhase(const nlohmann::json& data, SDL_Renderer* renderer, const HKey::ENTITY_TYPE key, const std::string& keyStr)
		{

			if (data.contains("texture_path")) {
				if (constructComponent_Texture(data, renderer, key, keyStr)) {
					constructComponent_TTransfer(data, key);
				}
			}
			else {
				printf("\nwarning: entity construction without texture: <%s>", keyStr.c_str());
			}

			if (data.contains("animations")) {
				constructComponent_Animations(data["animations"], key, keyStr);
			}
			else {
				printf("\nwarning: entity construction without animation: <%s>", keyStr.c_str());
			}

			if (data.contains("Hitbox")) {
				constructComponent_Hitbox(data["Hitbox"], key, keyStr);
			}
			else {
				printf("\nwarning: entity construction without hitbox: <%s>", keyStr.c_str());
			}

			/*
			OTHER FIELDS HERE
			*/
		}
		
		bool EntityFactory::constructComponent_Texture(const nlohmann::json& data, SDL_Renderer* renderer, const HKey::ENTITY_TYPE key, const std::string& keyStr)
		{
			std::string path = data["texture_path"].get<std::string>();
			SDL_Texture* texture = IMG_LoadTexture(renderer, path.c_str());

			if (!texture) {
				printf("\nerror: entity texture missing physical file: <type: %s>\t<path: %s>", path, keyStr);
				return false;
			}

			textures.emplace(key, texture);
			return true;
		}
		void EntityFactory::constructComponent_TTransfer(const nlohmann::json& data, const HKey::ENTITY_TYPE key)
		{
			TSA::TTransfer ss;

			if (data.contains("texture_src"))
				ss.src = constructComponent_TTransfer(data, "texture_src");
			else
				printf("\nwarning: missing field rect: <texture_src>");


			if (data.contains("texture_dest"))
				ss.dest = constructComponent_TTransfer(data, "texture_dest");
			else
				printf("\nwarning: missing field rect: <texture_dest>");


			ttransfers.emplace(key, ss);
		}
		SDL_FRect EntityFactory::constructComponent_TTransfer(const nlohmann::json& data, const char* field)
		{
			SDL_FRect rect = { 0,0,0,0 };

			try {
				const auto& arect = data[field];
				rect = { arect[0],arect[1] ,arect[2] ,arect[3] };
			}
			catch (std::exception& expt) {
				printf("\nerror: rect creation: <%s>", expt.what());
				rect = { 0,0,0,0 };
			}

			return rect;
		}
		void EntityFactory::constructComponent_Animations(const nlohmann::json& data, const HKey::ENTITY_TYPE key, const std::string& keyStr)
		{
			std::vector<TSA::Reel> animationCollections;

			for (const auto& rData : data) {

				if (!rData.contains("id") || !rData.contains("reel_xyw") && rData["reel_xyw"].is_object()) {
					printf("\nerror: animation reel missing id or invalid reel setup: <%s>", keyStr.c_str());
					continue;
				}

				TSA::Reel clip;

				std::string aids = rData["id"].get<std::string>();//animation id string
				clip.id = HKey::buildAnimationID(aids.c_str());
				clip.isLooping = rData.value("loops", false);

				const int frameCount = rData.value("frameCount", 1);
				printf("\n%d", frameCount);

				try {
					const int x = rData["reel_xyw"][0];
					const int y = rData["reel_xyw"][1];
					const int w = rData["reel_xyw"][2];

					for (int i = 0; i < frameCount; i++) {
						SDL_FPoint frameCordinate;
						frameCordinate.x = x + (i * w);
						frameCordinate.y = y;

						clip.frames.push_back(std::move(frameCordinate));
					}
				}
				catch (std::exception& expt) {
					printf("\nerror: aniamtion clip invalid index: <%s>\t<%s>", keyStr.c_str(), aids.c_str());
					continue;
				}

				animationCollections.push_back(std::move(clip));
			}
			animations.emplace(key, std::move(animationCollections));
		}
		void EntityFactory::constructComponent_Hitbox(const nlohmann::json& data, const HKey::ENTITY_TYPE key, const std::string& keyStr) {
			CCP::HitBox hitbox;


			hitbox.rectangle.x = data.value("x", 0);
			hitbox.rectangle.y = data.value("y", 0);
			hitbox.rectangle.w = data.value("w", 0);
			hitbox.rectangle.h = data.value("h", 0);


			if (hitbox.rectangle.w == 0 || hitbox.rectangle.h == 0) {
				printf("\nwarning: hitbox invalid initalization. default 0: <%s>", keyStr);
			}

			hitboxes.emplace(key, std::move(hitbox));
		}


		SDL_Texture* EntityFactory::getTexture(HKey::ENTITY_TYPE key)const
		{
			return textures.find(key) != textures.end() ? textures.at(key) : nullptr;
		}
		TSA::TTransfer EntityFactory::getTTransfer(HKey::ENTITY_TYPE key)const
		{
			TSA::TTransfer ttransfer;

			if (ttransfers.find(key) != ttransfers.end()) {
				ttransfer.src = ttransfers.at(key).src;
				ttransfer.dest = ttransfers.at(key).dest;
			}

			return ttransfer;
		}
		const std::vector<TSA::Reel>* EntityFactory::getAnimation(HKey::ENTITY_TYPE key)const
		{
			const std::vector<TSA::Reel>* animations = nullptr;

			if (this->animations.find(key) != this->animations.end()) {
				animations = &this->animations.at(key);
			}

			return animations;
		}
		CCP::HitBox EntityFactory::getHitbox(HKey::ENTITY_TYPE key)const
		{
			CCP::HitBox hitbox;

			if (hitboxes.find(key) != hitboxes.end()) {
				hitbox = hitboxes.at(key);
			}

			return hitbox;
		}


		//caller is the owner, can return nullptr
		EDM::Player* EntityFactory::createPlayer(HKey::ENTITY_TYPE key)const
		{

			SDL_Texture* texture = getTexture(key);
			TSA::TTransfer ttransfer = getTTransfer(key);
			const std::vector<TSA::Reel>* animations = getAnimation(key);
			CCP::HitBox hitbox = getHitbox(key);

			EDM::Player* player = new EDM::Player({ texture, ttransfer }, hitbox);

			bool isValidTexture = player->sprite.texture;

			if (!isValidTexture) {
				printf("\nwarning: attempt to init sprite texture failure: PLAYER");
			}

			if (isValidTexture && !player->sprite.initPlay(animations)) {
				printf("\nwarning: attempt to init animations failure: PLAYER");
			}

			return player;
		}
		//caller is the owner, can return nullptr
		EDM::Enemy* EntityFactory::createEnemy(HKey::ENTITY_TYPE key)const
		{
			SDL_Texture* texture = getTexture(key);
			TSA::TTransfer ttransfer = getTTransfer(key);
			CCP::HitBox hitbox = getHitbox(key);

			EDM::Enemy* enemy = new EDM::Enemy({ texture, ttransfer }, hitbox);

			bool isValidTexture = enemy->sprite.texture;

			if (!isValidTexture) {
				printf("\nwarning: attempt to init sprite texture failure: ENEMY");
			}
			return enemy;
		}



		bool EntityFactory::isInitalized()const
		{
			return isInit;
		}
		void EntityFactory::wipeMemory() {
			for (auto& [id, texture] : textures)
			{
				SDL_DestroyTexture(texture);
			}
			textures.clear();
			ttransfers.clear();
			animations.clear();
			hitboxes.clear();
		}
	}
}
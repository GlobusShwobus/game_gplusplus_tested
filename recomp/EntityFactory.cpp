#include "EntityFactory.h"
namespace badEngine {


	Player* EntityFactory::createPlayer(const PlayerID id)const {
		Player* player = nullptr;

		if (textureComponents.contains(id) && animationComponents.contains(id) && playerData.contains(id)) {
			player = new Player(textureComponents.at(id), animationComponents.at(id), playerData.at(id));
		}
		else {
			printf("something wrong in player data initialization OR invalid ID\n");
		}
		return player;
	}
	EnemyBasic* EntityFactory::createEnemy(const EnemyID id)const {
		EnemyBasic* enemybasic = nullptr;

		if (textureComponents.contains(id) && enemyData.contains(id)) {
			enemybasic = new EnemyBasic(textureComponents.at(id), enemyData.at(id));
		}
		else {
			printf("something wrong in enemyBasic data initialization OR invalid ID\n");
		}
		return enemybasic;
	}

	EntityFactory::~EntityFactory()
	{
		for (auto& [id, texture] : textureComponents) {
			SDL_DestroyTexture(texture);
		}
		textureComponents.clear();
		animationComponents.clear();
		enemyData.clear();
		playerData.clear();
	}
}
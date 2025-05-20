#pragma once

#include "BasicComponents.h"
#include "json.hpp"

class Player {

	Sprite sprite;
	NPCState state;
	std::vector<AnimationReel> reels;
	AnimationController animControlls;
	
	float movementSpeed = 0;
	float healthPoints = 0;
	float attackPower = 0;
public:
	
	Player(asd) {

	}


};
#pragma once

#include "Sprite.h"

enum MovementStatusFlags {
	MFLAG_KEY_W = 1 << 0,
	MFLAG_KEY_S = 1 << 1,
	MFLAG_KEY_A = 1 << 2,
	MFLAG_KEY_D = 1 << 3,

	MFLAG_FACING_UP = 1 << 4,
	MFLAG_FACING_DOWN = 1 << 5,
	MFLAG_FACING_LEFT = 1 << 6,
	MFLAG_FACING_RIGHT = 1 << 7,
	MFLAG_IS_MOVING = 1 << 8
};
typedef int MFLAG;

class Movement {

	int movementStatus = 0;

	//movement can be refined to work diagonally in an intended way, currently it is possible but it is a bug
public:

	void moveBegin(const SDL_Event* const event);
	void moveEnd();
	void setStatusFlags(const MFLAG flags);
	void removeStatusFlags(const MFLAG flags);
	int getMovementStatus()const;
};

class Camera {

	SDL_FPoint center{ 0,0 };
	int radiusWidth = 0;
	int radiusHeight = 0;

	//zooming requires another member variable float scalar, then call the setRenderScale in rendering logic, but not to get ahead too much
public:

	Camera(const int diameterWidth, const int diameterHeight) :radiusWidth(diameterWidth / 2), radiusHeight(diameterHeight / 2) {}

	void setFocus(const SDL_FRect* const playerPos);
	void setClamp(const SDL_FRect* const worldMap);
	const SDL_FPoint getCenter()const;
	SDL_FRect toCameraSpace(const SDL_FRect* const entity)const;
};


class Player {

public:

	Sprite sprite;
	Movement movement;
	Camera camera;

	static constexpr int speed = 5;

	//animation

	//frameLimiter
	//NPCs->handle new/delete inbetween frames

	Player(Sprite texture, SDL_FPoint cameraRadii) :sprite(texture), camera(cameraRadii.x, cameraRadii.y) {}
};
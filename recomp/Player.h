#pragma once

#include "SDL3/SDL.h"


enum MovementStatus {

	MOVE_UP,
	MOVE_DOWN,
	MOVE_LEFT,
	MOVE_RIGHT,

	MOVE_UP_LEFT,
	MOVE_UP_RIGHT,
	MOVE_DOWN_LEFT,
	MOVE_DOWN_RIGHT,

	FACING_UP,
	FACING_DOWN,
	FACING_LEFT,
	FACING_RIGHT,
	MOVEMENT_STATUS_NULL
};
//movement flags

class Movement {

	MovementStatus movementStatus = MOVEMENT_STATUS_NULL;
	MovementStatus facingDirection = MOVEMENT_STATUS_NULL;

	//movement can be refined to work diagonally in an intended way, currently it is possible but it is a bug
public:

	void moveBegin(const SDL_Event* const event);
	void moveEnd();


	MovementStatus getMovementStatus()const {
		return movementStatus;
	}
	MovementStatus getFacingDirection()const {
		return facingDirection;
	}

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

class Sprite {

	SDL_Texture* texture = nullptr;//not owner
	SDL_FRect source = { 0,0,0,0 };
	SDL_FRect destination = { 0,0,0,0 };


	bool isAnimated = false;
	int currentAnimationDescription = 0;
	int frameWidth = 0;
	int frameHeight = 0;

	void ifIdleAnimation() {

	}

public:

	Sprite(SDL_Texture* Texture) :texture(Texture) {
		//defaults
		source.w = texture->w;
		source.h = texture->h;

		destination.w = texture->w;
		destination.h = texture->h;
	}

	//if a texture is animated, but framewidth and height are not set, then it is not animated, duhh
	void initFrame(int frameW, int frameH) {
		frameWidth = frameW;
		frameHeight = frameH;
		isAnimated = true;//the only place it is set
	}

	SDL_Texture* getTexture() {
		return texture;
	}
	SDL_FRect* getSource() {
		return &source;
	}
	SDL_FRect* getDestination() {
		return &destination;
	}

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
#pragma once

#include "SDL3/SDL.h"
#include <map>

enum class MovementStatus {
	MOVE_UP,
	MOVE_DOWN,
	MOVE_LEFT,
	MOVE_RIGHT,

	MOVE_UP_LEFT,
	MOVE_UP_RIGHT,
	MOVE_DOWN_LEFT,
	MOVE_DOWN_RIGHT,
	MOVEMENT_STATUS_NOTHING
};
typedef MovementStatus MS;
//movement flags

class Movement {

	MovementStatus movementStatus  = MS::MOVEMENT_STATUS_NOTHING;
	MovementStatus lastMove        = MS::MOVEMENT_STATUS_NOTHING;

public:
	void movementUpdate();

	MovementStatus getCurrentMove()const;
	MovementStatus getLastMove()const;
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

enum class ClipID {
	none         = 0,
	walk_up      = 1,
	walk_down    = 2,
	walk_left    = 3,
	walk_right   = 4,
	idle_up      = 5,
	idle_down    = 6,
	idle_left    = 7,
	idle_right   = 8,
};

struct Animation {
	int x = 0;
	int y = 0;
	int w = 0;
	int h = 0;
	int frameCount = 0;
	int frameDelay = 0;
	bool isLooping = false;
};

class Sprite {

	SDL_Texture* texture = nullptr;//not owner
	SDL_FRect source = { 0,0,0,0 };
	SDL_FRect destination = { 0,0,0,0 };

	ClipID previousClip = ClipID::none;
	int frameIndex = 0;
	int clipTimer = 0;

public:

	Sprite(SDL_Texture* Texture, SDL_FRect* src, SDL_FRect* dest) :texture(Texture), source(*src), destination(*dest) {}
	Sprite() = default;//later refine texturemanager code (if it's not a meme) and get rid of this constructor

	void play(const ClipID clipID);

	SDL_Texture* getTexture();
	SDL_FRect* getSource();
	SDL_FRect* getDestination();
};

class Player {

public:

	Sprite sprite;
	Movement movement;
	Camera camera;

	static constexpr float speed = 2.5f;//uhm, speed*FPS is real speed so yeah. oops

	Player(Sprite texture, SDL_FPoint cameraRadii) :sprite(texture), camera(cameraRadii.x, cameraRadii.y) {}
};

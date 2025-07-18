#include "Sprite.h"

namespace badEngine {
	Sprite::Sprite(const std::shared_ptr<SDL_Texture>& texture) :texture(texture) {}
	SDL_Texture& Sprite::getTexture() {
		return *texture;
	}
}
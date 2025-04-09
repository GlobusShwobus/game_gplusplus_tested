#include "RenderWindow.h"

RenderWindow::RenderWindow(const char* title, int width, int height)
{

	window = SDL_CreateWindow(title, width, height, SDL_WINDOW_RESIZABLE | SDL_WINDOW_OPENGL);

	if (window) {
		renderer = SDL_CreateRenderer(window, NULL);
	}

}

void RenderWindow::Clear()
{
	SDL_RenderClear(renderer);
}
bool RenderWindow::Good()const {
	return window && renderer;
}

void RenderWindow::Display()
{
	SDL_RenderPresent(renderer);
}
void RenderWindow::Render(SDL_Texture* texture, const SDL_FRect* bb)
{
	SDL_RenderTexture(renderer, texture, nullptr, bb);
}
SDL_Texture* RenderWindow::UniqueTextureLoad(const char* path)
{
	return IMG_LoadTexture(renderer, path);
}
#include "RenderWindow.h"

RenderWindow::RenderWindow(const char* title, int width, int height)
{

	window = SDL_CreateWindow(title, width, height, SDL_WINDOW_RESIZABLE | SDL_WINDOW_OPENGL);

	if (window) {
		renderer = SDL_CreateRenderer(window, NULL);
	}

}

void RenderWindow::clear()
{
	SDL_RenderClear(renderer);
}
bool RenderWindow::good()const {
	return window && renderer;
}

void RenderWindow::display()
{
	SDL_RenderPresent(renderer);
}
void RenderWindow::render(SDL_Texture* texture, const SDL_FRect* bb)
{
	SDL_RenderTexture(renderer, texture, nullptr, bb);
}
SDL_Texture* RenderWindow::loadNewTexture(const char* path)
{
	return IMG_LoadTexture(renderer, path);
}
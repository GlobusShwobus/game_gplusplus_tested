#include "RenderWindow.h"

RenderWindow::RenderWindow(const nlohmann::json* const config) {

	if (!config || !config->contains("Window")) {
		printf("\nERROR:: missing 'Window' object from config\n");
		std::exit(EXIT_FAILURE);
	}

	const auto& windowParams = (*config)["Window"];

	if (!windowParams.contains("game_name") || !windowParams.contains("width") || !windowParams.contains("height")) {
		printf("\nERROR:: missing 'game_name' or 'width' or 'height' parameters from config\n");
		std::exit(EXIT_FAILURE);
	}

	const std::string title = windowParams["game_name"];
	const int width = windowParams["width"];
	const int height = windowParams["height"];

	window = SDL_CreateWindow(title.c_str(), width, height, SDL_WINDOW_RESIZABLE | SDL_WINDOW_OPENGL);

	if (!window) {
		printf("\nERROR:: failed to create SDL Window: %s\n", SDL_GetError());
		std::exit(EXIT_FAILURE);
	}

	renderer = SDL_CreateRenderer(window, nullptr);

	if (!renderer) {
		printf("\nERROR:: failed to create SDL Renderer: %s\n", SDL_GetError());
		std::exit(EXIT_FAILURE);
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
SDL_Texture* RenderWindow::loadNewTexture(const char* path)
{
	return IMG_LoadTexture(renderer, path);
}
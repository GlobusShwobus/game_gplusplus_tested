#include "Window.h"

Window::Window(const nlohmann::json* const windowConfig) {

	const auto& windowItem = (*windowConfig)["Window"];

	const int windowWidth = windowItem["window_width"];
	const int windowHeight = windowItem["window_height"];
	const int cameraWidth = windowItem["camera_width"];
	const int cameraHeight = windowItem["camera_height"];
	const int FPSTarget = windowItem["FPS_target"];
	const std::string title = windowItem["game_name"];


	window = SDL_CreateWindow(title.c_str(), windowWidth, windowHeight, SDL_WINDOW_RESIZABLE | SDL_WINDOW_OPENGL);

	renderer = SDL_CreateRenderer(window, nullptr);

	frameLimiter = FrameLimiter(FPSTarget);
	camera = Camera(cameraWidth, cameraHeight);

	//THIS IS NOT INTENDED BEHAVIOR, CAMERA SHOULD DETERMINE LOGICAL RENDERING NOT WINDOW SIZE, FIX LATER
	SDL_SetRenderLogicalPresentation(renderer, windowWidth, windowHeight, SDL_LOGICAL_PRESENTATION_STRETCH);

	//SDL_LOGICAL_PRESENTATION_DISABLED,     --> There is no logical size in effect 
	//SDL_LOGICAL_PRESENTATION_STRETCH,      --> The rendered content is stretched to the output resolution 
	//SDL_LOGICAL_PRESENTATION_LETTERBOX,    --> The rendered content is fit to the largest dimension and the other dimension is letterboxed with black bars 
	//SDL_LOGICAL_PRESENTATION_OVERSCAN,     --> The rendered content is fit to the smallest dimension and the other dimension extends beyond the output bounds 
	//SDL_LOGICAL_PRESENTATION_INTEGER_SCALE --> The rendered content is scaled up by integer multiples to fit the output resolution 
}
void Window::updateBegin() {
	SDL_RenderClear(renderer);
	frameLimiter.frameBufferBegin();
}
void Window::updateEnd() {
	SDL_RenderPresent(renderer);
	frameLimiter.frameBufferEnd();
}
SDL_Renderer* Window::getRenderer() {
	return renderer;
}
void Window::drawTexture(TextureData* const sprite)const {
	camera.applyDestinationFromCamera(&sprite->destination);
	SDL_RenderTexture(renderer, sprite->texture, &sprite->source, &sprite->destination);
}

void Window::Camera::setFocusPoint(const SDL_Point* const pos, const SDL_Point* const size) {
	center.x = pos->x + (size->x / 2);
	center.y = pos->y + (size->y / 2);
}
void Window::Camera::setTopLeft() {
	topLeft.x = center.x - radiusWidth;
	topLeft.y = center.y - radiusHeight;
}
void Window::Camera::clampTo(int x, int y, int w, int h) {
	if (center.x - radiusWidth < x)  { center.x = radiusWidth; }//left edge
	if (center.y - radiusHeight < y) { center.y = radiusHeight; }//top edge
	if (center.x + radiusWidth > w)  { center.x = w - radiusWidth; }//right edge
	if (center.y + radiusHeight > h) { center.y = h - radiusHeight; }//bottom edge
}
void Window::Camera::applyDestinationFromCamera(SDL_FRect* const entity)const {
	entity->x -= topLeft.x;
	entity->y -= topLeft.y;
};
void Window::FrameLimiter::frameBufferBegin() {
	frameBegin = SDL_GetTicks();
}
void Window::FrameLimiter::frameBufferEnd() {
	frameDuration = SDL_GetTicks() - frameBegin;
	if (frameDelay > frameDuration) {

		SDL_Delay(frameDelay - frameDuration);
	}
}

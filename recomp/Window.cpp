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

	frameLimiter.maximumFrameDuration = frameLimiter.ms_in_second / FPSTarget;

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
	frameLimiter.beginFrame();
}
void Window::updateEnd() {
	SDL_RenderPresent(renderer);
	frameLimiter.endFrame();
}
SDL_Renderer* Window::getRenderer() {
	return renderer;
}
void Window::drawTexture(SDL_Texture* texture, SDL_FRect* src, SDL_FRect* dest)const {
	SDL_FRect adjustedDest = camera.cameraSpaceDest(dest);
	SDL_RenderTexture(renderer, texture, src, &adjustedDest);
}
SDL_FRect Window::Camera::cameraSpaceDest(const SDL_FRect* const dest)const {
	return{ dest->x - rect.x, dest->y - rect.y, dest->w, dest->h };
};
void Window::FrameLimiter::beginFrame() {
	frameStart = SDL_GetTicks();
}
void Window::FrameLimiter::endFrame() {
	frameEnd = SDL_GetTicks();
	Uint64 duration = frameEnd - frameStart;

	isDelayActivated = false;
	delayDuration = 0;

	if (duration < maximumFrameDuration) {
		delayDuration = maximumFrameDuration - duration;
		isDelayActivated = true;
	}
}
bool Window::shouldDelay()const {
	return frameLimiter.isDelayActivated;
}
Uint64 Window::getDelayDuration()const {
	return frameLimiter.delayDuration;
}
void Window::updateCamera(const SDL_FRect& target, SDL_FRect clamp) {

	camera.rect.x = (target.x + (target.w / 2)) - camera.halfWidth;
	camera.rect.y = (target.y + (target.h / 2)) - camera.halfHeight;

	Collision::clampInOf(clamp, camera.rect);
}


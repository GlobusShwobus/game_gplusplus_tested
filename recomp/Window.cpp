#include "Window.h"

Window::Window(const nlohmann::json* const windowConfig) {

	const int windowWidth = (*windowConfig)["window_width"];
	const int windowHeight = (*windowConfig)["window_height"];
	const int cameraWidth = (*windowConfig)["camera_width"];
	const int cameraHeight = (*windowConfig)["camera_height"];
	const int FPSTarget = (*windowConfig)["FPS_target"];
	const std::string title = (*windowConfig)["game_name"];


	window = SDL_CreateWindow(title.c_str(), windowWidth, windowHeight, SDL_WINDOW_RESIZABLE | SDL_WINDOW_OPENGL);

	renderer = SDL_CreateRenderer(window, nullptr);

	frameLimiter = new FrameLimiter(FPSTarget);
	camera = new Camera(cameraWidth, cameraHeight);
	SDL_SetRenderLogicalPresentation(renderer, cameraWidth*2, cameraHeight*2, SDL_LOGICAL_PRESENTATION_STRETCH);

	//SDL_LOGICAL_PRESENTATION_DISABLED,     --> There is no logical size in effect 
	//SDL_LOGICAL_PRESENTATION_STRETCH,      --> The rendered content is stretched to the output resolution 
	//SDL_LOGICAL_PRESENTATION_LETTERBOX,    --> The rendered content is fit to the largest dimension and the other dimension is letterboxed with black bars 
	//SDL_LOGICAL_PRESENTATION_OVERSCAN,     --> The rendered content is fit to the smallest dimension and the other dimension extends beyond the output bounds 
	//SDL_LOGICAL_PRESENTATION_INTEGER_SCALE --> The rendered content is scaled up by integer multiples to fit the output resolution 
}
void Window::updateBegin() {
	SDL_RenderClear(renderer);
	frameLimiter->frameBufferBegin();
}
void Window::updateEnd() {
	SDL_RenderPresent(renderer);
	frameLimiter->frameBufferEnd();
}
SDL_Renderer* Window::getRenderer() {
	return renderer;
}
void Window::drawSprite(Sprite* sprite)const {
	SDL_FRect dest = camera->toCameraSpace(sprite->getDestination());
	SDL_RenderTexture(renderer, sprite->getTexture(), sprite->getSource(), &dest);
}

void Window::Camera::setFocusPoint(const SDL_FRect* const rect) {
	center.x = rect->x + (rect->w / 2);
	center.y = rect->y + (rect->h / 2);
}
void Window::Camera::clampTo(const SDL_FRect* const rect) {
	if (center.x - radiusWidth < rect->x)  { center.x = radiusWidth; }//left edge
	if (center.y - radiusHeight < rect->y) { center.y = radiusHeight; }//top edge
	if (center.x + radiusWidth > rect->w)  { center.x = rect->w - radiusWidth; }//right edge
	if (center.y + radiusHeight > rect->h) { center.y = rect->h - radiusHeight; }//bottom edge
}
SDL_FRect Window::Camera::toCameraSpace(const SDL_FRect* const entity)const {
	SDL_FRect screenPos{ 0,0,0,0 };

	screenPos.x = entity->x - (center.x - radiusWidth);
	screenPos.y = entity->y - (center.y - radiusHeight);

	screenPos.w = entity->w;
	screenPos.h = entity->h;

	return screenPos;
}


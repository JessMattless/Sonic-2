#include "App.h"
#include <SDL3/SDL_image.h>
#include <cmath>


void renderFilledRect(SDL_Renderer* renderer, float x, float y, int w, int h, SDL_Color color) {
	SDL_FRect rect;
	rect.x = x; rect.y = y;
	rect.w = w; rect.h = h;
	SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
	SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
	SDL_RenderFillRect(renderer, &rect);
}

void renderRect(SDL_Renderer* renderer, float x, float y, int w, int h, SDL_Color color) {
	SDL_FRect rect;
	rect.x = x; rect.y = y;
	rect.w = w; rect.h = h;
	SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
	SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
	SDL_RenderRect(renderer, &rect);
}

App::App()
{
	_running = true;

	SDL_Init(SDL_INIT_VIDEO);
	window = SDL_CreateWindow("Sonic 2 Level Editor", (SCREEN_WIDTH + OPTIONS_WIDTH), SCREEN_HEIGHT, 0);
	renderer = SDL_CreateRenderer(window, NULL, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ABGR8888, SDL_TEXTUREACCESS_STREAMING, SCREEN_WIDTH, SCREEN_HEIGHT);
	//SDL_SetRenderScale(renderer, 2, 2);

	camX = 0; camY = 0;
	tileSize = 16;

	selectedItem = nullptr;

	TTF_Init();
	font = TTF_OpenFont("../sonic-the-hedgehog-2-hud-font.ttf", 30);

	onZoneChange(new Zone(renderer, "Emerald Hill", 1, { 0, 34, 204, 255 }, "Emerald_Hill.png"));
	//currentZone = new Zone(renderer, "Emerald Hill", 1, {0, 34, 204, 255}, "Emerald_Hill.png");


	optionList.push_back(Text(renderer, font, "Act Name:", 20, OPTIONS_WIDTH, 0, false));
	optionList.push_back(Text(renderer, font, currentZone->zoneName,
		optionList[0].x + optionList[0].width, optionList[0].y, OPTIONS_WIDTH - 40 - optionList[0].width));
	optionList.push_back(Text(renderer, font, "Act Number:", 20, OPTIONS_WIDTH + 50, 0, false));
	optionList.push_back(Text(renderer, font, std::to_string(currentZone->actNo),
		optionList[2].x + optionList[2].width, optionList[2].y, OPTIONS_WIDTH - 40 - optionList[2].width, true, true, true));


	optionList.push_back(Button(renderer, font, "Save", OPTIONS_WIDTH - 120, SCREEN_HEIGHT - 50, 100, true, false));
	optionList.push_back(Button(renderer, font, "New", 20, SCREEN_HEIGHT - 50, 100, true, false));

	onExecute();
}

int App::onExecute()
{
	float timeA, timeB = 0;

	SDL_Event event;
	while (_running) {
		while (SDL_PollEvent(&event)) {
			onEvent(&event);
		}

		timeA = SDL_GetTicks();
		float deltaTime = timeA - timeB;
		if (deltaTime > 1000 / 20.0) {
			timeB = timeA;
			onLoop();
			onRender();
		}
	}

	onCleanup();
	return 0;
}

void App::onEvent(SDL_Event* event)
{
	switch (event->type) {
	case SDL_EVENT_QUIT:
		_running = false;
		break;
	case SDL_EVENT_KEY_DOWN:
		keyboard[event->key.keysym.sym] = true;
		if (selectedItem != nullptr) {
			if (shift) selectedItem->onType(event->key.keysym.sym, true);
			else selectedItem->onType(event->key.keysym.sym, false);
		}
		break;
	case SDL_EVENT_KEY_UP:
		keyboard[event->key.keysym.sym] = false;
		break;
	case SDL_EVENT_MOUSE_BUTTON_DOWN:
		mouse[event->button.button] = true;
		break;
	case SDL_EVENT_MOUSE_BUTTON_UP:
		mouse[event->button.button] = false;
		break;
	case SDL_EVENT_MOUSE_MOTION:
		mouseX = event->motion.x;
		mouseY = event->motion.y;
		movementX = event->motion.xrel;
		movementY = event->motion.yrel;
		break;
	case SDL_EVENT_MOUSE_WHEEL:
		mouseWheel = event->wheel.y;
		break;
	}

	if (keyboard[SDLK_LSHIFT] || keyboard[SDLK_RSHIFT]) shift = true;
	else shift = false;
}

void App::onLoop()
{
	SDL_Cursor* cursor = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_ARROW);

	for (int i = 0; i < optionList.size(); i++) {
		if (optionList[i].hoverable) {
			if (mouseX >= optionList[i].x + SCREEN_WIDTH && mouseX <= optionList[i].x + optionList[i].width + SCREEN_WIDTH
				&& mouseY >= optionList[i].y && mouseY <= optionList[i].y + optionList[i].height) {
				cursor = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_HAND);
				optionList[i].hovered = true;
			}
			else optionList[i].hovered = false;
		}
	}

	if (selectedItem == nullptr) {
		if (keyboard[SDLK_a] || keyboard[SDLK_LEFT]) {
			printf("Moving Left\n");
			camX += tileSize;
		}
		if (keyboard[SDLK_d] || keyboard[SDLK_RIGHT]) {
			printf("Moving Right\n");
			camX -= tileSize;
		}
		if (keyboard[SDLK_w] || keyboard[SDLK_UP]) {
			printf("Moving Up\n");
			camY += tileSize;
		}
		if (keyboard[SDLK_s] || keyboard[SDLK_DOWN]) {
			printf("Moving Down\n");
			camY -= tileSize;
		}
	
		if (keyboard[SDLK_EQUALS] || mouseWheel > 0) {
			if (tileSize < 64) tileSize++;
		}
		if (keyboard[SDLK_MINUS] || mouseWheel < 0) {
			if (tileSize > 16) tileSize--;
		}
	}
	
	if (mouse[SDL_BUTTON_LEFT]) {
		printf("Left Mouse Clicked\n");
		for (int i = 0; i < optionList.size(); i++) {
			if (optionList[i].editable && optionList[i].hovered) {
				selectedItem = &optionList[i];
				selectedItem->selected = true;
				break;
			}
			else {
				if (selectedItem != nullptr) {
					selectedItem->selected = false;
					selectedItem = nullptr;
				}
			}
		}

		if (selectedItem != nullptr) printf(selectedItem->text.c_str());
		else printf("Nothing");
		printf("\n");

		mouse[SDL_BUTTON_LEFT] = false;
	}
	if (mouse[SDL_BUTTON_RIGHT]) {
		if (mouseX > 0 && mouseX < SCREEN_WIDTH
			&& mouseY > 0 && mouseY < SCREEN_HEIGHT) {
			cursor = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_SIZEALL);
			printf("Right Mouse Held\n");
			camX += tileSize * movementX * 2;
			camY += tileSize * movementY * 2;
		}
		else {
			printf("Right Mouse Clicked\n");
			mouse[SDL_BUTTON_RIGHT] = false;
		}
	}

	if (mouseX >= SCREEN_WIDTH + 20 && mouseX <= SCREEN_WIDTH + OPTIONS_WIDTH - 20
		&& mouseY >= 20 && mouseY <= OPTIONS_WIDTH - 20) {
			cursor = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_HAND);
	}

	if (camX > tileSize * 10) camX = tileSize * 10;
	else if (camX < -((currentZone->zoneWidth * tileSize) + (tileSize * 10) - SCREEN_WIDTH)) 
		camX = -((currentZone->zoneWidth * tileSize) + (tileSize * 10) - SCREEN_WIDTH);

	if (camY > tileSize * 10) camY = tileSize * 10;
	else if (camY < -((currentZone->zoneHeight * tileSize) + (tileSize * 10) - SCREEN_HEIGHT)) 
		camY = -((currentZone->zoneHeight * tileSize) + (tileSize * 10) - SCREEN_HEIGHT);

	mouseWheel = 0; movementX = 0; movementY = 0;
	SDL_SetCursor(cursor);
}

void App::onRender()
{
	SDL_SetRenderDrawColor(renderer, 
		currentZone->backgroundColor.r, currentZone->backgroundColor.g, 
		currentZone->backgroundColor.b, currentZone->backgroundColor.a);
	SDL_RenderClear(renderer);

	currentZone->renderZone(camX, camY, tileSize);


	renderFilledRect(renderer, SCREEN_WIDTH, 0, OPTIONS_WIDTH, SCREEN_HEIGHT, currentZone->backgroundColor);
	renderRect(renderer, SCREEN_WIDTH, 0, OPTIONS_WIDTH, SCREEN_HEIGHT, { 0, 0, 0, 120 });
	currentZone->renderTileSet();

	for (OptionItem option : optionList) {
		option.render();
	}

	int tileScreenSize = 22;
	for (int x = SCREEN_WIDTH + 20; x < SCREEN_WIDTH + OPTIONS_WIDTH - 40; x += tileScreenSize) {
		// For width of tile set on screen, step up by size of one tile
		for (int y = 20; y < OPTIONS_WIDTH - 40; y += tileScreenSize) {
			if (mouseX >= x && mouseX < x + tileScreenSize
				&& mouseY >= y && mouseY < y + tileScreenSize) {
				renderFilledRect(renderer, x, y, tileScreenSize, tileScreenSize, { 255, 255, 255 ,120 });
			}
		}
	}

	SDL_RenderPresent(renderer);
}

void App::onCleanup()
{
	TTF_Quit();

	SDL_DestroyTexture(currentTileSet);
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
}

void App::onZoneChange(Zone* zone)
{
	currentZone = zone;
	currentTileSet = currentZone->tileSet;
}

int main(int argc, char* argv[]) {
	App app;

	return app.onExecute();
}
#pragma once

#include <SDL3/SDL.h>
#include <SDL3/SDL_ttf.h>
#include <SDL3/SDL_image.h>
#include <map>
#include <vector>
#include "OptionItem.h"
#include "Button.h"
#include "Zone.h"
#include "Text.h"

#define SCREEN_WIDTH 960
#define OPTIONS_WIDTH 480
#define SCREEN_HEIGHT 960

class App
{
private:
	bool _running, shift;
	SDL_Window* window = NULL;
	SDL_Renderer* renderer = NULL;
	SDL_Texture* texture = NULL;
	TTF_Font* font = NULL;

	int tileScreenSize = 22;
	int activeTile = 0;

	float camX, camY;
	float mouseX, mouseY;
	float movementX, movementY;
	int mouseWheel;

	int tileSize;

	Zone* currentZone;
	OptionItem* selectedItem;

	std::map<int, bool> keyboard;
	std::map<int, bool> mouse;

	SDL_Texture* currentTileSet;
	std::vector<OptionItem> optionList;

public:
	App(); // Used as our init function

	int onExecute();

	void onEvent(SDL_Event* Event);

	void onLoop();

	void onRender();

	void onCleanup();

	void onZoneChange(Zone* zone);
};
#include "App.h"
#include <SDL3/SDL_image.h>
#include <cmath>

// Handle input to the custom textboxes
void handleTextboxInput(OptionItem* selectedItem, Zone* currentZone, bool save) {
	if (selectedItem->name == "ZoneName") {
		if (save) currentZone->zoneName = selectedItem->text;
		else selectedItem->text = currentZone->zoneName;
	}
	else if (selectedItem->name == "ActNo") {
		if (save) {
			selectedItem->text.erase(0, selectedItem->text.find_first_not_of('0'));
			if (selectedItem->text.size() == 0 || selectedItem->text == "0") selectedItem->text = "1";
			currentZone->actNo = stoi(selectedItem->text);
		}
		else {
			selectedItem->text = std::to_string(currentZone->actNo);
		}
	}

	selectedItem->updateText();
}

// Get the position of the selected tile in the options bar
int* getActiveTilePos(int tile, int tileScreenSize) {
	int pos[2];

	pos[0] = (tile % 20) * tileScreenSize + SCREEN_WIDTH + 20;
	pos[1] = (tile / 20) * tileScreenSize + 20;

	return pos;
}

App::App()
{
	_running = true;

	SDL_Init(SDL_INIT_VIDEO);
	window = SDL_CreateWindow("Sonic 2 Level Editor", (SCREEN_WIDTH + OPTIONS_WIDTH), SCREEN_HEIGHT, 0);
	renderer = SDL_CreateRenderer(window, NULL, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ABGR8888, SDL_TEXTUREACCESS_STREAMING, SCREEN_WIDTH, SCREEN_HEIGHT);

	TTF_Init();
	this->font = TTF_OpenFont("../sonic-the-hedgehog-2-hud-font.ttf", 30);

	this->gameRenderer = new Renderer(renderer);
	this->optionMenu = new OptionMenu(renderer, font, {0, 0, 0, 120});

	camX = 0; camY = 0;
	tileSize = 16;

	selectedItem = nullptr;

	loadDefaultZone();
	
	optionMenu->addMenuItem(TextInput, "ZoneName", currentZone->zoneName, "Zone Name:");
	optionMenu->addMenuItem(NumberInput, "ActNo", std::to_string(currentZone->actNo), "Act Number:");
	optionMenu->addMenuItem(Button, "SaveButton", "Save");
	optionMenu->addMenuItem(Button, "LoadButton", "Load", "", true);
	optionMenu->addMenuItem(Button, "NewButton", "New", "", true);

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
		if (selectedItem != nullptr) selectedItem->onType(event->key.keysym.sym);
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
}

void App::onLoop()
{
	// Default cursor
	SDL_Cursor* cursor = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_ARROW);

	// If hovering over an option in the menu, set it as hovered and change the cursor.
	for (int i = 0; i < optionMenu->options.size(); i++) {
		if (optionMenu->options[i].type == Button || optionMenu->options[i].type == TextInput || optionMenu->options[i].type == NumberInput) {
			if (mouseX >= optionMenu->options[i].rect->x && mouseX <= optionMenu->options[i].rect->x + optionMenu->options[i].rect->w
				&& mouseY >= optionMenu->options[i].rect->y && mouseY <= optionMenu->options[i].rect->y + optionMenu->options[i].rect->h) {
				cursor = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_HAND);
				optionMenu->options[i].hovered = true;
			}
			else optionMenu->options[i].hovered = false;
		}
	}

	// Check if a menu option is selected
	if (selectedItem == nullptr) {
		// Camera movement using keyboard keys
		if (keyboard[SDLK_a] || keyboard[SDLK_LEFT]) camX += tileSize;
		if (keyboard[SDLK_d] || keyboard[SDLK_RIGHT]) camX -= tileSize;
		if (keyboard[SDLK_w] || keyboard[SDLK_UP]) camY += tileSize;
		if (keyboard[SDLK_s] || keyboard[SDLK_DOWN]) camY -= tileSize;
	
		// Camera zoom using keyboard keys
		if (keyboard[SDLK_EQUALS] || mouseWheel > 0) if (tileSize < 64) tileSize++;
		if (keyboard[SDLK_MINUS] || mouseWheel < 0) if (tileSize > 16) tileSize--;
	}
	else {
		if (selectedItem->type == TextInput || selectedItem->type == NumberInput) {
			// If return is pressed, confirm edit to menu option
			if (keyboard[SDLK_RETURN]) {
				handleTextboxInput(selectedItem, currentZone, true);

				selectedItem->selected = false;
				selectedItem = nullptr;
			}
			// If escape is pressed, disgard edit to menu option
			if (keyboard[SDLK_ESCAPE]) {
				handleTextboxInput(selectedItem, currentZone, false);

				selectedItem->selected = false;
				selectedItem = nullptr;
			}
		}
		else if (selectedItem->type == Button) {
			if (selectedItem->name == "NewButton") {
				int result = 0;

				const SDL_MessageBoxButtonData buttons[] = { 
					{SDL_MESSAGEBOX_BUTTON_ESCAPEKEY_DEFAULT, -1, "No"}, 
					{SDL_MESSAGEBOX_BUTTON_RETURNKEY_DEFAULT, 1, "Yes"} 
				};

				SDL_MessageBoxData boxData{ SDL_MESSAGEBOX_WARNING, window, "Confirm", "Are you Sure?", 2, buttons, NULL };
				SDL_ShowMessageBox(&boxData, &result);

				if (result == 1) loadDefaultZone();
			}
			else if (selectedItem->name == "SaveButton") {
				currentZone->saveZone();
			}
			else if (selectedItem->name == "LoadButton") {
				//selectedItem->returnToDefault();
			}
			//TODO: sort out button inputs for individual buttons.
			selectedItem->selected = false;
			selectedItem = nullptr;
		}
	}
	
	if (mouse[SDL_BUTTON_LEFT]) {
		// Check if an option has been clicked. If it has, then if it can be selected, select it.
		// Else, if anywhere else is clicked then unselect any selected items.
		for (int i = 0; i < optionMenu->options.size(); i++) {
			if (optionMenu->options[i].hovered) {
				if (optionMenu->options[i].type == TextInput || optionMenu->options[i].type == NumberInput 
					|| optionMenu->options[i].type == Button) {
					selectedItem = &optionMenu->options[i];
					selectedItem->selected = true;
					break;
				}
			}
			else {
				if (selectedItem != nullptr) {
					handleTextboxInput(selectedItem, currentZone, true);

					selectedItem->selected = false;
					selectedItem = nullptr;
				}
			}
		}

		// If a tile is clicked on the tilemap within the options menu, then set it as the active drawing tile.
		for (int x = 0; x < 20; x++) {
			for (int y = 0; y < 20; y++) {
				int xPos = (x * tileScreenSize) + SCREEN_WIDTH + 20;
				int yPos = (y * tileScreenSize) + 20;
				if (mouseX >= xPos && mouseX < xPos + tileScreenSize
					&& mouseY >= yPos && mouseY < yPos + tileScreenSize) {
					activeTile = x + (y * 20);
				}
			}
		}

		// If the mouse is clicked outside of the option menu, place a tile at the selected area.
		if (mouseX < SCREEN_WIDTH) {
			for (int x = 0; x < currentZone->zoneWidth; x++) {
				for (int y = 0; y < currentZone->zoneHeight; y++) {
					int xPos = (x * tileSize) + camX;
					int yPos = (y * tileSize) + camY;
					if (mouseX >= xPos && mouseX < xPos + tileSize
						&& mouseY >= yPos && mouseY < yPos + tileSize) {
						Tile* currentTile = &currentZone->mapSet[x + (y * currentZone->zoneWidth)];
						currentTile->tileMapIndex = activeTile;

						if (SDL_GetModState() & SDL_KMOD_LSHIFT) currentTile->flipH = !currentTile->flipH;
						if (SDL_GetModState() & SDL_KMOD_LCTRL) currentTile->flipV = !currentTile->flipV;

						goto endL;
					}
				}
			}
		}

		// If mouse is being held in the main screen, and is moved to the menu, unclick the mouse
		endL:
		mouse[SDL_BUTTON_LEFT] = false;
	}
	if (mouse[SDL_BUTTON_RIGHT]) {
		// If the mouse is clicked outside of the option menu, remove a tile at the selected area.
		if (mouseX < SCREEN_WIDTH) {
			for (int x = 0; x < currentZone->zoneWidth; x++) {
				for (int y = 0; y < currentZone->zoneHeight; y++) {
					int xPos = (x * tileSize) + camX;
					int yPos = (y * tileSize) + camY;
					if (mouseX >= xPos && mouseX < xPos + tileSize
						&& mouseY >= yPos && mouseY < yPos + tileSize) {
						Tile* currentTile = &currentZone->mapSet[x + (y * currentZone->zoneWidth)];
						currentTile->tileMapIndex = 0;

						goto endR;
					}
				}
			}
		}

		// If mouse is being held in the main screen, and is moved to the menu, unclick the mouse
		endR:
		mouse[SDL_BUTTON_RIGHT] = false;
	}
	if (mouse[SDL_BUTTON_MIDDLE]) {
		// if the mouse is within the bounds of the editor (not including options)
		// then move the camera by the amount dragged by the mouse.
		if (mouseX > 0 && mouseX < SCREEN_WIDTH
			&& mouseY > 0 && mouseY < SCREEN_HEIGHT) {
			cursor = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_SIZEALL);
			camX += tileSize * movementX * 2;
			camY += tileSize * movementY * 2;
		}
	}

	// Change the cursor when hovering over the tileset in the options menu
	if (mouseX >= SCREEN_WIDTH + 20 && mouseX <= SCREEN_WIDTH + OPTIONS_WIDTH - 20
		&& mouseY >= 20 && mouseY <= OPTIONS_WIDTH - 20) {
			cursor = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_HAND);
	}

	// Limit the bounds of the camera on the X and Y axis so you can't move too far from the level.
	if (camX > tileSize * 10) camX = tileSize * 10;
	else if (camX < -((currentZone->zoneWidth * tileSize) + (tileSize * 10) - SCREEN_WIDTH)) 
		camX = -((currentZone->zoneWidth * tileSize) + (tileSize * 10) - SCREEN_WIDTH);

	if (camY > tileSize * 10) camY = tileSize * 10;
	else if (camY < -((currentZone->zoneHeight * tileSize) + (tileSize * 10) - SCREEN_HEIGHT)) 
		camY = -((currentZone->zoneHeight * tileSize) + (tileSize * 10) - SCREEN_HEIGHT);

	// Reset movement/zoom vectors for the next frame.
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

	// Render the options menu background/border.
	gameRenderer->renderFilledRect({ SCREEN_WIDTH, 0, OPTIONS_WIDTH, SCREEN_HEIGHT }, currentZone->backgroundColor);
	gameRenderer->renderRect({ SCREEN_WIDTH, 0, OPTIONS_WIDTH, SCREEN_HEIGHT }, { 0, 0, 0, 120 });

	currentZone->renderTileSet();

	for (OptionItem option : optionMenu->options) {
		option.render();
	}

	// Render a rect over the selected tile to give indication it is selected.
	int* activeTilePos = getActiveTilePos(activeTile, tileScreenSize);
	gameRenderer->renderRect({ (float)activeTilePos[0], (float)activeTilePos[1], (float)tileScreenSize, (float)tileScreenSize }, { 255, 255, 255 ,255 });

	// when hovering over a tile in the options menu, highlight it.
	for (int x = SCREEN_WIDTH + 20; x < SCREEN_WIDTH + OPTIONS_WIDTH - 40; x += tileScreenSize) {
		for (int y = 20; y < OPTIONS_WIDTH - 40; y += tileScreenSize) {
			if (mouseX >= x && mouseX < x + tileScreenSize
				&& mouseY >= y && mouseY < y + tileScreenSize) {
				gameRenderer->renderFilledRect({ (float)x, (float)y, (float)tileScreenSize, (float)tileScreenSize }, { 255, 255, 255 ,120 });
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

void App::loadDefaultZone()
{
	currentZone = new Zone(renderer, "Zone Name", 1, { 0, 34, 204, 255 }, "Emerald_Hill.png");
	currentTileSet = currentZone->tileSet;
	for (int i = 0; i < optionMenu->options.size(); i++) {
		if (optionMenu->options[i].name == "ZoneName") optionMenu->options[i].text = currentZone->zoneName;
		else if (optionMenu->options[i].name == "ActNo") optionMenu->options[i].text = std::to_string(currentZone->actNo);
		optionMenu->options[i].updateText();
	}
}

int main(int argc, char* argv[]) {
	App app;

	return app.onExecute();
}
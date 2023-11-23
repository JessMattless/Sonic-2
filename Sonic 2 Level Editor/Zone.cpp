#include "Zone.h"
#include <SDL3/SDL_image.h>
#include "App.h"

Zone::Zone(SDL_Renderer* renderer, std::string zoneName, int actNo, SDL_Color background, std::string tileSetPath, int width, int height)
{
	std::string zonePath = "../Zone Tilesets/" + tileSetPath;
	SDL_Surface* tileSetSurface = IMG_Load(zonePath.c_str());
	Uint32 colorKey = SDL_MapRGB(tileSetSurface->format, ALPHA_KEY);
	SDL_SetSurfaceColorKey(tileSetSurface, SDL_TRUE, colorKey);

	this->renderer = renderer;
	this->zoneName = zoneName;
	this->actNo = actNo;
	this->backgroundColor = background;
	this->tileSet = SDL_CreateTextureFromSurface(renderer, tileSetSurface);

	if (width > 100) this->zoneWidth = width;
	else this->zoneWidth = 100;
	if (height > 64) this->zoneHeight = height;
	else this->zoneHeight = 64;
}

void Zone::renderTileSet()
{
	SDL_FRect textureRect;
	textureRect.x = SCREEN_WIDTH + 20;
	textureRect.y = 20;
	textureRect.w = OPTIONS_WIDTH - 40;
	textureRect.h = OPTIONS_WIDTH - 40;

	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 120);
	SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
	SDL_RenderRect(renderer, &textureRect);
	SDL_RenderTexture(renderer, tileSet, NULL, &textureRect);
}

void Zone::renderZone(float camX, float camY, int tileSize) {
	for (int x = 0; x < zoneWidth; x++) {
		for (int y = 0; y < zoneHeight; y++) {
			SDL_FRect rect;
			rect.x = (x * tileSize) + camX;
			rect.y = (y * tileSize) + camY;
			rect.w = tileSize;
			rect.h = tileSize;

			SDL_SetRenderDrawColor(renderer, 0, 0, 0, 150);
			SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
			SDL_RenderRect(renderer, &rect);
		}
	}
}

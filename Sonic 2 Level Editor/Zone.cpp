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


	if (width >= 100) this->zoneWidth = width;
	else this->zoneWidth = 100;
	if (height >= 64) this->zoneHeight = height;
	else this->zoneHeight = 64;

	//this->currentMapSet.reserve(zoneWidth * zoneHeight);
	for (int i = 0; i < zoneWidth * zoneHeight; i++) mapSet.push_back(0);
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
	for (int i = 0; i < mapSet.size(); i++) {
		int tileType = mapSet[i];
		bool flipped = tileType > 400 ? true : false;

		if (flipped) tileType -= 400;

		if (i == zoneWidth + 10) {
			printf("Tile: ");
			printf(std::to_string(tileType).c_str());
			printf("\n");
		}


		int xPos = (i % zoneWidth) * tileSize + camX;
		int yPos = (i / zoneWidth) * tileSize + camY;

		SDL_FRect worldTile;
		worldTile.x = xPos;
		worldTile.y = yPos;
		worldTile.w = tileSize;
		worldTile.h = tileSize;

		if (tileType == 0) {
			SDL_SetRenderDrawColor(renderer, 0, 0, 0, 150);
			SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
			SDL_RenderRect(renderer, &worldTile);
		}
		else {
			SDL_FRect textureTile;
			textureTile.x = (tileType % 20) * 16;
			textureTile.y = (tileType / 20) * 16;
			textureTile.w = 16;
			textureTile.h = 16;

			if (flipped) SDL_RenderTextureRotated(renderer, tileSet, &textureTile, &worldTile, 0.0, NULL, SDL_FLIP_HORIZONTAL);
			else SDL_RenderTexture(renderer, tileSet, &textureTile, &worldTile);
		}
	}
	//for (int x = 0; x < zoneWidth; x++) {
	//	for (int y = 0; y < zoneHeight; y++) {
	//		SDL_FRect rect;
	//		rect.x = (x * tileSize) + camX;
	//		rect.y = (y * tileSize) + camY;
	//		rect.w = tileSize;
	//		rect.h = tileSize;

	//		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 150);
	//		SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
	//		SDL_RenderRect(renderer, &rect);
	//	}
	//}
}

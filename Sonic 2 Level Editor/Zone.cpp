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
	this->tileSetPath = tileSetPath;
	this->zoneName = zoneName;
	this->actNo = actNo;
	this->backgroundColor = background;
	this->tileSet = SDL_CreateTextureFromSurface(renderer, tileSetSurface);


	if (width >= 100) this->zoneWidth = width;
	else this->zoneWidth = 100;
	if (height >= 64) this->zoneHeight = height;
	else this->zoneHeight = 64;

	//this->currentMapSet.reserve(zoneWidth * zoneHeight);
	for (int i = 0; i < zoneWidth * zoneHeight; i++) mapSet.push_back(Tile());
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
		int tileIndex = mapSet[i].tileMapIndex;
		
		int xPos = (i % zoneWidth) * tileSize + camX;
		int yPos = (i / zoneWidth) * tileSize + camY;

		SDL_FRect worldTile{xPos, yPos, tileSize, tileSize};

		if (tileIndex == 0) {
			SDL_SetRenderDrawColor(renderer, 0, 0, 0, 150);
			SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
			SDL_RenderRect(renderer, &worldTile);
		}
		else {
			SDL_FRect textureTile{ (tileIndex % 20) * TILE_SIZE, (tileIndex / 20) * TILE_SIZE, TILE_SIZE, TILE_SIZE };

			int flip = SDL_FLIP_NONE;
			if (mapSet[i].flipH && mapSet[i].flipV) flip = SDL_FLIP_HORIZONTAL | SDL_FLIP_VERTICAL;
			else if (mapSet[i].flipH) flip = SDL_FLIP_HORIZONTAL;
			else if (mapSet[i].flipV) flip = SDL_FLIP_VERTICAL;

			SDL_RenderTextureRotated(renderer, tileSet, &textureTile, &worldTile, 0.0, NULL, (SDL_RendererFlip)flip);
		}
	}
}

// Save the zone under /Zones/XXN.zone:
// X: First letter of each word in zone
// N: Act number
void Zone::saveZone()
{
	std::string fileName = "../Zones/";

	std::istringstream iss(zoneName);
	std::string word;
	while (std::getline(iss, word, ' ')) {
		fileName += toupper(word[0]);
	}

	fileName += std::to_string(actNo) + ".zone";

	//std::replace(fileName.begin(), fileName.end(), ' ', '_');
	printf(fileName.c_str());

	std::ofstream ZoneFile(fileName, std::ofstream::trunc);
	ZoneFile << "Zone Name: " + zoneName + "\n";
	ZoneFile << "Act Number: " + std::to_string(actNo) + "\n";
	ZoneFile << "Sprite Set: " + tileSetPath + "\n";
	for (int y = 0; y < zoneHeight; y++) {
		for (int x = 0; x < zoneWidth; x++) {
			ZoneFile << "0x0000 ";
		}
		ZoneFile << "\n";
	}

	ZoneFile.close();
}

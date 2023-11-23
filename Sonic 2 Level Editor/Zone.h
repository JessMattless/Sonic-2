#pragma once
#include <SDL3/SDL.h>
#include <string>

#define ALPHA_KEY 203, 0, 212

struct Zone
{
private:
	SDL_Renderer* renderer;

public:
	SDL_Color backgroundColor;
	SDL_Texture* tileSet;
	int zoneWidth, zoneHeight;
	std::string zoneName;
	int actNo;

	Zone(SDL_Renderer* renderer, std::string zoneName, int actNo, SDL_Color background, std::string tileSetPath, int width = 100, int height = 64);
	void renderTileSet();
	void renderZone(float camX, float camY, int tileSize);
};
#pragma once

#include <SDL3/SDL.h>

struct Renderer
{
private:
	SDL_Renderer* renderer;

public:
	Renderer(SDL_Renderer* renderer);

	void renderRect(SDL_FRect rect, SDL_Color color);
	void renderFilledRect(SDL_FRect rect, SDL_Color color);
};


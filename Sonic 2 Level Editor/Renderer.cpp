#include "Renderer.h"

Renderer::Renderer(SDL_Renderer* renderer)
{
	this->renderer = renderer;
}

void Renderer::renderRect(SDL_FRect rect, SDL_Color color)
{
	SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
	SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
	SDL_RenderRect(renderer, &rect);
}

void Renderer::renderFilledRect(SDL_FRect rect, SDL_Color color)
{
	SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
	SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
	SDL_RenderFillRect(renderer, &rect);
}

#include "Button.h"

//Button::Button()
//{
//	this->buttonText = text;
//	this->x = x;
//	this->y = y;
//	this->width = width;
//	this->height = 0;
//	this->hovered = false;
//
//	this->renderer = renderer;
//	this->font = font;
//
//	TTF_SizeUTF8(font, buttonText.c_str(), &textWidth, &height);
//	surface = TTF_RenderUTF8_Solid(font, buttonText.c_str(), { 255, 255, 255, 255 });
//	message = SDL_CreateTextureFromSurface(renderer, surface);
//}

//void Button::render()
//{
//	SDL_FRect button;
//	button.x = x + 960;
//	button.y = y;
//	button.w = width;
//	button.h = height;
//
//	if (this->hovered) SDL_SetRenderDrawColor(renderer, 0, 0, 0, 190);
//	else SDL_SetRenderDrawColor(renderer, 0, 0, 0, 120);
//	SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
//	SDL_RenderFillRect(renderer, &button);
//
//	SDL_FRect textRect;
//	textRect.x = x + 960 + ((width - textWidth) / 2);
//	textRect.y = y;
//	textRect.w = textWidth;
//	textRect.h = height;
//
//	SDL_RenderTexture(renderer, message, NULL, &textRect);
//}
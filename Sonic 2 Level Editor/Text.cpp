#include "Text.h"

//Text::Text()
//{
//	//
//	//this->text = text;
//	//this->x = x;
//	//this->y = y;
//	//this->width = width;
//	//this->height = 0;
//	//this->hovered = false;
//	//this->editable = editable;
//
//	//this->renderer = renderer;
//	//this->font = font;
//
//	//TTF_SizeUTF8(font, text.c_str(), &textWidth, &height);
//	//surface = TTF_RenderUTF8_Solid(font, text.c_str(), { 255, 255, 255, 255 });
//	//message = SDL_CreateTextureFromSurface(renderer, surface);
//
//	//if (this->width == 0) this->width = textWidth + 30;
//}

//void Text::render()
//{
//	SDL_FRect rect;
//	rect.x = x + 960;
//	rect.y = y;
//	rect.w = width;
//	rect.h = height;
//
//	if (this->hovered) SDL_SetRenderDrawColor(renderer, 0, 0, 0, 190);
//	else SDL_SetRenderDrawColor(renderer, 0, 0, 0, 120);
//	//SDL_SetRenderDrawColor(renderer, 0, 0, 0, 120);
//	SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
//	SDL_RenderFillRect(renderer, &rect);
//
//	SDL_FRect textRect;
//	textRect.x = x + 960 + ((width - textWidth) / 2);
//	textRect.y = y;
//	textRect.w = textWidth;
//	textRect.h = height;
//
//	SDL_RenderTexture(renderer, message, NULL, &textRect);
//}
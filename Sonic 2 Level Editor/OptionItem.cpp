#include "OptionItem.h"
#include <iostream>

OptionItem::OptionItem(SDL_Renderer* renderer, TTF_Font* font, std::string text, float x, float y, int width, bool hoverable, bool editable, bool numbersOnly)
{
	this->text = text;
	this->x = x;
	this->y = y;
	this->width = width;
	this->height = 0;
	this->hoverable = hoverable;
	this->hovered = false;
	if (this->hoverable) this->editable = editable;
	else this->editable = false;
	this->numbersOnly = numbersOnly;

	this->renderer = renderer;
	this->font = font;

	TTF_SizeUTF8(font, text.c_str(), &textWidth, &height);
	surface = TTF_RenderUTF8_Solid(font, text.c_str(), { 255, 255, 255, 255 });
	message = SDL_CreateTextureFromSurface(renderer, surface);

	if (this->width == 0) this->width = textWidth + 30;
}

void OptionItem::render()
{
	SDL_FRect rect;
	rect.x = x + 960;
	rect.y = y;
	rect.w = width;
	rect.h = height;

	if (this->selected) SDL_SetRenderDrawColor(renderer, 0, 0, 0, 220);
	else if (this->hovered && !this->selected) SDL_SetRenderDrawColor(renderer, 0, 0, 0, 190);
	else SDL_SetRenderDrawColor(renderer, 0, 0, 0, 120);
	//SDL_SetRenderDrawColor(renderer, 0, 0, 0, 120);
	SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
	SDL_RenderFillRect(renderer, &rect);

	SDL_FRect textRect;
	textRect.x = x + 960 + ((width - textWidth) / 2);
	textRect.y = y;
	textRect.w = textWidth;
	textRect.h = height;

	SDL_RenderTexture(renderer, message, NULL, &textRect);
}

void OptionItem::onType(char ch, bool shift)
{
	if (ch == 8 && text.size() > 0) {
		text.pop_back();
		std::cout << "backspace" << std::endl;
	}
	else if ((ch >= 48 && ch <= 57)) { // Numbers
		std::cout << ch << std::endl;
		text.push_back(ch);
	}
	else if (!numbersOnly && ch == 32) { // Space
		text.push_back(' ');
		std::cout << "space" << std::endl;
	}
	else if (!numbersOnly && ((ch >= 97 && ch <= 122))) {
		std::cout << ch << std::endl;
		if(shift) text.push_back(ch - 32);
		else text.push_back(ch);
	}

	TTF_SizeUTF8(font, text.c_str(), &textWidth, &height);
	surface = TTF_RenderUTF8_Solid(font, text.c_str(), { 255, 255, 255, 255 });
	message = SDL_CreateTextureFromSurface(renderer, surface);
}

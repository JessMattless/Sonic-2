#pragma once
#include <string>
#include <SDL3/SDL.h>
#include <SDL3/SDL_ttf.h>
#include "OptionItem.h"


struct Button : OptionItem
{
	//std::string buttonText;
	//float x, y;
	//int width, height, textWidth;
	//bool hovered;

	//SDL_Surface* surface = nullptr;
	//SDL_Texture* message = nullptr;

	//SDL_Renderer* renderer;
	//TTF_Font* font;

	using OptionItem::OptionItem;

	//Button();
	//void render();
};


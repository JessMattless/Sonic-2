#pragma once
#include <string>
#include <SDL3/SDL.h>
#include <SDL3/SDL_ttf.h>
#include "OptionItem.h"


struct Text : OptionItem
{
	//std::string text;
	//float x, y;
	//int width, height, textWidth, textHeight;
	//bool hovered, editable;

	//SDL_Surface* surface = nullptr;
	//SDL_Texture* message = nullptr;

	//SDL_Renderer* renderer;
	//TTF_Font* font;

	using OptionItem::OptionItem;

	//Text();
	//void render();
};


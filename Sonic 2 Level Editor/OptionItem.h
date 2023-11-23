#pragma once
#include <string>
#include <SDL3/SDL.h>
#include <SDL3/SDL_ttf.h>

struct OptionItem
{
	std::string name, text;
	float x, y;
	int width, height, textWidth;
	bool hovered, hoverable, editable, selected, numbersOnly;

	SDL_Surface* surface = nullptr;
	SDL_Texture* message = nullptr;

	SDL_Renderer* renderer;
	TTF_Font* font;

	OptionItem(SDL_Renderer* renderer, TTF_Font* font, std::string name, std::string text, float x, float y, int width = 0, bool hoverable = true, bool editable = true, bool numbersOnly = false);
	void render();
	void onType(char ch, bool shift);
	void updateText();
};


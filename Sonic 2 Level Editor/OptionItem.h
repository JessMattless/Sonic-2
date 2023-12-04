#pragma once
#include <string>
#include <SDL3/SDL.h>
#include <SDL3/SDL_ttf.h>

#include "Settings.h"

enum Type { Button, Label, TextInput, NumberInput };

struct OptionItem
{
private:
	int textSize[2];
	SDL_FRect* textRect;

	SDL_Surface* surface = nullptr;
	SDL_Texture* message = nullptr;

	SDL_Renderer* renderer;
	TTF_Font* font;
public:
	SDL_FRect* rect;
	bool hovered = false;
	bool selected = false;

	Type type;
	std::string name, text;
	SDL_Color color;

	OptionItem(SDL_Renderer* renderer, TTF_Font* font, Type type, std::string name, std::string bodyText, SDL_FRect* rect, SDL_Color color = { 0, 0, 0 });
	void calculateTextSize();
	void render();
	void onType(char ch);
	void updateText();
};

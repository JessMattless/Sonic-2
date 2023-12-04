#include "OptionItem.h"
#include <iostream>

/* If color is excluded, then the default is black. Alpha is ignored in the color.
If -1 is given as the rect width/height then the text width/height is used.
The X coordinate given to the rect is in relation to the option window.
*/
OptionItem::OptionItem(SDL_Renderer* renderer, TTF_Font* font, Type type, std::string name, 
	std::string bodyText, SDL_FRect* rect, SDL_Color color)
{
	this->renderer = renderer;
	this->font = font;

	this->type = type;

	this->name = name;
	this->text = bodyText;
	this->defaultText = bodyText;

	this->rect = rect;
	this->color = color;

	this->rect->x += SCREEN_WIDTH;

	// Below is math for centering text
	calculateTextSize();

	if (this->rect->w < 0) this->rect->w = textSize[0] + (MENU_PADDING * 2);
	if (this->rect->h < 0) this->rect->h = textSize[1];

	this->textRect = new SDL_FRect(*rect);
	textRect->x +=  MENU_PADDING;


	updateText();
}

// Calculate the width and height of the option text
void OptionItem::calculateTextSize()
{
	TTF_SizeUTF8(font, text.c_str(), &textSize[0], &textSize[1]);
}

// Render the option item on screen
void OptionItem::render()
{
	if ((type == Button || type == TextInput|| type == NumberInput) && this->selected) SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, 220);
	else if ((type == Button || type == TextInput || type == NumberInput) && this->hovered) SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, 190);
	else SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, 120);
	SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
	SDL_RenderFillRect(renderer, rect);

	

	SDL_RenderTexture(renderer, message, NULL, textRect);
}

void OptionItem::onType(char ch)
{
	if (type == TextInput || type == NumberInput) {
		// Backspace
		if (ch == 8 && text.size() > 0) {
			text.pop_back();
		}
		// Numbers
		else if ((ch >= 48 && ch <= 57)) {
			text.push_back(ch);
		}
		// Space
		else if (type != NumberInput && ch == 32) {
			text.push_back(' ');
		}
		// Lowercase/Uppercase
		else if (type != NumberInput && ((ch >= 97 && ch <= 122))) {
			if (SDL_GetModState() & SDL_KMOD_SHIFT) text.push_back(ch - 32);
			else text.push_back(ch);
		}

		updateText();
	}
}

void OptionItem::updateSize()
{
	updateText();
	this->rect->w = textSize[0] + (MENU_PADDING * 2);
	this->rect->h = textSize[1];
}

// Update text size/length/position
void OptionItem::updateText()
{
	calculateTextSize();
	textRect->w = textSize[0];
	textRect->h = textSize[1];
	surface = TTF_RenderUTF8_Blended(font, text.c_str(), { 255, 255, 255, 255 });
	message = SDL_CreateTextureFromSurface(renderer, surface);
}

void OptionItem::returnToDefault()
{
	this->color = { 0, 0, 0 };
	this->text = defaultText;
	updateSize();
}
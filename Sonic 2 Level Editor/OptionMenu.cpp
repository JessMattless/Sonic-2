#include "OptionMenu.h"

OptionMenu::OptionMenu(SDL_Renderer* renderer, TTF_Font* font, SDL_Color background)
{
	this->renderer = renderer;
	this->font = font;
	this->backgroundColor = background;
}

void OptionMenu::addMenuItem(Type type, std::string itemName, std::string itemText, std::string labelText, bool inlineWithPrev)
{
	int itemX = MENU_PADDING;
	int itemY = OPTIONS_WIDTH;
	if (!inlineWithPrev) {
		for (OptionItem item : options) {
			if (item.rect->y >= itemY) itemY = item.rect->y + 50;
		}
	}
	else {
		itemX = options.back().rect->x + options.back().rect->w + 20 - SCREEN_WIDTH;
		itemY = options.back().rect->y;
	}

	if (labelText != "") {
		this->options.push_back(OptionItem(renderer, font, Label, itemName + "Label", labelText, new SDL_FRect{ MENU_PADDING, (float)itemY, -1, -1 }));

		this->options.push_back(OptionItem(renderer, font, type, itemName, itemText, new SDL_FRect{ itemX + options.back().rect->w, (float)itemY, OPTIONS_WIDTH - (MENU_PADDING * 2) - options.back().rect->w, -1 }));
	}
	else {
		this->options.push_back(OptionItem(renderer, font, type, itemName, itemText, new SDL_FRect{ (float)itemX, (float)itemY, -1, -1 }));
	}

	
}

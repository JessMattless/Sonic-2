#include "OptionMenu.h"

OptionMenu::OptionMenu(SDL_Renderer* renderer, TTF_Font* font, SDL_Color background)
{
	this->renderer = renderer;
	this->font = font;
	this->backgroundColor = background;
}

void OptionMenu::addMenuItem(Type type, std::string itemName, std::string itemText, std::string labelText, bool inlineWithPrev)
{
	int itemX = settings.MENU_PADDING;
	int itemY = settings.OPTIONS_WIDTH;
	if (!inlineWithPrev) {
		for (OptionItem item : options) {
			if (item.rect->y >= itemY) itemY = item.rect->y + 50;
		}
	}
	else {
		itemX = options.back().rect->x + options.back().rect->w + 20 - settings.SCREEN_WIDTH;
		itemY = options.back().rect->y;
	}

	if (labelText != "") {
		this->options.push_back(OptionItem(renderer, font, Label, itemName + "Label", labelText, new SDL_Rect{ settings.MENU_PADDING, itemY, -1, -1 }, inlineWithPrev));

		this->options.push_back(OptionItem(renderer, font, type, itemName, itemText, new SDL_Rect{ itemX + options.back().rect->w, itemY, settings.OPTIONS_WIDTH - (settings.MENU_PADDING * 2) - options.back().rect->w, -1 }, inlineWithPrev));
	}
	else {
		this->options.push_back(OptionItem(renderer, font, type, itemName, itemText, new SDL_Rect{ itemX, itemY, -1, -1 }, inlineWithPrev));
	}

	
}

#pragma once

// TODO: Fix this, it needs to work for resizing the window
static struct Settings {
public:
	int WINDOW_WIDTH;
	int WINDOW_HEIGHT;
	int OPTIONS_WIDTH;
	int SCREEN_WIDTH;
	const int TILE_SIZE = 16;
	const int MENU_PADDING = 20;
	const int MAX_OPTIONS_WIDTH = 500;
	const int MIN_WINDOW_WIDTH = 1200;
	const int MIN_WINDOW_HEIGHT = 700;

	Settings(int windowWidth, int windowHeight);
	void setScreenSizes(int windowWidth, int windowHeight);
} settings(1200, 700);
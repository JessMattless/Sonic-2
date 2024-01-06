#include "Settings.h"


Settings::Settings(int windowWidth, int windowHeight) {
	setScreenSizes(windowWidth, windowHeight);
}

void Settings::setScreenSizes(int windowWidth, int windowHeight) {
	WINDOW_HEIGHT = windowHeight;
	WINDOW_WIDTH = windowWidth;

	OPTIONS_WIDTH = (WINDOW_WIDTH / 3) < MAX_OPTIONS_WIDTH ? WINDOW_WIDTH / 3 : MAX_OPTIONS_WIDTH;
	SCREEN_WIDTH = WINDOW_WIDTH - OPTIONS_WIDTH;
}
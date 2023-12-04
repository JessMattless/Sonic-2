#pragma once
struct Tile
{
	bool foreground;
	bool flipH, flipV;
	int palette;
	int tileMapIndex;

	Tile(char tileInput);
};


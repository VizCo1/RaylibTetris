#pragma once
#include "raylib.h"
#include "Vec2.h"

namespace raycpp
{
	void DrawRectangle(Vec2<int> pos, Vec2<int> widthHeight, Color color);
	void DrawRectangleLinesEx(Vec2<int> pos, Vec2<int> widthHeight, int lineThick, Color color);
	void DrawText(const char* text, Vec2<int> pos, int fontSize, Color color);
}
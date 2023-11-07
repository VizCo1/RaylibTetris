#include "raylibCpp.h"
#include <cassert>

void raycpp::DrawRectangle(Vec2<int> pos, Vec2<int> widthHeight, Color color)
{
	assert(pos.GetX() >= 0 && pos.GetY() >= 0 &&
		pos.GetX() < GetScreenWidth() && pos.GetY() < GetScreenHeight()); // If assertion triggers : pos is out of bounds
	DrawRectangle(pos.GetX(), pos.GetY(), widthHeight.GetX(), widthHeight.GetY(), color);
}

void raycpp::DrawRectangleLinesEx(Vec2<int> pos, Vec2<int> widthHeight, int lineThick, Color color)
{
	assert(pos.GetX() >= 0 && pos.GetY() >= 0 &&
		pos.GetX() < GetScreenWidth() && pos.GetY() < GetScreenHeight()); // If assertion triggers : pos is out of bounds
	assert(lineThick > 0); // If assertion triggers : lineTichk less than 0
	DrawRectangleLinesEx({(float)pos.GetX(), (float)pos.GetY(), (float)widthHeight.GetX(), (float)widthHeight.GetY()}, (float)lineThick, color);
}

void raycpp::DrawText(const char* text, Vec2<int> pos, int fontSize, Color color)
{
	DrawText(text, pos.GetX(), pos.GetY(), fontSize, color);
}



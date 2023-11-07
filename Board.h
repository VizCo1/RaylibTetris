#pragma once

#include <vector>
#include "raylib.h"	
#include "Vec2.h"
#include "raylibCpp.h"
#include "invokable.hpp"
//#include "Game.h"

struct Game;

class Board {
private:
	class Cell {
	public:
		Cell();
		void SetColor(Color c_in);
		void Remove();
		bool Exists() const;
		Color GetColor() const;
	private:
		bool bExists;
		Color c;
	};

public:
	Board(Vec2<int> screenPos, Vec2<int> widthHeight, int cellSize_in, int padding, Game& game);
	~Board();
	void Draw() const;
	void DrawCell(Vec2<int> pos) const;
	void DrawCell(Vec2<int> pos, Color color) const;
	void DrawWiredCell(Vec2<int> pos, Color color) const;
	bool CellExists(Vec2<int> pos) const;
	int GetWidth() const;
	int GetHeight() const;

	void Reset();

	invokable< const std::vector<Vec2<int> >&, Color> OnTetrominoPlaced;

private:
	
	Game& game;

	std::vector<Cell> cells;	

	void DrawBorder() const;
	void SetCell(Vec2<int> pos, Color c);
	
	void PlaceTetromino(const std::vector<Vec2<int>>& positions, Color color);
	void CheckForCompletedLine();
	void MoveAllAboveLinesDown(int y);

	void InitEvents();
	void UnInitEvents();

	Vec2<int> screenPos;

	const int width;
	const int height;
	const int cellSize;
	const int padding;
};
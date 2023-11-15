#include "Board.h"
#include "Game.h"
#include <cassert>
#include "Tetromino.h"
#include <iostream>

Board::Cell::Cell()
	: bExists(false), c(WHITE)
{

}

void Board::Cell::SetColor(Color c_in)
{
	c = c_in;
	bExists = true;
}

void Board::Cell::Remove()
{
	bExists = false;
}

bool Board::Cell::Exists() const
{
	return bExists;
}

Color Board::Cell::GetColor() const
{
	return c;
}

Board::Board(Vec2<int> screenPos, Vec2<int> widthHeight, int cellSize_in, int padding, Game& game)
	:	screenPos(screenPos), 
		width(widthHeight.GetX()), 
		height(widthHeight.GetY()), 
		cellSize(cellSize_in), 
		padding(padding),
		game(game)
{
	assert(width > 0 && height > 0); // If assertion triggers : The width or height is smaller than 1
	assert(cellSize > 0); // If assertion triggers : The cellSize is smaller than 1
	cells.resize(width * height);

	InitEvents();
}

Board::~Board()
{
	UnInitEvents();
}

void Board::SetCell(Vec2<int> pos, Color c)
{
	assert(pos.GetX() >= 0 && pos.GetY() >= 0 && pos.GetX() < width && pos.GetY() < height); // If assertion triggers : x or y is out of bounds
	cells[pos.GetY() * width + pos.GetX()].SetColor(c);
}

void Board::DrawCell(Vec2<int> pos) const
{
	Color color = cells[pos.GetY() * width + pos.GetX()].GetColor();
	DrawCell(pos, color);
}

void Board::DrawCell(Vec2<int> pos, Color color) const
{
	assert(pos.GetX() >= 0 && pos.GetY() >= 0 && pos.GetX() < width && pos.GetY() < height); // If assertion triggers : x or y is out of bounds
	Vec2<int> topLeft = screenPos + padding + (pos * cellSize);
	raycpp::DrawRectangle(topLeft, Vec2<int>{ cellSize, cellSize } - padding, color);
}

void Board::DrawWiredCell(Vec2<int> pos, Color color) const
{
	assert(pos.GetX() >= 0 && pos.GetY() >= 0 && pos.GetX() < width && pos.GetY() < height); // If assertion triggers : x or y is out of bounds
	Vec2<int> topLeft = screenPos + padding + (pos * cellSize);
	raycpp::DrawRectangleLinesEx(topLeft, Vec2<int>{ cellSize, cellSize } - padding, 1, color);
}

void Board::DrawBorder() const
{
	raycpp::DrawRectangleLinesEx(screenPos - (cellSize / 2),
		Vec2<int>{width* cellSize, height* cellSize} + cellSize,
		cellSize / 2,
		WHITE);
}

void Board::Draw() const
{
	for (int iY = 0; iY < height; iY++) {
		for (int iX = 0; iX < width; iX++) {
			if (CellExists({ iX, iY })) {
				DrawCell({ iX, iY });
			}
		}
	}

	DrawBorder();
}

bool Board::CellExists(Vec2<int> pos) const
{
	return cells[pos.GetY() * width + pos.GetX()].Exists();
}

int Board::GetWidth() const
{
	return width;
}

int Board::GetHeight() const
{
	return height;
}

void Board::Reset()
{
	cells.clear();
	cells.resize(width * height);
}

void Board::PlaceTetromino(const std::vector<Vec2<int>>& positions, Color color) 
{
	for (Vec2<int> pos : positions) {
		SetCell(pos, color);
	}

	game.IncreaseScore(5, 1);

	CheckForCompletedLine();
}

void Board::CheckForCompletedLine()
{
	int numberOfLines = 0;
	for (int iY = 0; iY < height; iY++) {

		bool isLineCompleted = true;
		std::vector<Cell*> line;
		for (int iX = 0; iX < width; iX++) {
			if (!CellExists({ iX, iY })) {
				isLineCompleted = false;
				break;
			}
			line.push_back(&cells[iY * width + iX]);
		}

		if (isLineCompleted) {
			for (Cell* cell : line) {
				cell->Remove();
			}
			MoveAllAboveLinesDown(iY);
			numberOfLines++;
		}

		line.clear();
	}

	game.IncreaseScore(75, numberOfLines*2);
}

void Board::MoveAllAboveLinesDown(int y)
{
	// Get colors that will be moved downwards
	
	for (int iY = y; iY > 0; iY--) {
		for (int iX = 0; iX < width; iX++) {
			Cell& aboveCell = cells[(iY - 1) * width + iX];
			Cell& belowCell = cells[iY * width + iX];
			
			if (aboveCell.Exists()) {
				belowCell.SetColor(aboveCell.GetColor());
				aboveCell.Remove();
			}
		}
	}
}

void Board::InitEvents()
{
	auto lambda1 = [this](const std::vector<Vec2<int>>& positions, Color color) { this->PlaceTetromino(positions, color); };
	callback<const std::vector<Vec2<int>>&, Color> PlaceTetromino_OnTetrominoPlaced(lambda1);
	OnTetrominoPlaced.hook(PlaceTetromino_OnTetrominoPlaced);
}

void Board::UnInitEvents()
{
	OnTetrominoPlaced.empty();
}


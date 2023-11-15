#include "Tetromino.h"
#include <iostream>
#include<algorithm>

Tetromino::Tetromino(const bool* shape, int dimension, Color color, Board& board)
	: shape(shape),
	  dimension(dimension),
	  color(color),
	  board(board),
	  boardPos(board.GetWidth()/2 - dimension/2, 0),
	  currentRotation(Rotation::UP)
{

}

void Tetromino::TryRotateClockwise()
{
	Rotation desiredRotation = Rotation((int(currentRotation) + 1) % 4);

	if (CanRotate(desiredRotation) == Collision::NONE)
	{
		currentRotation = desiredRotation;
	}
}

void Tetromino::TryRotateCounterClockwise()
{
	Rotation desiredRotation;

	if (currentRotation == Rotation::UP)
	{
		desiredRotation = Rotation::LEFT;
	}
	else
	{
		desiredRotation = Rotation((int(currentRotation) - 1));
	}

	if (CanRotate(desiredRotation) == Collision::NONE)
	{
		currentRotation = desiredRotation;
	}
}

bool Tetromino::TryMoveDown()
{
	Collision coll = CanMoveDown();
	if (coll == Collision::NONE)
	{
		boardPos.SetY(boardPos.GetY() + 1);
		return true;
	}
	else if (coll == Collision::TETROMINO)
	{
		std::vector<Vec2<int>> positions = GetActivePositions();
		board.OnTetrominoPlaced.invoke(positions, color);
		return false;
	}

	return false;
}

void Tetromino::TryMoveRight()
{
	if (CanMoveRight() == Collision::NONE)
	{
		boardPos.SetX(boardPos.GetX() + 1);
	}
}

void Tetromino::TryMoveLeft()
{
	if (CanMoveLeft() == Collision::NONE)
	{
		boardPos.SetX(boardPos.GetX() - 1);
	}
}

Tetromino::Collision Tetromino::CanMoveDown() const
{
	for (int y = 0; y < dimension; y++) {
		for (int x = 0; x < dimension; x++) {
			bool cell = GetCellStatusFromRotation(currentRotation, x, y);
			if (cell)
			{
				Vec2<int> nextPos = { boardPos.GetX() + x, boardPos.GetY() + y + 1 };

				if (nextPos.GetY() >= board.GetHeight() || board.CellExists(nextPos))
				{
					return Collision::TETROMINO;
				}
			}
		}
	}

	return Collision::NONE;
}

Tetromino::Collision Tetromino::CanMoveRight() const
{
	for (int y = 0; y < dimension; y++) {
		for (int x = 0; x < dimension; x++) {
			bool cell = GetCellStatusFromRotation(currentRotation, x, y);
			if (cell)
			{
				Vec2<int> nextPos = { boardPos.GetX() + x + 1, boardPos.GetY() + y };
				if (nextPos.GetX() >= board.GetWidth())
				{
					return Collision::BORDER;
				}
				if (board.CellExists(nextPos))
				{
					return Collision::TETROMINO;
				}

			}
		}
	}

	return Collision::NONE;;
}

Tetromino::Collision Tetromino::CanMoveLeft() const
{
	for (int y = 0; y < dimension; y++) {
		for (int x = 0; x < dimension; x++) {
			bool cell = GetCellStatusFromRotation(currentRotation, x, y);
			if (cell)
			{
				Vec2<int> nextPos = { boardPos.GetX() + x - 1, boardPos.GetY() + y };
				if (nextPos.GetX() < 0)
				{
					return Collision::BORDER;
				}
				if (board.CellExists(nextPos))
				{
					return Collision::TETROMINO;
				}
			}
		}
	}

	return Collision::NONE;
}

Tetromino::Collision Tetromino::CanRotate(Rotation desiredRotation) const
{
	for (int y = 0; y < dimension; y++) {
		for (int x = 0; x < dimension; x++) {
			bool cell = GetCellStatusFromRotation(desiredRotation, x, y);
			if (cell)
			{
				int nextIndex = GetNextIndexFromRotation(currentRotation, x, y);
				//int actualIndex = GetNextIndexFromRotation(currentRotation, x, y);
				
				Vec2<int> nextPos = boardPos + GetNextPosWithNextIndex(nextIndex);
				//Vec2<int> actualPos = boardPos + GetNextPosWithNextIndex(actualIndex);
				if ( nextPos.GetX() < 0 || nextPos.GetX() >= board.GetWidth() || nextPos.GetY() < 0 || nextPos.GetY() > board.GetHeight() )
				{
					return Collision::BORDER;
				}
				else if (board.CellExists(nextPos))
				{
					return Collision::TETROMINO;
				}
			}
		}
	}

	return Collision::NONE;
}

bool Tetromino::GetCellStatusFromRotation(Rotation rotation, int x, int y) const
{
	bool cell = false;
	switch (currentRotation)
	{
	case Tetromino::Rotation::UP:
		cell = shape[y * dimension + x];
		break;
	case Tetromino::Rotation::RIGHT:
		cell = shape[dimension * (dimension - 1) - dimension * x + y];
		break;
	case Tetromino::Rotation::DOWN:
		cell = shape[(dimension * dimension - 1) - dimension * y - x];
		break;
	case Tetromino::Rotation::LEFT:
		cell = shape[dimension - 1 + dimension * x - y];
		break;
	}

	return cell;
}

int Tetromino::GetNextIndexFromRotation(Rotation rotation, int x, int y) const
{
	int nextIndex = 0;
	switch (rotation)
	{
	case Tetromino::Rotation::UP:
		nextIndex = y * dimension + x;
		break;
	case Tetromino::Rotation::RIGHT:
		nextIndex = dimension * (dimension - 1) - dimension * x + y;
		break;
	case Tetromino::Rotation::DOWN:
		nextIndex = (dimension * dimension - 1) - dimension * y - x;
		break;
	case Tetromino::Rotation::LEFT:
		nextIndex = dimension - 1 + dimension * x - y;
		break;
	}

	return nextIndex;
}

std::vector<Vec2<int>> Tetromino::GetActivePositions() const
{
	std::vector<Vec2<int>> positions;
	for (int y = 0; y < dimension; y++) {
		for (int x = 0; x < dimension; x++) {
			bool cell = GetCellStatusFromRotation(currentRotation, x, y);
			if (cell)
			{
				Vec2<int> activePos = { boardPos.GetX() + x, boardPos.GetY() + y };
				positions.push_back(activePos);
			}
		}
	}

	return positions;
}

Vec2<int> Tetromino::GetWiredBoardPos() const
{
	int lowerY = board.GetHeight() - 1;
	for (int i = boardPos.GetY(); i < board.GetHeight(); i++) {
		Vec2<int> pos(boardPos.GetX(), i);
		Collision coll = IsSpaceEmpty(pos);
		if (coll == Collision::NONE)
		{
			lowerY = i;
		}
		else {	
			return Vec2<int>(boardPos.GetX(), lowerY);			
		}
	}

	return boardPos;
}

Tetromino::Collision Tetromino::IsSpaceEmpty(Vec2<int> wiredBoardPos) const
{
	for (int y = 0; y < dimension; y++) {
		for (int x = 0; x < dimension; x++) {
			bool cell = GetCellStatusFromRotation(currentRotation, x, y);
			if (cell)
			{
				Vec2<int> nextPos = { wiredBoardPos.GetX() + x, wiredBoardPos.GetY() + y};
				if (nextPos.GetX() >= board.GetWidth() || nextPos.GetY() >= board.GetHeight()) {
					return Collision::BORDER;
				}
				else if (board.CellExists(nextPos))
				{
					return Collision::TETROMINO;
				}
			}
		}
	}

	return Collision::NONE;
}

void Tetromino::Draw() const
{
	for (int y = 0; y < dimension; y++) {
		for (int x = 0; x < dimension; x++) {
			bool cell = GetCellStatusFromRotation(currentRotation, x, y);
			if (cell)
			{
				Vec2<int> pos = boardPos + Vec2<int>{ x, y };
				board.DrawCell(pos, color);
			}
		}
	}

	for (int y = 0; y < dimension; y++) {
		for (int x = 0; x < dimension; x++) {
			bool cell = GetCellStatusFromRotation(currentRotation, x, y);
			if (cell)
			{
				Vec2<int> pos = GetWiredBoardPos() + Vec2<int>{x, y};
				pos.SetY(std::clamp(pos.GetY(), 0, board.GetHeight() - 1));
				board.DrawWiredCell(pos, color);
			}
		}
	}
}

Vec2<int> Tetromino::GetBoardPos() const
{
	return boardPos;
}

void Tetromino::SetBoardPos(Vec2<int> pos)
{
	boardPos = pos;
}

Color Tetromino::GetColor() const
{
	return color;
}

const bool* Tetromino::GetShape() const
{
	return shape;
}

const int Tetromino::GetDimension() const
{
	return dimension;
}

Vec2<int> Straight::GetNextPosWithNextIndex(int nextIndex) const
{
	Vec2<int> nextPos{};

	switch (nextIndex)
	{
	case 0:
		nextPos = { 0,0 };
		break;
	case 1:
		nextPos = { 1,0 };
		break;
	case 2:
		nextPos = { 2,0 };
		break;
	case 3:
		nextPos = { 3,0 };
		break;
	case 4:
		nextPos = { 0,1 };
		break;
	case 5:
		nextPos = { 1,1 };
		break;
	case 6:
		nextPos = { 2,1 };
		break;
	case 7:
		nextPos = { 3,1 };
		break;
	case 8:
		nextPos = { 0,2 };
		break;
	case 9:
		nextPos = { 1,2 };
		break;
	case 10:
		nextPos = { 2,2 };
		break;
	case 11:
		nextPos = { 3,2 };
		break;
	case 12:
		nextPos = { 0,3 };
		break;
	case 13:
		nextPos = { 1,3 };
		break;
	case 14:
		nextPos = { 2,3 };
		break;
	case 15:
		nextPos = { 3,3 };
		break;
	}

	return nextPos;
}

Vec2<int> Square::GetNextPosWithNextIndex(int nextIndex) const
{
	Vec2<int> nextPos{};

	switch (nextIndex)
	{
	case 0:
		nextPos = { 0,0 };
		break;
	case 1:
		nextPos = { 1,0 };
		break;
	case 2:
		nextPos = { 0,1 };
		break;
	case 3:
		nextPos = { 1,1 };
		break;
	}

	return nextPos;
}


Vec2<int> Tee::GetNextPosWithNextIndex(int nextIndex) const
{
	Vec2<int> nextPos{};

	switch (nextIndex)
	{
	case 0:
		nextPos = { 0,0 };
		break;
	case 1:
		nextPos = { 1,0 };
		break;
	case 2:
		nextPos = { 2,0 };
		break;
	case 3:
		nextPos = { 0,1 };
		break;
	case 4:
		nextPos = { 1,1 };
		break;
	case 5:
		nextPos = { 2,1 };
		break;
	case 6:
		nextPos = { 0,2 };
		break;
	case 7:
		nextPos = { 1,2 };
		break;
	case 8:
		nextPos = { 2,2 };
		break;
	}

	return nextPos;
}


Vec2<int> Jay::GetNextPosWithNextIndex(int nextIndex) const
{
	Vec2<int> nextPos{};

	switch (nextIndex)
	{
	case 0:
		nextPos = { 0,0 };
		break;
	case 1:
		nextPos = { 1,0 };
		break;
	case 2:
		nextPos = { 2,0 };
		break;
	case 3:
		nextPos = { 0,1 };
		break;
	case 4:
		nextPos = { 1,1 };
		break;
	case 5:
		nextPos = { 2,1 };
		break;
	case 6:
		nextPos = { 0,2 };
		break;
	case 7:
		nextPos = { 1,2 };
		break;
	case 8:
		nextPos = { 2,2 };
		break;
	}

	return nextPos;
}

Vec2<int> El::GetNextPosWithNextIndex(int nextIndex) const
{
	Vec2<int> nextPos{};

	switch (nextIndex)
	{
	case 0:
		nextPos = { 0,0 };
		break;
	case 1:
		nextPos = { 1,0 };
		break;
	case 2:
		nextPos = { 2,0 };
		break;
	case 3:
		nextPos = { 0,1 };
		break;
	case 4:
		nextPos = { 1,1 };
		break;
	case 5:
		nextPos = { 2,1 };
		break;
	case 6:
		nextPos = { 0,2 };
		break;
	case 7:
		nextPos = { 1,2 };
		break;
	case 8:
		nextPos = { 2,2 };
		break;
	}

	return nextPos;
}

Vec2<int> SkewS::GetNextPosWithNextIndex(int nextIndex) const
{
	Vec2<int> nextPos{};

	switch (nextIndex)
	{
	case 0:
		nextPos = { 0,0 };
		break;
	case 1:
		nextPos = { 1,0 };
		break;
	case 2:
		nextPos = { 2,0 };
		break;
	case 3:
		nextPos = { 0,1 };
		break;
	case 4:
		nextPos = { 1,1 };
		break;
	case 5:
		nextPos = { 2,1 };
		break;
	case 6:
		nextPos = { 0,2 };
		break;
	case 7:
		nextPos = { 1,2 };
		break;
	case 8:
		nextPos = { 2,2 };
		break;
	}

	return nextPos;
}

Vec2<int> SkewZ::GetNextPosWithNextIndex(int nextIndex) const
{
	Vec2<int> nextPos{};

	switch (nextIndex)
	{
	case 0:
		nextPos = { 0,0 };
		break;
	case 1:
		nextPos = { 1,0 };
		break;
	case 2:
		nextPos = { 2,0 };
		break;
	case 3:
		nextPos = { 0,1 };
		break;
	case 4:
		nextPos = { 1,1 };
		break;
	case 5:
		nextPos = { 2,1 };
		break;
	case 6:
		nextPos = { 0,2 };
		break;
	case 7:
		nextPos = { 1,2 };
		break;
	case 8:
		nextPos = { 2,2 };
		break;
	}

	return nextPos;
}

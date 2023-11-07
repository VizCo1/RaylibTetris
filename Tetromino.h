#pragma once
#include "Vec2.h"
#include "raylibCpp.h"
#include "Board.h"
class Tetromino {
public:
	enum class Rotation {
		UP,
		RIGHT,
		DOWN,
		LEFT
	};

	enum class Collision {
		NONE,
		BORDER,
		TETROMINO
	};

public:
	Tetromino(const bool* shape, int dimension, Color color, Board& board);

	void TryRotateClockwise();
	void TryRotateCounterClockwise();
	bool TryMoveDown();
	void TryMoveRight();
	void TryMoveLeft();

	void Draw() const;

	Vec2<int> GetBoardPos() const;
	void SetBoardPos(Vec2<int> pos);

	Color GetColor() const;
	const bool* GetShape() const;
	const int GetDimension() const;

private:
	Vec2<int> boardPos;
	const bool* shape;
	Rotation currentRotation;
	const int dimension;
	const Color color;
	Board& board;

	Collision CanMoveDown() const;
	Collision CanMoveRight() const;
	Collision CanMoveLeft() const;
	Collision CanRotate(Rotation desiredRotation) const;
	Vec2<int> GetWiredBoardPos() const;
	Collision IsSpaceEmpty(Vec2<int> pos) const;
	bool GetCellStatusFromRotation(Rotation rotation, int x, int y) const;
	int GetNextIndexFromRotation(Rotation rotation, int x, int y) const;
	std::vector<Vec2<int>> GetActivePositions() const;

	virtual Vec2<int> GetNextPosWithNextIndex(int nextIndex) const = 0;
};

class Straight : public Tetromino {
public:
	Straight(Board& board)
		: Tetromino(shape, dimension, color, board)
	{
		static_assert(sizeof(shape) / sizeof(shape[0]) == dimension * dimension);
	}

private:
	static constexpr bool shape[] = {0,0,0,0,
									 1,1,1,1,
									 0,0,0,0,
									 0,0,0,0};

	static constexpr int dimension = 4;
	static constexpr Color color = SKYBLUE;

	virtual Vec2<int> GetNextPosWithNextIndex(int nextIndex) const override;
};

class Square : public Tetromino {
public:
	Square(Board& board)
		: Tetromino(shape, dimension, color, board)
	{
		static_assert(sizeof(shape) / sizeof(shape[0]) == dimension * dimension);
	}

private:
	static constexpr bool shape[] = {1,1,
									 1,1};

	static constexpr int dimension = 2;
	static constexpr Color color = YELLOW;

	virtual Vec2<int> GetNextPosWithNextIndex(int nextIndex) const override;
};

class Tee : public Tetromino {
public:
	Tee(Board& board)
		: Tetromino(shape, dimension, color, board)
	{
		static_assert(sizeof(shape) / sizeof(shape[0]) == dimension * dimension);
	}

private:
	static constexpr bool shape[] = {0,1,0,
									 1,1,1,
									 0,0,0};

	static constexpr int dimension = 3;
	static constexpr Color color = MAGENTA;

	virtual Vec2<int> GetNextPosWithNextIndex(int nextIndex) const override;
};

class Jay : public Tetromino {
public:
	Jay(Board& board)
		: Tetromino(shape, dimension, color, board)
	{
		static_assert(sizeof(shape) / sizeof(shape[0]) == dimension * dimension);
	}

private:
	static constexpr bool shape[] = {1,0,0,
									 1,1,1,
									 0,0,0};

	static constexpr int dimension = 3;
	static constexpr Color color = DARKBLUE;

	virtual Vec2<int> GetNextPosWithNextIndex(int nextIndex) const override;
};

class El : public Tetromino {
public:
	El(Board& board)
		: Tetromino(shape, dimension, color, board)
	{
		static_assert(sizeof(shape) / sizeof(shape[0]) == dimension * dimension);
	}

private:
	static constexpr bool shape[] = {0,0,1,
									 1,1,1,
									 0,0,0};

	static constexpr int dimension = 3;
	static constexpr Color color = ORANGE;

	virtual Vec2<int> GetNextPosWithNextIndex(int nextIndex) const override;
};

class SkewS : public Tetromino {
public:
	SkewS(Board& board)
		: Tetromino(shape, dimension, color, board)
	{
		static_assert(sizeof(shape) / sizeof(shape[0]) == dimension * dimension);
	}

private:
	static constexpr bool shape[] = {0,1,1,
									 1,1,0,
									 0,0,0};

	static constexpr int dimension = 3;
	static constexpr Color color = GREEN;

	virtual Vec2<int> GetNextPosWithNextIndex(int nextIndex) const override;
};

class SkewZ : public Tetromino {
public:
	SkewZ(Board& board)
		: Tetromino(shape, dimension, color, board)
	{
		static_assert(sizeof(shape) / sizeof(shape[0]) == dimension * dimension);
	}

private:
	static constexpr bool shape[] = {1,1,0,
									 0,1,1,
									 0,0,0};

	static constexpr int dimension = 3;
	static constexpr Color color = RED;

	virtual Vec2<int> GetNextPosWithNextIndex(int nextIndex) const override;
};
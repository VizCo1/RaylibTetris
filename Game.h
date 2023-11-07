#pragma once
#include <string>
#include "Board.h"
#include "Tetromino.h"
#include "UserInterface.h"
class Game {
public:

	Game(int width, int height, int fps, std::string title);
	~Game() noexcept;

	Game(const Game& other) = delete;
	Game& operator=(const Game& other) = delete;

	bool GameShouldClose() const;

	void Tick();

	Tetromino* GetNextTetromino() const;

	void IncreaseScore(int base, int times);

	int GetScore() const;
	bool GetStop() const;

private:

	void Update();
	void Draw();

	void Reset();
	void FollowingTetromino();
	void CreateRandomTetromino(Tetromino*& tetrominoPointer);

	bool stop = false;
	int score;

	float timer{0};
	float timerLife{1.0f};

	Board board;
	UserInterface userInterface;

	Tetromino* currentTetromino;
	Tetromino* nextTetromino;
};
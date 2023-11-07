#include <cassert>
#include "Game.h"
#include "raylib.h"
#include "Settings.h"
#include <iostream>

Game::Game(int width, int height, int fps, std::string title)
	: board(settings::boardPosition, 
			settings::boardWidthHeight, 
			settings::cellSize, 
			settings::padding,
			*this),
	  userInterface(*this, board)
{
	assert(!GetWindowHandle()); // If assertion triggers : Window is already opened

	CreateRandomTetromino(currentTetromino);
	CreateRandomTetromino(nextTetromino);

	SetTargetFPS(fps);
	InitWindow(width, height, title.c_str());
}

Game::~Game() noexcept
{
	assert(GetWindowHandle()); // If assertion triggers : Window is already closed
	delete currentTetromino;
	delete nextTetromino;
	CloseWindow();
}

bool Game::GameShouldClose() const
{
	return WindowShouldClose();
}

void Game::Tick()
{
	BeginDrawing();
	Update();
	Draw();
	EndDrawing();
}

Tetromino* Game::GetNextTetromino() const
{
	return nextTetromino;
}

void Game::IncreaseScore(int base, int times)
{
	score += base * times;
}

int Game::GetScore() const
{
	return score;
}

bool Game::GetStop() const
{
	return stop;
}

void Game::Update()
{
	if (stop) {

		if (IsKeyPressed(KEY_SPACE)) {
			Reset();
		}

		return;
	}

	timer += GetFrameTime();

	if (IsKeyPressed(KEY_E))
	{
		currentTetromino->TryRotateClockwise();
	}
	else if (IsKeyPressed(KEY_Q))
	{
		currentTetromino->TryRotateCounterClockwise();
	}

	if (IsKeyPressed(KEY_A))
	{
		currentTetromino->TryMoveLeft();
	}
	else if (IsKeyPressed(KEY_D))
	{
		currentTetromino->TryMoveRight();
	}
	else if (IsKeyDown(KEY_S))
	{
		if (!currentTetromino->TryMoveDown()) {
			FollowingTetromino();
			// Check lose condition!
			if (!currentTetromino->TryMoveDown()) {
				stop = true;
				delete currentTetromino;
			}
		}
	}

	if (timer >= timerLife)
	{
		timer = 0;
		if (!currentTetromino->TryMoveDown()) {
			FollowingTetromino();
			// Check lose condition!
			if (!currentTetromino->TryMoveDown()) {
				stop = true;
				delete currentTetromino;
			}
		}
	}
}

void Game::Draw()
{
	ClearBackground(BLACK);
	if (!stop) {
		board.Draw();
		currentTetromino->Draw();
	}
	userInterface.Draw();
}

void Game::Reset()
{
	score = 0;
	CreateRandomTetromino(currentTetromino);
	CreateRandomTetromino(nextTetromino);
	board.Reset();

	stop = false;
}

void Game::FollowingTetromino()
{
	if (currentTetromino != nullptr) {
		delete currentTetromino;
	}

	timer = 0;
	currentTetromino = nextTetromino;
	CreateRandomTetromino(nextTetromino);
}

void Game::CreateRandomTetromino(Tetromino*& tetrominoPointer)
{
	int x = GetRandomValue(0, 6);

	switch (x)
	{
	case 0:
		tetrominoPointer = new Straight(board);
		break;
	case 1:
		tetrominoPointer = new Square(board);
		break;
	case 2:
		tetrominoPointer = new Tee(board);
		break;
	case 3:
		tetrominoPointer = new Jay(board);
		break;
	case 4:
		tetrominoPointer = new El(board);
		break;
	case 5:
		tetrominoPointer = new SkewS(board);
		break;
	case 6:
		tetrominoPointer = new SkewZ(board);
		break;
	}
}

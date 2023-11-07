#pragma once
#include "Board.h"
#include "Tetromino.h"
#include <string>

struct Game;

class UserInterface
{
public:
	UserInterface() = default;
	UserInterface(Game& game, Board& board);

	void Draw() const;

private:

	const Game& game;
	const Board& board;

	const std::string drawTetrominoText = "Next tetromino :";
	const std::string drawScoreText = "Score :";

	void DrawScore() const;
	void DrawDisplayedTetromino() const;
	void DrawGameOver() const;
	void DrawTetrominoShape() const;

};
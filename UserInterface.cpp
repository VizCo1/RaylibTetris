#include "UserInterface.h"
#include "Game.h"
#include "Settings.h"
#include "raylibCpp.h"

UserInterface::UserInterface(Game& game, Board& board)
	:	board(board),
		game(game)
{
}

void UserInterface::Draw() const
{
	if (game.GetStop()) {
		DrawGameOver();
	}
	else {
		DrawDisplayedTetromino();
		DrawScore();
	}
}

void UserInterface::DrawScore() const
{
	Vec2<int> screenPosScoreText(settings::screenWidth * 0.65, settings::screenHeight * 0.45);
	Vec2<int> screenPosScore(settings::screenWidth * 0.65, settings::screenHeight * 0.55);

	raycpp::DrawText(drawScoreText.c_str(), screenPosScoreText, settings::fontSize, RAYWHITE);
	raycpp::DrawText(std::to_string(game.GetScore()).c_str(), screenPosScore, settings::fontSize * 1.5, RAYWHITE);
}

void UserInterface::DrawDisplayedTetromino() const
{
	Vec2<int> screenPos(settings::screenWidth * 0.65, settings::screenHeight * 0.15);

	raycpp::DrawText(drawTetrominoText.c_str(), screenPos, settings::fontSize, RAYWHITE);
	DrawTetrominoShape();
}

void UserInterface::DrawGameOver() const
{
	std::string s = "GAME OVER!";
	Vec2<int> screenPos(settings::screenWidth * 0.25, settings::screenHeight * 0.4);
	Vec2<int> screenPos2(settings::screenWidth * 0.25, settings::screenHeight * 0.55);
	Vec2<int> screenPos3(settings::screenWidth * 0.25, settings::screenHeight * 0.70);
	raycpp::DrawText(s.c_str(), screenPos, settings::fontSize * 2, RAYWHITE);
	raycpp::DrawText(("POINTS: " + std::to_string(game.GetScore())).c_str(), screenPos2, settings::fontSize * 1.5, RAYWHITE);
	raycpp::DrawText("PRESS SPACE TO RESTART THE GAME", screenPos3, settings::fontSize, RAYWHITE);
}

void UserInterface::DrawTetrominoShape() const
{
	Tetromino* tetromino = game.GetNextTetromino();

	Vec2<int> screenPos(settings::screenWidth * 0.75, settings::screenHeight * 0.25);
	int padding = settings::padding;
	int cellSize = settings::cellSize * 1.5;

	for (int iY = 0; iY < tetromino->GetDimension(); iY++) {
		for (int iX = 0; iX < tetromino->GetDimension(); iX++) {
			if (tetromino->GetShape()[iY * tetromino->GetDimension() + iX]) {
				Vec2<int> topLeft = screenPos + padding + (Vec2<int>{iX, iY} * cellSize);
				raycpp::DrawRectangle(topLeft, Vec2{ cellSize, cellSize } - padding, tetromino->GetColor());
			}
		}
	}
}

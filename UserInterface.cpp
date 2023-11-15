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
	std::string gameOverText = "GAME OVER!";
	std::string pointsText = "POINTS: ";
	pointsText += std::to_string(game.GetScore());
	std::string restartText = "PRESS SPACE TO RESTART THE GAME";
	Vec2<int> screenPos(settings::screenWidth * 0.05, settings::screenHeight * 0.15);
	Vec2<int> screenPos2(settings::screenWidth * 0.05, settings::screenHeight * 0.35);
	Vec2<int> screenPos3(settings::screenWidth * 0.05, settings::screenHeight * 0.50);
	raycpp::DrawText(gameOverText.c_str(), screenPos, settings::fontSize * 2, RAYWHITE);
	raycpp::DrawText(pointsText.c_str(), screenPos2, settings::fontSize * 1.5, RAYWHITE);
	raycpp::DrawText(restartText.c_str(), screenPos3, settings::fontSize, RAYWHITE);
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
				raycpp::DrawRectangle(topLeft, Vec2<int>{ cellSize, cellSize } - padding, tetromino->GetColor());
			}
		}
	}
}

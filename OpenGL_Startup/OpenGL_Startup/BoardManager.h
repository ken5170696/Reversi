#pragma once

#include <iostream>
#include <vector>

#include "glad/glad.h"
#include "GLFW/glfw3.h"

#include "Renderer.h"
#include "Board.h"
#include "Chess.h"
#include "Camera.h"

#define BOARD_SIZE_X 8 
#define BOARD_SIZE_Y 8 

/*
* -0.88,-0.88
* +0.25
*	 A B C D E F G F
	 _ _ _ _ _ _ _ _
1	|_|_|_|_|_|_|_|_|
2	|_|_|_|_|_|_|_|_|
3	|_|_|_|_|_|_|_|_|
4	|_|_|_|_|_|_|_|_|
5	|_|_|_|_|_|_|_|_|
6	|_|_|_|_|_|_|_|_|
7	|_|_|_|_|_|_|_|_|
8	|_|_|_|_|_|_|_|_|
*/
const int posScoreTable[8][8] = {
	{9,1,8,2,2,8,1,9},
	{1,1,8,3,3,8,1,1},
	{8,8,8,6,6,8,8,8},
	{2,3,6,0,0,6,3,2},
	{2,3,6,0,0,6,3,2},
	{8,8,8,6,6,8,8,8},
	{1,1,8,7,7,8,1,1},
	{9,1,8,3,3,8,1,9}
};
const int DirTable[8][2] = {
	{-1, -1 },
	{ 0, -1 },
	{ 1, -1 },
	{ -1, 0 },
	//{ 0,  0 },
	{ 1,  0 },
	{ -1, 1 },
	{ 0,  1 },
	{ 1,  1 }
};

class BoardManager
{
public:
	enum class GameStatus
	{
		Preparing,

		SetBlackHint,
		WaitForBlack, // wait status
		BlackSeletced,

		SetWhiteHint,
		WaitForWhite, // wait status
		WhiteSeletced,

		End
	};
	enum class MouseState
	{
		Idle,
		Clicked
	};

	struct updateContent {
		float deltaTime;
		Camera* camera;
	};

	BoardManager(Renderer* rend);
	~BoardManager();

	//void processInput(GLFWwindow*);
	void update(updateContent content);
	void draw(Shader* shader);
	void processInput(GLFWwindow* window);
	glm::vec3 boardIdx2WorldPos(const int& x, const int& y);

	void addChess(const int& x, const int& y, const Chess::Color& color);
	void removeChess(const int& x, const int& y);
	void setChessStatus(const int& x, const int& y, unsigned int status, bool flag);
	void clearChess();
	void clearHintChess();

	bool isFull();

	Chess::Color setWinner();

	void findAvailablePos(const Chess::Color& color);
	void checkFliping(const int& x, const int& y);

	void setGameStart();

	GameStatus getGameStatus();

	Chess* getChess(const int& x, const int& y);

	Chess::Color getWinner();

	bool WhiteBot;
	bool BlackBot;
	Chess::Color winnwer;
	float aiSpeed;

	void botPlay(glm::ivec2& selectedId);
	int posWeight(int x);
	int posWeight(int x, int y);

private:
	MouseState _rightMouseState;
	vector<vector<Chess*>> _board;
	vector<glm::ivec2> _availPosRecord;
	GameStatus _gameStatus;

	shared_ptr<Model> blackModel;
	shared_ptr<Model> whiteModel;
	Chess* newChess(const Chess::Color& color);

	
	bool gameStart;

	bool whiteOutOfMove;
	bool blackOutOfMove;

};


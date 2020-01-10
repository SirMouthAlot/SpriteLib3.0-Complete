#include "Game.h"

#undef main
int main()
{
	//Create the Game
	Game theGame;

	//Initialize the game
	theGame.InitGame();

	//Runs the game until it returns :)
	return theGame.Run();
}
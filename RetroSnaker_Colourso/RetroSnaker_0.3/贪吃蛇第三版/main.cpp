#include "Snake.h"
#include "Game.h"

int main()
{
	Game game;
	game.Init();
	game.Run();
	game.Close();

	return 0;
}
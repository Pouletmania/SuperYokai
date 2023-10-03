#include "src/Game.h"

int main()
{
	Game game;

	game.launch();
	game.run();
	game.run(); // TEST : to make sure unloading and loading work correctly

	return 0;
}

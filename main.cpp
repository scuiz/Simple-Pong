#include<iostream>

#include "game.h"

int main() {

	//Init srand
	std::srand(static_cast<unsigned>(time(NULL)));
	
	//Init game
	game game;

	while (game.isRunning()) {
	
		game.update();

		game.render();

	}

	return 0;
}
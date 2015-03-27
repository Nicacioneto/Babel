#include "../include/game.h"
#include <iostream>

using namespace std;

Game::Game() {}

Game::~Game() {}

void Game::run() {
	cout << "Game loop" << endl;

	bool done = false;

	while (not done) {
		process_timestep();
		process_input();
		run_ai();
		collision_step();
		done = update_objects();
		render();
	}
}

void Game::process_timestep() {}

void Game::process_input() {}

void Game::run_ai() {}

void Game::collision_step() {}

bool Game::update_objects() {
	return false;
}

void Game::render() {}

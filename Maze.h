#ifndef MAZE_H
#define MAZE_H

//#include "Player.h"
#include "Board.h"
#include <vector>


//helpers that return random names and positions (respectively)
std::vector<std::string> ChooseRandomNames(const int num_enemies);
std::vector<Position> ChooseEnemyPositions(const int num_enemies, const Board &b);

class Maze {
public:
//	
	Maze(char c_size, char c_diff); // constructor
//
//	// initialize a new game, given one human player and 
//	// a number of enemies to generate
	void NewGame(Player *human);
//
//	// have the given Player take their turn
	void TakeTurn(Player *p);
//
//	// Get the next player in turn order
	Player * GetNextPlayer();

	//kill all players
	void KillAll();

	//have an enemy take their turn 
	//randomly move them to a valid pos
    void DoEnemyTurn(Player *p);

	//determine what happens after player inputs where they want to move
	void DoHumanTurnLogic(Player *p, Position &pos);

//	// return true iff the human made it to the exit 
//	// or the enemies ate all the humans
	bool IsGameOver();
//
//	// string info about the game's conditions after it is over (names and points)
	std::string GenerateReport();
//	//helper method to return how many enemies we should have based off both difficulty and size
	int CalculateEnemies();

private:
	Board *board_; 
	std::vector<Player*> players_;

	int current_player_idx_;
	
	char map_size_;
	char difficulty_;
//
};  // class Maze

#endif  // MAZE_H

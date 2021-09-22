#ifndef MAZE_H
#define MAZE_H

//#include "Player.h"
#include "Board.h"
#include <vector>


//helper for Maze to choose random names for enemies 
std::vector<std::string> ChooseRandomNames(const int num_enemies);
std::vector<Position> ChooseEnemyPositions(const int num_enemies, const Board &b);

class Maze {
public:
//	// TODO: implement these functions
	Maze(); // constructor
//
//	// initialize a new game, given one human player and 
//	// a number of enemies to generate
	void NewGame(Player *human, const int enemies);
//
//	// have the given Player take their turn
	void TakeTurn(Player *p);
//
//	// Get the next player in turn order
	Player * GetNextPlayer();
	//kill all players
	void KillAll();
    void DoEnemyTurn(Player *p);
	void DoHumanTurnLogic(Player *p, Position &pos);
//	// return true iff the human made it to the exit 
//	// or the enemies ate all the humans
	bool IsGameOver();
//
//	// You probably want to implement these functions as well
//	// string info about the game's conditions after it is over
	std::string GenerateReport();
//	friend std::ostream& operator<<(std::ostream& os, const Maze &m);
//
private:
	Board *board_; // HINT: when you instantiate your board_, use the new Board() syntax
	std::vector<Player*> players_;
	int turn_count_;
	int current_player_idx_;
//
//	// you may add more fields, as needed
//
};  // class Maze

#endif  // MAZE_H

#ifndef BOARD_H
#define BOARD_H

#include "Player.h"
#include <string>
#include <vector>

// you may change this enum as you need
enum class SquareType { Wall, Exit, Empty, Human, Enemy, Treasure };

// this function should return a string representation of a given SquareType
// for example an ascii letter or an emoji
std::string SquareTypeStringify(const SquareType sq);
//helper for constructing a Board
//use ran nums to return a random squaretype
SquareType ChooseRandomSquareType();

class Board {
public:
	//default constructor
	Board(char c_size, char c_diff);
	
	// already implemented in line
	int get_rows() const {return rows_; }  // you should be able to change the size of your 
	int get_cols() const {return cols_; }  // board by changing these numbers and the numbers in the arr_ field

	std::vector<std::vector<SquareType>>& accBoard() {return arr_;}
//	// return SquareValue of a given Position
	SquareType get_square_value(Position pos) {return arr_[pos.row][pos.col];}
//
//	// set the value at a Position to the given SquareType
	void SetSquareValue(Position pos, SquareType value) {arr_[pos.row][pos.col] = value;}
//
	std::vector<Position> ChooseEnemyPositions(int num_enemies);
//	// get the valid Positions that a Player could move to
	std::vector<Position> GetMoves(Player *p);
//
//	// Move a player to a new position on the board. Return
//	// true if they moved successfully, false otherwise.
	bool MovePlayer(Player *p, Position pos);
//
//	// Get the square type of the exit square
	SquareType GetExitOccupant() const {return arr_[rows_-2][cols_-2];}
	//helper for is solvable
	bool IsValid(const Position &pos) {return (arr_[pos.row][pos.col] != SquareType::Wall);};
//  //  Return wether the current Board is solvable
	bool IsSolvable();
//	// You probably want to implement this
	friend std::ostream& operator<<(std::ostream& os, const Board &b);

private:
//	SquareType arr_[8][8]; //the map
	std::vector<std::vector<SquareType>> arr_;

	int rows_; 
	int cols_;
	
};  // class Board

#endif
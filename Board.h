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
	Board();
	
	// already implemented in line
	int get_rows() const {return 6; }  // you should be able to change the size of your 
	int get_cols() const {return 6; }  // board by changing these numbers and the numbers in the arr_ field

//	// return SquareValue of a given Position
	SquareType get_square_value(Position pos) const {return arr_[pos.row][pos.col];}
//
//	// set the value at a Position to the given SquareType
	void SetSquareValue(Position pos, SquareType value) {arr_[pos.row][pos.col] = value;}
//
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
	bool IsValid(const Position pos) {return arr_[pos.row][pos.col] == SquareType::Empty;} 
//  //  Return wether the current Board is solvable
	bool IsSolvable();
//	// You probably want to implement this
	friend std::ostream& operator<<(std::ostream& os, const Board &b);

private:
	SquareType arr_[6][6];
	
	int rows_ = 6; // might be convenient but not necessary
	int cols_ = 6;
	
	// you may add more fields, as needed
};  // class Board

#endif
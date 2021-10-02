#ifndef BOARD_H
#define BOARD_H

#include "Player.h"
#include <string>
#include <vector>

//enum to represent different types of Squares that can be on the Board
enum class SquareType {Wall, Exit, Empty, Human, Enemy, Gold, Silver};

//return a string that represents a SquareType
std::string SquareTypeStringify(const SquareType sq);

//helper for constructing a Board
//use ran nums to return a random squaretype
SquareType ChooseRandomSquareType();

class Board {
public:
	//default constructor
	Board(char c_size, char c_diff);
	
	//getters for row and col
	int get_rows() const {return rows_;}
	int get_cols() const {return cols_;} 

//	// return SquareValue of a given Position
	SquareType get_square_value(Position pos) {return arr_[pos.row][pos.col];}
//
//	// set the value at a Position to the given SquareType
	void SetSquareValue(Position pos, SquareType value) {arr_[pos.row][pos.col] = value;}

//  // helper method to choose positions for the enemies
	std::vector<Position> ChooseEnemyPositions(int num_enemies);

//	// get the valid Positions that a Player could move to
	std::vector<Position> GetMoves(Player *p);

//	// Move a player to a new position on the board. Return
//	// true if they moved successfully, false otherwise.
	bool MovePlayer(Player *p, Position pos);

//	// Get the square type of the exit square
	SquareType GetExitOccupant() const {return arr_[rows_-2][cols_-2];}

    //helper method to check if a Tile could be traversed
	bool IsValid(const Position &pos) {return (arr_[pos.row][pos.col] != SquareType::Wall);};
    
	//  Return wether the current Board is solvable
	bool IsSolvable();
	
	// Return wether a type exists on the Board
	bool TypeExists(SquareType type);
	
	// Find out wether a path from a player to a squaretype is possible
	// Return the position of the square if so, else return dummy value to be handled
	Position PathToTypeExists(Player *p, SquareType type);
	
	// Overloaded helper method to backtrace FindPath and create the full path
	void FindPath(Position* pos, std::vector<Position*> &path);
	
	// Method to find a path from a player to a position
	// Return the first position on the path
	Position FindPath(Player *p, Position pos);

//	//overload << operator to print out the Board when necessary
	friend std::ostream& operator<<(std::ostream& os, const Board &b);

private:
	//the map
	std::vector<std::vector<SquareType>> arr_;
	//num rows and num cols
	int rows_; 
	int cols_;
};  // class Board

#endif
#ifndef PLAYER_H
#define PLAYER_H

#include <string>

struct Position {
	//coordinates to determine position in Board
	int row;
	int col;
	//parent node to help with BFS traversel
	Position* parent;
	//normal constructor
	Position(int new_row, int new_col) {
		row = new_row;
		col = new_col;
		parent = nullptr;
	}
	//constructor method to use during BFS
	Position(int new_row, int new_col, Position* new_parent) {
		row = new_row;
		col = new_col;
		parent = new_parent;
	}
	// == overload 
	bool operator==(const Position &other) {
		return row == other.row && col == other.col;
	}
	// < overload 
	bool operator<(const Position& pos) const {
		return (row < pos.row || (row == pos.row && col < pos.col));
	}
};

class Player {
public:
	// paramterized constructor using initialization list
	Player(const std::string name, const std::string npc_type, const Position pos, const bool is_human) : name_(name), npc_type_(npc_type), pos_(pos), is_human_(is_human) {} 
	
	// getter functions
	std::string get_name() const {return name_; }  // inline member function
	std::string get_npc_type() const {return npc_type_; }  // inline member function
	int get_points() const {return points_; }  // inline member function
	Position get_position() const {return pos_; }  // inline member function
	bool is_human() const {return is_human_; }  // inline member function

	//method to 'kill' the player (make them dead)
	void Kill() {is_human_ = false;}

	//set new amount of points for this player
	void ChangePoints(const int x) {points_ = x;}

//	// set a new position for this player
	void SetPosition(Position pos) {pos_ = pos;}

//	// ToRelativePosition is a function we used to translate positions
//	// into directions relative to the player (up, down, etc)
	std::string ToRelativePosition(Position other);
//	// Convert this player to a string representation of their name and points
	std::string Stringify() {return name_ + std::to_string(points_);}

private:
	std::string name_;
	std::string npc_type_;
	Position pos_;
	bool is_human_;
	int points_ = 0;

}; // class Player

#endif  // PLAYER_H

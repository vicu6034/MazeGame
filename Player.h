#ifndef PLAYER_H
#define PLAYER_H

#include <string>

struct Position {
	int row = 1;
	int col = 1;

	// == overload 
	bool operator==(const Position &other) {
		return row == other.row && col == other.col;
	}
};

class Player {
public:
	// paramterized constructor
	Player(const std::string name, const bool is_human) : name_(name), is_human_(is_human) {} 
	
	// These are already implemented for you (getters)
	std::string get_name() const {return name_; }  // inline member function
	int get_points() const {return points_; }  // inline member function
	Position get_position() const {return pos_; }  // inline member function
	bool is_human() const {return is_human_; }  // inline member function
	//'kill'  all players (ensure humans dead)
	void Kill() {is_human_ = false;}
	//set new amount of points
	void ChangePoints(const int x) {points_ = x;}
//	// set a new position for this player
	void SetPosition(Position pos) {pos_ = pos;}
//	// ToRelativePosition is a function we used to translate positions
//	// into directions relative to the player (up, down, etc)
	std::string ToRelativePosition(Position other);
//	// Convert this player to a string representation of their name and points
	std::string Stringify() {return name_ + std::to_string(points_);}
//
//	// You may add other functions as needed/wanted

private:
	std::string name_;
	int points_ = 0;
	Position pos_;
	bool is_human_;

	// You may add other fields as needed

}; // class Player

#endif  // PLAYER_H

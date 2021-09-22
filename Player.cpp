#include "Player.h"
/**
  Method to translate a position into a string 
  The string should give what direction the position
  is in relative to the Player (this)
  @param Position to check the direction is in
  @return string that represents the direction of the Position
*/
std::string Player::ToRelativePosition(Position other) {
    if ((pos_.row == other.row+1) && (pos_.col == other.col)) {return "NORTH";}
    else if ((pos_.row == other.row) && (pos_.col == other.col-1)) {return "EAST";}
    else if ((pos_.row == other.row-1) && (pos_.col == other.col)) {return "SOUTH";}
    else if ((pos_.row == other.row) && (pos_.col == other.col+1)) {return "WEST";}
    else {return "";}
}
#include "Board.h"

#include <queue>
/**
  Helper function to transform a SquareType to a string
  @param SquareType to be transformed
  @return string to represent the SquareType
*/
std::string SquareTypeStringify(const SquareType sq) {
	switch(sq) {
		case SquareType::Wall:
			return "W";
		case SquareType::Exit:
			return "E";
		case SquareType::Empty:
			return " ";
		case SquareType::Human:
			return "H";
		case SquareType::Enemy:
			return "X";
		case SquareType::Treasure:
			return "T";
		default:
			return "0";
	}
}

/**
  Helper function to choose SquareType based on given chances
  @return SquareType
*/
SquareType ChooseRandomSquareType() {
    int rando = rand() % 100;
    if (rando < 20) {return SquareType::Wall;}
    else {return SquareType::Empty;}
}

/**
  Board constructor
  Set the edges then fill rest of the squares based off given chances
  Use helper function to decide between Wall/Empty
*/
Board::Board() {
    //set edges as walls and random for the inside
    for (int i = 0; i < rows_; i++) {
        for (int j = 0; j < cols_; j++) {
            if ((i == 0) || (j == 0) || (i == (rows_-1)) || (j == (cols_-1))) {
                arr_[i][j] = SquareType::Wall;
            } else {
                arr_[i][j] = ChooseRandomSquareType();
            }
        }
    }
    //10% chance that empty squares get treasure in them
    for (int i = 1; i < rows_-1; i++) {
        for (int j = 1; j < cols_-1; j++) {
            int rando = rand()%100;
            if (arr_[i][j] == SquareType::Empty && rando < 10) {
                arr_[i][j] = SquareType::Treasure;
            }
        }
    }
    //set start and exit positions
    arr_[1][1] = SquareType::Human;
    arr_[cols_-2][rows_-2] = SquareType::Exit;
}

/**
  Method to return all the valid Positions a Player can move to
  @param Player to check moves for
  @return vector of valid Positions p can move to 
*/
std::vector<Position> Board::GetMoves(Player *p) {
    std::vector<Position> return_vec;
    //get current position and create positions in each cardinal direction
    Position pos = p->get_position();
    Position north, east, south, west;
    north.row = pos.row - 1;
    north.col = pos.col;
    east.row = pos.row;
    east.col = pos.col + 1;
    south.row = pos.row + 1;
    south.col = pos.col;
    west.row = pos.row;
    west.col = pos.col - 1;
    //check if the positions are valid, if so, append to return vector
    if (get_square_value(north) != SquareType::Wall) {
        return_vec.push_back(north);
    }
    if (get_square_value(east) != SquareType::Wall) {
        return_vec.push_back(east);
    }
    if (get_square_value(south) != SquareType::Wall) {
        return_vec.push_back(south);
    }
    if (get_square_value(west) != SquareType::Wall) {
        return_vec.push_back(west);
    }
    return return_vec;
}

/**
  Method to move a Player to a Position
  @param Player the player to move
  @param Position to move player to
  @return bool if the move took place or not 
*/
bool Board::MovePlayer(Player *p, Position pos) {
    std::vector<Position> possibleMoves = GetMoves(p);
    for (int i = 0; i < possibleMoves.size(); i++) {
        if (possibleMoves.at(i) == pos) {
            //give 100 points if move on a treasure tile
            if (get_square_value(pos) == SquareType::Treasure) {
                p->ChangePoints(p->get_points() + 100);
            }
            //set the tile where the player was to empty
            arr_[p->get_position().row][p->get_position().col] = SquareType::Empty;
            //set squaretype for board and position of the player
            if (p->is_human()) {
                arr_[pos.row][pos.col] = SquareType::Human;
            } else {
                arr_[pos.row][pos.col] = SquareType::Enemy;
            }
            p->SetPosition(pos);
            return true;
        }
    }
    //if the given position does not match any that are possible to move to, do nothing / return false
    return false;
}

/**
  checks to make sure theres no walls that block all the way across
  and that you can enter/exit the starting and ending positions 
  POSSIBLE to fail, but VERY unlikely given 1 size maze
  @return bool representing if (this) Board is solvable
*/
/*
bool Board::IsSolvable() {
    if ((arr_[1][2] == SquareType::Wall) && (arr_[2][1] == SquareType::Wall)) {
        return false;
    } else if ((arr_[3][4] == SquareType::Wall) && (arr_[4][3] == SquareType::Wall)) {
        return false;
    } else if ((arr_[1][2] == SquareType::Wall) && (arr_[2][2] == SquareType::Wall) && (arr_[3][2] == SquareType::Wall) && (arr_[4][2] == SquareType::Wall)) {
        return false;
    } else if ((arr_[1][3] == SquareType::Wall) && (arr_[2][3] == SquareType::Wall) && (arr_[3][3] == SquareType::Wall) && (arr_[4][3] == SquareType::Wall)) {
        return false;
    } else if ((arr_[2][1] == SquareType::Wall) && (arr_[2][2] == SquareType::Wall) && (arr_[2][3] == SquareType::Wall) && (arr_[2][4] == SquareType::Wall)) {
        return false;
    } else if ((arr_[3][1] == SquareType::Wall) && (arr_[3][2] == SquareType::Wall) && (arr_[3][3] == SquareType::Wall) && (arr_[3][4] == SquareType::Wall)) {
        return false;
    } else if ((arr_[4][1] == SquareType::Wall) && (arr_[3][2] == SquareType::Wall) && (arr_[2][3] == SquareType::Wall) && (arr_[1][1] == SquareType::Wall)) {
        return false;
    } else if ((arr_[1][2] == SquareType::Wall) && (arr_[2][2] == SquareType::Wall) && (arr_[3][1] == SquareType::Wall)) {
        return false;
    } else if ((arr_[4][3] == SquareType::Wall) && (arr_[3][3] == SquareType::Wall) && (arr_[2][4] == SquareType::Wall)) {
        return false;
    }else {
        return true;
    }
}
*/

/**
  use BFS to check if a Board is solvable
  @return bool representing if (this) Board is solvable
*/
bool Board::IsSolvable() {
    if (!((arr_[1][2] == SquareType::Wall) && (arr_[2][1] == SquareType::Wall))) {
        std::queue<Position> q;
        Position starter;
        starter.row = 1;
        starter.col = 1;
        q.push(starter);

        while (!q.empty()) {
            Position p = q.front();
            q.pop();
            if (p.row == rows_-2 && p.col == cols_-2) {
                return true;
            }
            Position p1;
            p1.row = p.row + 1;
            p1.col = p.col;
            if (((p1.row) < (rows_-1)) && IsValid(p1)) {
                q.push(p1);
            }
            Position p2;
            p2.row = p.row;
            p2.col = p.col+1;
            if (((p2.col) < (cols_-1)) && IsValid(p2)) {
                q.push(p2);
            }
        }
    } 
    return false;
}

/**
  Overload << operator to print out Board
  @param os to overload
  @param Board to be printed
  @return os with a string representation of board
*/
std::ostream& operator<<(std::ostream& os, const Board &b) {
    std::string s = "";
    for (int i = 0; i < b.rows_; i++) {
        for (int j = 0; j < b.cols_; j++) {
            s.append(SquareTypeStringify(b.arr_[i][j]));
        }
        s.append("\n"); 
    }
    os << s;
    return os;
}


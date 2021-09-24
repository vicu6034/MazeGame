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
		case SquareType::Gold:
			return "G";
        case SquareType::Silver:
            return "S";
		default:
			return "0";
	}
}

/**
  Helper function to choose SquareType based on difficulty
  Use input to determine how many spaces to be returned Wall/Empty
  @return SquareType that is randomly selected
  @param character that tells the difficulty
*/
SquareType ChooseRandomSquareType(char difficulty) {
    int wall_chance;
    switch (difficulty) {
        case 'e':
            wall_chance = 20;
            break;
        case 'm':
            wall_chance = 30;
            break;
        case 'h':
            wall_chance = 40;
            break;
        default:
            wall_chance = 0;
            break;
    }

    int rando = rand() % 100;
    if (rando < wall_chance) {return SquareType::Wall;}
    else {return SquareType::Empty;}
}

/**
  Board constructor
  Set the edges then fill rest of the squares based off given chances
  Use helper function to decide between Wall/Empty
  @param char that represents what size the board should be
*/
Board::Board(char c_size, char c_diff) {
    //set num rows and cols based off the chosen map size
    switch (c_size) {
        case 's':
            rows_ = 6;
            cols_ = 6;
            break;
        case 'm':
            rows_ = 8;
            cols_ = 8;
            break;
        case 'l':
            rows_ = 10;
            cols_ = 10;
            break;
        default:
            rows_ = 8;
            cols_ = 8;
            break;
    }
    arr_.resize(rows_,std::vector<SquareType>(cols_));
    //set edges as walls and random for the inside
    for (int i = 0; i < rows_; i++) {
        for (int j = 0; j < cols_; j++) {
            if ((i == 0) || (j == 0) || (i == (rows_-1)) || (j == (cols_-1))) {
                arr_[i][j] = (SquareType::Wall);
            } else {
                arr_[i][j] = ChooseRandomSquareType(c_diff);
            }
        }
    }
    //10% chance that empty squares get treasure in them
    for (int i = 1; i < rows_-1; i++) {
        for (int j = 1; j < cols_-1; j++) {
            int rando = rand()%100;
            if (arr_[i][j] == SquareType::Empty && rando < 5) {
                arr_[i][j] = SquareType::Gold;
            } else if (arr_[i][j] == SquareType::Empty && rando < 15) {
                arr_[i][j] = SquareType::Silver;
            }
        }
    }
    //set start and exit positions
    arr_[1][1] = SquareType::Human;
    arr_[cols_-2][rows_-2] = SquareType::Exit;
}
/**
  Helper function to choose random positions for ai
  @param int num of enemies to be in the game
  @param Board so we can look for empty tiles to place enemies on
  @return vector of positions for the enemies
*/
std::vector<Position> Board::ChooseEnemyPositions(int num_enemies) {
    std::vector<Position> ret_vec;
    for (int i = 0; i < num_enemies; i++) {
        Position pos;//, starter, exit;
        //starter.row = 1;
        //starter.col = 1;
        //exit.row = rows_-2;
        //exit.col = cols_-2;
        int rand_row = 1 + ( std::rand() %  (rows_-1) );
        int rand_col = 1 + ( std::rand() %  (cols_-1) );
        pos.row = rand_row;
        pos.col = pos.col;
        while (get_square_value(pos) != SquareType::Empty) { //|| (pos == starter) || (pos == exit)) {
            rand_row = 1 + ( std::rand() %  (rows_-1) );
            rand_col = 1 + ( std::rand() %  (cols_ -1) );
            pos.row = rand_row;
            pos.col = rand_col;
        }
        ret_vec.push_back(pos);
    }
    return ret_vec;
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
            //give 100 points for Gold, 50 points for Silver
            if (get_square_value(pos) == SquareType::Gold) {
                p->ChangePoints(p->get_points() + 100);
            } else if (get_square_value(pos) == SquareType::Silver) {
                p->ChangePoints(p->get_points() + 50);
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


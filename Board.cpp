#include "Board.h"
#include <iostream>
#include <queue>
#include <set>
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
  @return vector of positions for the enemies
*/
std::vector<Position> Board::ChooseEnemyPositions(int num_enemies) {
    std::vector<Position> ret_vec;
    for (int i = 0; i < num_enemies; i++) {
        int rand_row = 1 + ( std::rand() %  (rows_-1) );
        int rand_col = 1 + ( std::rand() %  (cols_-1) );
        Position pos = {rand_row, rand_col};
        while (get_square_value(pos) != SquareType::Empty) {
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
    Position north = {pos.row - 1, pos.col};
    Position east = {pos.row, pos.col + 1};
    Position south = {pos.row + 1, pos.col};
    Position west = {pos.row, pos.col - 1};
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
  use BFS to check if a Board is solvable
  @return bool representing if (this) Board is solvable
*/
bool Board::IsSolvable() {
    if (!((arr_[1][2] == SquareType::Wall) && (arr_[2][1] == SquareType::Wall))) {
        std::queue<Position> q;
        Position starter = {1,1};
        q.push(starter);
        while (!q.empty()) {
            Position p = q.front();
            q.pop();
            if (p.row == rows_-2 && p.col == cols_-2) {
                return true;
            }
            Position p1 = {p.row+1, p.col};
            if (IsValid(p1)) {
                q.push(p1);
            }
            Position p2 = {p.row, p.col+1};
            if (IsValid(p2)) {
                q.push(p2);
            }
        }
    } 
    return false;
}

/**
  Helper method to ensure a given SquareType exists 
  @param SquareType to look for
  @return bool which tells if the SquareType exists
*/
bool Board::TypeExists(SquareType type) {
    //check every square in the board and return true if its found
    for (int i = 1; i < rows_-1; i++) {
        for (int j = 1; j < cols_-1; j++) {
            Position pos = {i,j};
            if (get_square_value(pos) == type) {
                return true;
            }
        }
    }
    //if the type is never found, return false
    //std::cout << "There are no " << SquareTypeStringify(type) << " on the board" << std::endl;
    return false;
}

/**
  Method to find if a path from a player to a given squaretype exists
  @param Player to find a move for 
  @param SquareType to look for
  @return Pos that is where the closest type is
*/
Position Board::PathToTypeExists(Player *p, SquareType type) {
    if (TypeExists(type)) {
        std::queue<Position> q;
        Position starter = {1,1};
        q.push(starter);
        while (!q.empty()) {
            Position p = q.front();
            q.pop();
            if (get_square_value(p) == type) {
                return p;
            }
            Position p1 = {p.row+1, p.col};
            if (IsValid(p1)) {
                q.push(p1);
            }
            Position p2 = {p.row, p.col+1};
            if (IsValid(p2)) {
                q.push(p2);
            }
        }
    } 
    Position dummy = {100,100,nullptr};
    return dummy;
}

/**
  Overload for FindPath to hep it backtrace and add nodes to path
  @param Position 
  @param vector<Position> that holds the path
*/
void Board::FindPath(Position* node, std::vector<Position*> &path) {
    if (node != nullptr) {
        FindPath(node->parent, path);
        //std::cout << node->row << "," << node->col << std::endl;
        path.push_back(node);
    }
}

/**
  Method to find a path between a player and a pos
  @param Player 
  @param Position that holds the path
  @return Position that is first Position on the path
*/
Position Board::FindPath(Player *p, Position pos) {
    int row[] = { -1, 0, 0, 1 };
    int col[] = { 0, -1, 1, 0 };
    // vector to store the shortest path
    std::vector<Position*> path;
    // create a queue and enqueue the first node
    std::queue<Position*> q;
    Position* src = new Position(p->get_position().row, p->get_position().col, nullptr);
    q.push(src);
    // set to check if the matrix cell is visited before or not
    std::set<Position*> visited;
    visited.insert(src);
    // loop till queue is empty
    while (!q.empty())
    {
        // dequeue front node and process it
        Position* curr = q.front();
        q.pop();
        int i = curr->row;
        int j = curr->col;
        // if the destination is found, print the shortest path and
        // return the shortest path
        if (i == pos.row && j == pos.col) {
            FindPath(curr, path);
            //Position pos = *path[1];
            return *path[1];
        }
        // get the value of the current cell
        // SquareType type = arr_[i][j];
        // check all four possible movements from the current cell
        // and recur for each valid movement
        for (int k = 0; k < 4; k++) {
            // get next position coordinates using the value of the current cell
            int x = i + row[k];
            int y = j + col[k];
            // check if it is possible to go to the next position
            // from the current position
            Position next_pos = {x,y};
            if (IsValid(next_pos)) {
                // construct the next cell node
                Position* next = new Position(x, y, curr);
                // if it isn't visited yet
                if (!visited.count(next)) {
                    // enqueue it and mark it as visited
                    q.push(next);
                    visited.insert(next);
                }
            }
        }
    }
    // we reach here if the path is not possible
    Position dummy = {100,100, nullptr};
    return dummy;
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


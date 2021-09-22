//#include <vector>
#include <string>
#include <iostream>

#include "Maze.h"
/**
  Helper function to choose random names for the ais
  @param int num of enemies to be in the game
  @return vector of strings that are names
*/
std::vector<std::string> ChooseRandomNames(const int num_enemies) {
    std::vector<std::string> names{"John","Alice","Bobby","Sydney","Bill","Sam","Ryan","Lily","Bailey","Michael"};
    std::vector<std::string> ret_vec;
    for (int i = 0; i < num_enemies; i++) {
        int rando = rand() % 10;
        ret_vec.push_back(names.at(rando));
    }
    return ret_vec;
}

/**
  Helper function to choose random positions for ai
  @param int num of enemies to be in the game
  @param Board so we can look for empty tiles to place enemies on
  @return vector of positions for the enemies
*/
std::vector<Position> ChooseEnemyPositions(const int num_enemies, const Board &b) {
    std::vector<Position> ret_vec;
    for (int i = 0; i < num_enemies; i++) {
        Position pos, starter, exit;
        int rand_row = 1 + (rand() % b.get_rows());
        int rand_col = 1 + (rand() % b.get_cols());
        pos.row = rand_row;
        pos.col = pos.col;
        while (!(b.get_square_value(pos) == SquareType::Empty) || (pos == starter) || (pos == exit)) {
            rand_row = 1 + (rand() % b.get_rows());
            rand_col = 1 + (rand() % b.get_cols());
            pos.row = rand_row;
            pos.col = rand_col;
        }
        ret_vec.push_back(pos);
    }
    return ret_vec;
}

/**
  Default maze constructor
*/
Maze::Maze() {
    std::vector<Board*> boards;
    boards.push_back(new Board());
    while (!boards.back()->IsSolvable()) {
        boards.push_back(new Board);
    }
    board_ = boards.back();
    
    turn_count_ = 0;
    current_player_idx_ = 0;
}

/**
  Method to set up the game with given human and num of enemies
  @param Player that is the human
  @param int number of enemies
*/
void Maze::NewGame(Player *human, const int enemies) {
    //set humans position to 1,1
    Position starterpack;
    starterpack.row = 1;
    starterpack.col = 1;
    human->SetPosition(starterpack);
    players_.push_back(human);
    //create number of enemies based off parameter
    //set their positions and ensure the board is updated
    std::vector<std::string> names = ChooseRandomNames(enemies);
    std::vector<Position> enemy_positions = ChooseEnemyPositions(enemies, *board_);
    for (int i = 0; i < enemies; i++) {
        Player *playa = new Player(names.at(i),false);
        playa->SetPosition(enemy_positions.at(i));
        players_.push_back(playa);
        board_->SetSquareValue(enemy_positions.at(i), SquareType::Enemy);
    }
}
/**
  Method to kill all players
  force end the game
*/
void Maze::KillAll() {
    for (int i = 0; i < players_.size(); i++) {
        players_.at(i)->Kill();
    }
}
/**
  Method to take the ais turn
  @param Player whos turn it is
*/
void Maze::DoEnemyTurn(Player *p) {
    std::cout << p->get_name() << "'s Turn (above)" << std::endl;
    std::vector<Position> possibles = board_->GetMoves(p);
    if (possibles.size() == 0) {return;}
    int rando = rand() % possibles.size();

    if (board_->get_square_value(possibles.at(rando)) == SquareType::Human) {
        std::cout << "An enemy has killed you!" << std::endl;
        KillAll();
    } else if ((board_->get_square_value(possibles.at(rando)) == SquareType::Enemy) || (board_->get_square_value(possibles.at(rando)) == SquareType::Exit)) {
        //do nothing if an enemy moves onto an enemy or the exit (they should never try to go into a wall)
    } else {
        board_->MovePlayer(p,possibles.at(rando));
    }
}

void Maze::DoHumanTurnLogic(Player *p, Position &pos) {
     if (board_->get_square_value(pos) == SquareType::Wall) {
        //the case where a player runs into a wall
        //do nothing
    } else if (board_->get_square_value(pos) == SquareType::Enemy) {
        //the case where a human runs into an ai
        //kill the human
        p->Kill();
    } else {
        //the case where theres a valid move, nove the player
        board_->MovePlayer(p, pos);
    }
}

/**
  Method to take a players turn
  @param Player whos turn it is
*/
void Maze::TakeTurn(Player *p) {
    if (p->is_human()) {current_player_idx_ = 0;}
    //print out the board
    std::cout << *board_ << std::endl;
    //cop out to have ai control when an enemy is up
    if (!p->is_human()) {
        DoEnemyTurn(p);
        turn_count_++;
        return;
    }
    //show player the valid directions they can move
    std::cout << "Available moves: ";
    std::vector<Position> moves = board_->GetMoves(p);
    for (int i = 0; i < moves.size(); i++) {
        std::cout << p->ToRelativePosition(moves.at(i)) << " ";
    }
    //get all the possible inputs
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
    //prompt player for input
    std::cout << std::endl << p->get_name() << "'s choice (Please input N , W , S , or E): ";
    std::string choice;
    std::cin >> choice;

    //ugly please fix
    if ((choice == "N") || (choice == "n") || (choice == "north") || (choice == "North") || (choice == "NORTH")) {
        //cases when the player goes north
        DoHumanTurnLogic(p,north);
    } else if ((choice == "E") || (choice == "e") || (choice == "east") || (choice == "East") || (choice == "EAST")) {
        //cases when the player goes east
        DoHumanTurnLogic(p,east);
    } else if ((choice == "S") || (choice == "s") || (choice == "south") || (choice == "South") || (choice == "SOUTH")) {
        //cases when the player goes south
        DoHumanTurnLogic(p,south);
    } else if ((choice == "W") || (choice == "w") || (choice == "west") || (choice == "West") || (choice == "WEST")) {
        //cases when the player goes west
        DoHumanTurnLogic(p,west);
    } else {
        //else theres an invalid input
        //alert the player they've lost their turn
        std::cout << "Invalid Input, skipping turn" << std::endl;
    }
    turn_count_++;
}

/**
  Method to return a ptr to the next player in the turn order
  @return Player that is next in the turn order
*/
Player* Maze::GetNextPlayer() {
    if (current_player_idx_ == 0) {
        current_player_idx_++;
        return (players_.at(current_player_idx_));
    } else if (current_player_idx_ == 1) {
        current_player_idx_++;
        return (players_.at(current_player_idx_));
    } else if ((current_player_idx_ == 2) && (players_.size() == 3)) {
        current_player_idx_ = 0;
        return players_.at(current_player_idx_);
    } else if ((current_player_idx_ == 2) && (players_.size() == 4)) {
        current_player_idx_++;
        return players_.at(current_player_idx_);
    } else if (current_player_idx_ == 3) {
        current_player_idx_ = 0;
        return players_.at(current_player_idx_);
    } else {
        std::cout << std::endl << "PROBLEMS - GETNEXTPLAYER() - MAZE.CPP" << std::endl;
        return nullptr;
    }
}

/**
  Method to check if the game has ended
  @return bool that represents if the game is over
*/
bool Maze::IsGameOver() {
    //check if human is at the end
    //if they are, give them a point and return true 
    if (board_->GetExitOccupant() == SquareType::Human) {
        for (int i = 0; i < players_.size(); i++) {
            if (players_.at(i)->is_human()) {
                players_.at(i)->ChangePoints(players_.at(i)->get_points() + 1);
            }
        }
        return true;
    }
    //check if any humans are left in the game
    //if none, return true
    int num_human = 0;
    for (int i = 0; i < players_.size(); i++) {
        if (players_.at(i)->is_human()) {
            num_human++;
        }
    }
    if (num_human == 0) {return true;}
    //if neither game-ending condition has been met, return false
    return false;
}

/**
  Method to create a string that contains the names and points of all players
  @return String containing names/points of all players
*/
std::string Maze::GenerateReport() {
    std::string s = "";
    for (int i = 0; i < players_.size(); i++) {
         s += players_.at(i)->get_name();
         s += ": ";
         s += std::to_string(players_.at(i)->get_points());
         s += "\n";
    }
    return s;
}
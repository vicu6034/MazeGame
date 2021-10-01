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
    std::vector<std::string> names{"John","Alice","Bobby","Sydney","Bill","Sam","Ryan","Lily","Ali","Michael","Tori"};
    std::vector<std::string> ret_vec;
    for (int i = 0; i < num_enemies; i++) {
        int rando = std::rand() % 10;
        ret_vec.push_back(names.at(rando));
    }
    return ret_vec;
}

/**
  Paramterized maze constructor
  @param char that represents size the Board should be
*/
Maze::Maze(const char c_size, const char c_diff) {
    std::vector<Board*> boards;
    boards.push_back(new Board(c_size, c_diff));
    while (!boards.back()->IsSolvable()) {
        boards.push_back(new Board(c_size, c_diff));
    }
    board_ = boards.back();
    map_size_ = c_size;
    difficulty_ = c_diff;
    current_player_idx_ = 0;
}

/**
  Method to set up the game with given human and num of enemies
  @param Player that is the human
  @param int number of enemies
*/
void Maze::NewGame(Player *human) {
    //set humans position to 1,1 and add them to players
    Position starterpack = {1,1};
    human->SetPosition(starterpack);
    players_.push_back(human);
    int enemies = CalculateEnemies();
    //create number of enemies based off parameter
    //set their positions and ensure the board is updated
    std::vector<std::string> names = ChooseRandomNames(enemies);
    std::vector<Position> enemy_positions = board_->ChooseEnemyPositions(enemies);
    std::vector<std::string> npc_types = ChooseNpcTypes();
    for (int i = 0; i < enemies; i++) {
        Player *playa = new Player(names.at(i), npc_types.at(i), enemy_positions.at(i), false);
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
    //if theres no moves, do nothing
    //else do the enemies turn
    if (possibles.empty()) {}
    else {
        if (p->get_npc_type() == "Random") {
            int rando = rand() % possibles.size();
            DoTurnLogic(p, possibles.at(rando));
        } else if (p->get_npc_type() == "Killer") {
            if (board_->PathToTypeExists(p, SquareType::Human)) {
                std::vector<Position*> path = board_->findPath(p, board_->ClosestOfType(p,SquareType::Human));
                Position pos = *path.at(1);
                DoTurnLogic(p, pos);
            } else {
                int rando = rand() % possibles.size();
                DoTurnLogic(p, possibles.at(rando));
            }
        } else if (p->get_npc_type() == "Seeker") {
            if (board_->PathToTypeExists(p, SquareType::Gold)) {
                std::vector<Position*> path = board_->findPath(p, board_->ClosestOfType(p,SquareType::Gold));
                Position pos = *path.at(1);
                DoTurnLogic(p, pos);
            } else if (board_->PathToTypeExists(p, SquareType::Silver)) {
                std::vector<Position*> path = board_->findPath(p, board_->ClosestOfType(p,SquareType::Silver));
                Position pos = *path.at(1);
                DoTurnLogic(p, pos);
            } else {
                int rando = rand() % possibles.size();
                DoTurnLogic(p, possibles.at(rando));
            }
        } else {
            std::cout << "Problem with npc_type in DoEnemyTurn" << std::endl;
        }
    }
}

/**
  Method to compute what happens after a player chooses their direction
  @param Player whos turn it is
  @param Position they would like to move to
*/
void Maze::DoTurnLogic(Player *p, Position &pos) {
    if (p->is_human()) {
        if (board_->get_square_value(pos) == SquareType::Wall) {
            //the case where a player runs into a wall, do nothing
        } else if (board_->get_square_value(pos) == SquareType::Enemy) {
            //the case where a human runs into an ai, kill the human
            p->Kill();
        } else {
            //the case where theres a valid move, move the player
            board_->MovePlayer(p, pos);
        }
    } else {
        if (board_->get_square_value(pos) == SquareType::Human) {
            //if ai moves onto a human, kill the human
            std::cout << "An enemy has killed you!" << std::endl;
            KillAll();
        } else if ((board_->get_square_value(pos) == SquareType::Enemy) || (board_->get_square_value(pos) == SquareType::Exit)) {
            //do nothing if an enemy moves onto an enemy or the exit (they should never try to go into a wall)
        } else {
            //a normal move
            board_->MovePlayer(p, pos);
        }
    }
}

/**
  Method to take a players turn
  @param Player whos turn it is
*/
void Maze::TakeTurn(Player *p) {
    //print out the board
    std::cout << *board_ << std::endl;
    //have ai control when an enemy is up
    if (!p->is_human()) {
        DoEnemyTurn(p);
    } else {
        //show player the valid directions they can move
        std::cout << "Available moves: ";
        std::vector<Position> moves = board_->GetMoves(p);
        for (int i = 0; i < moves.size(); i++) {
            std::cout << p->ToRelativePosition(moves.at(i)) << " ";
        }
        //get all the possible inputs
        Position pos = p->get_position();
        Position north = {pos.row - 1, pos.col};
        Position east = {pos.row, pos.col + 1};
        Position south = {pos.row + 1, pos.col};
        Position west = {pos.row, pos.col - 1};
        //prompt player for input
        std::cout << std::endl << p->get_name() << "'s choice (Please input N , W , S , or E): ";
        std::string choice;
        std::cin >> choice;
        char c = NormalizeInput(choice);
        if (c == 'n') {
            //cases when the player goes north
            DoTurnLogic(p,north);
        } else if (c == 'e') {
            //cases when the player goes east
            DoTurnLogic(p,east);
        } else if (c == 's') {
            //cases when the player goes south
            DoTurnLogic(p,south);
        } else if (c == 'w') {
            //cases when the player goes west
            DoTurnLogic(p,west);
        } else {
            //else theres an invalid input
            //alert the player they've lost their turn
            std::cout << "Invalid Input, skipping turn" << std::endl;
        }
    }
}

/**
  Method to return a ptr to the next player in the turn order
  @return Player* that is next in the turn order
*/
Player* Maze::GetNextPlayer() {
    switch (players_.size()) {
        case 2:
            if (current_player_idx_ == 0) {
                current_player_idx_++;
                return (players_.at(current_player_idx_));
            } else if (current_player_idx_ == 1) {
                current_player_idx_ = 0;
                return (players_.at(current_player_idx_));
            }
        case 3:
            if (current_player_idx_ == 0) {
                current_player_idx_++;
                return (players_.at(current_player_idx_));
            } else if (current_player_idx_ == 1) {
                current_player_idx_++;
                return (players_.at(current_player_idx_));
            } else if (current_player_idx_ == 2) {
                current_player_idx_ = 0;
                return (players_.at(current_player_idx_));
            }
        case 4:
            if (current_player_idx_ == 0) {
                current_player_idx_++;
                return (players_.at(current_player_idx_));
            } else if (current_player_idx_ == 1) {
                current_player_idx_++;
                return (players_.at(current_player_idx_));
            } else if (current_player_idx_ == 2) {
                current_player_idx_++;
                return (players_.at(current_player_idx_));
            } else if (current_player_idx_ == 3) {
                current_player_idx_ = 0;
                return (players_.at(current_player_idx_));
            }
        case 5:
            if (current_player_idx_ == 0) {
                current_player_idx_++;
                return (players_.at(current_player_idx_));
            } else if (current_player_idx_ == 1) {
                current_player_idx_++;
                return (players_.at(current_player_idx_));
            } else if (current_player_idx_ == 2) {
                current_player_idx_++;
                return (players_.at(current_player_idx_));
            } else if (current_player_idx_ == 3) {
                current_player_idx_++;
                return (players_.at(current_player_idx_));
            } else if (current_player_idx_ == 4) {
                current_player_idx_ = 0;
                return (players_.at(current_player_idx_));
            }
        default:
            std::cout << "THIS SHOULDNT HAPPEN - GETNEXTPLAYER()" << std::endl;
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

/**
  Helper method to calculate how many enemies should be in the game
  Based off private difficulty and size members
  @return int that represents how many enemies to be in the game
*/
int Maze::CalculateEnemies() {
    if ((map_size_ == 's') && (difficulty_ == 'e')) {
        return 1;
    } else if (((map_size_ == 's') && (difficulty_ != 'e')) || ((map_size_ == 'm') && (difficulty_ == 'e'))){
        return 2;
    } else if (((map_size_ == 'm') && (difficulty_ != 'e')) || ((map_size_ == 'l') && (difficulty_ == 'e'))) {
        return 3;
    } else {
        return 4;
    }
}

/**
  Helper function to choose random names for the ais
  @param int num of enemies to be in the game
  @return vector of strings that are names
*/
std::vector<std::string> Maze::ChooseNpcTypes() {
    //Random NPCs will move randomly
    //Killer NPCs will hunt down the player
    //Seeker NPCs will try to collect treasure
    std::vector<std::string> types{"Random","Killer","Seeker"};
    std::vector<std::string> ret_vec;
    //hardcode in what npc types we want depending on the map size and difficulty
    switch (map_size_) {
        case 's':
            if (difficulty_ == 'e') {
                ret_vec.push_back(types[0]);
                break;
            } else if (difficulty_ == 'm') {
                ret_vec.push_back(types[0]);
                ret_vec.push_back(types[2]);
                break;
            } else {
                ret_vec.push_back(types[0]);
                ret_vec.push_back(types[1]);
                break;
            }
        case 'm':
            if (difficulty_ == 'e') {
                ret_vec.push_back(types[0]);
                ret_vec.push_back(types[0]);
                break;
            } else if (difficulty_ == 'm') {
                ret_vec.push_back(types[0]);
                ret_vec.push_back(types[0]);
                ret_vec.push_back(types[2]);
                break;
            } else {
                ret_vec.push_back(types[0]);
                ret_vec.push_back(types[1]);
                ret_vec.push_back(types[2]);
                break;
            }
        case 'l':
            if (difficulty_ == 'e') {
                ret_vec.push_back(types[0]);
                ret_vec.push_back(types[0]);
                ret_vec.push_back(types[2]);
                break;
            } else if (difficulty_ == 'm') {
                ret_vec.push_back(types[0]);
                ret_vec.push_back(types[1]);
                ret_vec.push_back(types[2]);
                ret_vec.push_back(types[2]);
                break;
            } else {
                ret_vec.push_back(types[0]);
                ret_vec.push_back(types[1]);
                ret_vec.push_back(types[1]);
                ret_vec.push_back(types[2]);
                break;
            }
    }
    return ret_vec;
}
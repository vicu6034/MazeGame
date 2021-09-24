#include "Player.h"
#include "Maze.h"

#include <iostream>

char NormalizeInput(std::string str) {
    return tolower(str[0]);
}

int main() {
    //seed random nums to be used throughout
    srand(time(0));
    //greet the player and prompt for name 
    std::string name;
    std::cout << "HI! Welcome to my Maze!" << std::endl;
    std::cout << "Please input your name to play: ";
    std::cin >> name;
    //prompt player for a size board to play on
    std::string board_size;
    std::cout << "What size board would you like to play on, " << name << "?" << std::endl;
    std::cout << "Please choose small, medium, or large (s, m, l) ";
    std::cin >> board_size;
    char c_size = NormalizeInput(board_size);
    //prompt player for a difficulty to play with
 //   std::string difficulty;
 //   std::cout << "What difficulty would you like to play on, " << name << "?" << std::endl;
 //   std::cout << "Please choose easy, medium, or hard (e, m, h) ";
 //   std::cin >> difficulty;
 //   char c_diff = NormalizeInput(difficulty);
    //print out a key
    std::cout << std::endl << "KEY: " << std::endl;
    std::cout << "    EMPTY SPACES ARE 'OPEN'" << std::endl;
    std::cout << "    H = Human (YOU)" << std::endl;
    std::cout << "    X = Enemy (DEATH)" << std::endl;
    std::cout << "    W = Wall (BLOCKED)" << std::endl;
    std::cout << "    T = Treasure (POINTS)" << std::endl;
    std::cout << "    E = End (WIN)" << std::endl << std::endl;
    //create the maze and human player
    Player *myPlayer = new Player(name, true);
    Maze *myMaze = new Maze(c_size);
    myMaze->NewGame(myPlayer,2);
    //have the player go then use nextplayer to loop until the games over 
    myMaze->TakeTurn(myPlayer);
    while (!myMaze->IsGameOver()) {
        myMaze->TakeTurn(myMaze->GetNextPlayer());
    }
    //show how many points the players got
    std::cout << myMaze->GenerateReport();
}
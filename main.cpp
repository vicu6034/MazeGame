#include "Player.h"
#include "Maze.h"

#include <iostream>

int main() {
    //seed random nums to be used throughout
    srand(time(0));
    //greet the player and prompt for name 
    std::string name;
    std::cout << "HI! Welcome to my Maze!" << std::endl;
    std::cout << "Please input your name to play: ";
    std::cin >> name;
    //create the maze and human player
    Player *myPlayer = new Player(name, true);
    Maze *myMaze = new Maze();
    myMaze->NewGame(myPlayer,2);
    //have the player go then use nextplayer to loop until the games over 
    myMaze->TakeTurn(myPlayer);
    while (!myMaze->IsGameOver()) {
        myMaze->TakeTurn(myMaze->GetNextPlayer());
    }
    //show how many points the players got
    std::cout << myMaze->GenerateReport();
}
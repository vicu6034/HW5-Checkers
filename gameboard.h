#ifndef GAMEBOARD_H
#define GAMEBOARD_H

#include <tile.h>
#include <player.h>
#include <pieceprototypefactory.h>
#include <powerup.h>

class GameBoard
{

public:
    GameBoard();

private:
    PiecePrototypeFactory* factory_;
    std::vector<Tile*> board_;
    std::vector<Player*> players_;
    std::vector<PowerUp*> powerups_;

};

#endif // GAMEBOARD_H

#ifndef GAMEBOARD_H
#define GAMEBOARD_H

#include <tile.h>
#include <player.h>
#include <pieceprototypefactory.h>
#include <powerup.h>

class GameBoard
{

private:
    // game has a factory
    PieceFactory* factory_;
    // game has tiles, pieces, and powerups
    std::vector<Tile*> tiles_;
    std::vector<Player*> players_;
    std::vector<PowerUp*> powerups_;

public:
    // only need default constructor
    GameBoard();

    // getters for GraphicsItems so we can add them to scene
    std::vector<Tile*> getTiles() { return tiles_; }
    std::vector<PowerUp*> getPowerUps() { return powerups_; }
    std::vector<PiecePrototype*> getPieces();

};

#endif // GAMEBOARD_H

#ifndef GAMEBOARD_H
#define GAMEBOARD_H

#include <tile.h>
#include <player.h>
#include <pieceprototypefactory.h>
#include <powerup.h>

class GameBoard
{

private:
    PieceFactory* factory_;
    std::vector<Tile*> tiles_;
    std::vector<Player*> players_;
    std::vector<PowerUp*> powerups_;

public:
    GameBoard();

    // getters for GraphicsItems so we can add them to scene
    std::vector<Tile*> getTiles() { return tiles_; }
    std::vector<PowerUp*> getPowerUps() { return powerups_; }
    std::vector<PiecePrototype*> getPieces();

};

#endif // GAMEBOARD_H

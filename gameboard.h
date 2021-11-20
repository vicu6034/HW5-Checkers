#ifndef GAMEBOARD_H
#define GAMEBOARD_H

#include <tile.h>
#include <player.h>
#include <pieceprototypefactory.h>
#include <powerup.h>

class GameBoard : public QObject
{
    Q_OBJECT

private:
    // game has a factory
    PieceFactory* factory_;
    // game has tiles, pieces, and powerups
    std::vector<Tile*> tiles_;
    std::vector<Player*> players_;
    std::vector<PowerUp*> powerups_;

    int current_player_;

public:
    // only need default constructor
    GameBoard();

    // getters for GraphicsItems so we can add them to scene
    std::vector<Tile*> getTiles() { return tiles_; }
    std::vector<PowerUp*> getPowerUps() { return powerups_; }
    std::vector<PiecePrototype*> getPieces();
    PiecePrototype* getPiece(Position pos);
    // get the selected piece
    PiecePrototype* getSelectedPiece();
    // deselect pieces
    void deselectPiece();

    bool checkValidity(Tile* t, PiecePrototype* p);
    // helper method for tileSelected
    void handleSelected(Tile* t, PiecePrototype* p, bool red);

signals:
    void addPiece(PiecePrototype* p);
    void updateTurnLabel(int turn);
    void updatePiecesLabel(bool red, int pieces);

public slots:
    void tileSelected(Tile* t);
    void pieceSelected();
};


#endif // GAMEBOARD_H

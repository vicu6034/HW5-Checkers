#ifndef GAMEBOARD_H
#define GAMEBOARD_H

#include <tile.h>
#include <player.h>
#include <pieceprototypefactory.h>
#include <powerup.h>
#include <QWidget>

class GameBoard : public QWidget
{
    Q_OBJECT

private:
    // game has a factory
    PieceFactory* factory_;

    // game has tiles, players, and powerups
    // pieces live in players
    std::vector<Tile*> tiles_;
    std::vector<Player*> players_;
    std::vector<PowerUp*> powerups_;

    // keep track of currently selected Piece
    PiecePrototype* selected_;

    // track what players turn it is
    int current_player_;

public:
    // only need default constructor
    GameBoard();

    // method to reset GameBoard to its state immediately after initialization
    void Reset();

    void NewGame();

    // getters
    std::vector<Tile*> getTiles() { return tiles_; }
    std::vector<PowerUp*> getPowerUps() { return powerups_; }
    std::vector<PiecePrototype*> getPieces();
    PiecePrototype* getPiece(Position pos);
    Player* getCurrentPlayer() { return players_[current_player_]; }
    int getCurrentPlayerInt() { return current_player_; }
    Player* getOtherPlayer() { return players_[!current_player_]; }

    std::string checkPowerup(Position pos);
    void removePowerup(Position pos);

    bool checkRegularMoves(Position t_pos, Position s_pos, bool red);
    bool checkKingMoves(Position t_pos, Position s_pos, bool red);
    bool checkTripleKMoves(Position t_pos, Position s_pos, bool red);
    // handle when a player wants to jump (helps checkValidity)
    bool jumpHelper(Position pos, bool red);
    bool friendlyJumpHelper(Position pos, bool red);
    bool doubleJumpHelper(Position pos1, Position pos2, bool red);
    // check if a tile is valid for a piece to move to (helps handleSelected)
    bool checkValidity(Tile* t, bool red);
    // if a move was valid check if someone won
    bool checkForWinner();
    // handles what happens when a tile is selected (helps pieceSelected)
    void handleSelected(Tile* t, bool red);
    void handlePowerup(Position t_pos, Position last_pos, bool red);

signals:
   // custom signals to emit when we need to update something in window
    void updateTurnLabel(int turn);
    void updatePiecesLabel(bool red, int pieces);
    void updatePiece(PiecePrototype* p);
    void addPiece(PiecePrototype* p);
    void removePiece(PiecePrototype* p);
    void gameOver();

    void playSlideSound();

public slots:
    // custom slots to handle when tiles and pieces are clicked
    void tileSelected(Tile* t);
    void pieceSelected(PiecePrototype* p);

};

#endif // GAMEBOARD_H

#ifndef GAMEBOARD_H
#define GAMEBOARD_H

#include <tile.h>
#include <player.h>
#include <pieceprototypefactory.h>
#include <powerup.h>
#include <QWidget>

enum class Difficulty { None = 0, Easy, Medium, Hard };

struct Move {
    PiecePrototype* piece_;
    Tile* tile_;
    int score_;
};

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

    // difficulty for SP games
    Difficulty difficulty_;

    // keep track of currently selected Piece
    PiecePrototype* selected_;

    // track what players turn it is
    int current_player_;

public:
    // only need default constructor
    GameBoard();

    // method to reset GameBoard to its state immediately after initialization
    void NewGame();

    // getters
    std::vector<Tile*> getTiles() const { return tiles_; }
    std::vector<PowerUp*> getPowerUps() const { return powerups_; }
    std::vector<PiecePrototype*> getPieces() const;
    PiecePrototype* getPiece(Position pos) const;
    Player* getPlayer(int index) const { return players_[index]; }
    int getCurrentPlayerInt() const { return current_player_; }
    Difficulty getDifficulty() const { return difficulty_; }
    void setDifficulty(Difficulty difficulty) { difficulty_ = difficulty; }
    std::string checkPowerup(Position pos);
    void removePowerup(Position pos);
    // check moves for different types of pieces
    int checkRegularMoves(Position t_pos, Position s_pos, bool red, bool jump);
    int checkKingMoves(Position t_pos, Position s_pos, bool red, bool jump);
    int checkTripleKMoves(Position t_pos, Position s_pos, bool red, bool jump);
    // handle when a player wants to jump (helps checkValidity)
    int jumpHelper(Position pos, bool red, bool jump);
    int friendlyJumpHelper(Position pos, bool red, bool jump);
    int doubleJumpHelper(Position pos1, Position pos2, bool red, bool jump);
    // check if a tile is valid for a piece to move to (helps handleSelected)
    int checkValidity(Tile* t, PiecePrototype* p, bool red, bool jump);
    // if a move was valid check if someone won
    int checkForWinner();
    // handles what happens when a tile is selected (helps pieceSelected)
    void handleSelected(Tile* t, bool red);
    Position findPosForPowerup(bool red);
    void handlePowerup(Position t_pos, Position last_pos, bool red);

    std::vector<Tile*> getPieceMoves(PiecePrototype* p);
    std::pair<PiecePrototype*, Tile*> chooseMove(std::vector<PiecePrototype*> pieces);

signals:
    // custom signals to emit when we need to update something in window
    void updateTurnLabel(int turn);
    void updatePiecesLabel(bool red, int pieces);
    void updatePiece(PiecePrototype* p);
    void addPiece(PiecePrototype* p);
    void removePiece(PiecePrototype* p);
    void gameOver(int winner);
    // signals to have mainwindow play sounds
    void playSlideSound();
    void playJumpSound();
    void playDeniedSound();

public slots:
    // custom slots to handle when tiles and pieces are clicked
    void tileSelected(Tile* t);
    void pieceSelected(PiecePrototype* p);

    // slot for when SP ai needs to take its turn
    void AI_Timer_slot();

};

#endif // GAMEBOARD_H

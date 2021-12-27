/*
 * GameBoard Header (Game Class)
 * Holds information about Players, Pieces, Tiles, PowerUps
 * Runs the actual Checkers game
 * CSCI 3010 Homework 5
 * By: Vincent Curran & Philip Knott
*/

#ifndef GAMEBOARD_H
#define GAMEBOARD_H

#include <QWidget>
#include <QTime>
#include <QTimer>

#include <pieceprototype.h>
#include <tile.h>
#include <player.h>
#include <pieceprototypefactory.h>
#include <powerup.h>
#include <hash.h>

// none for multiplayer, simulation for simulations
// easy medium hard for single player
enum class Difficulty { None = 0, Easy, Medium, Hard, Simulation };

// struct to represent a Move
struct Move {
    // Piece to move to a Tile
    PiecePrototype* piece;
    Tile* tile;
    // move has a score based on how 'good' it is
    int score;
};

class GameBoard : public QWidget
{
    Q_OBJECT

private:
    // timers for AI
    QTimer* red_timer_;
    QTimer* black_timer_;

    // factory for creating pieces
    PieceFactory* factory_;

    // game has tiles, players, and powerups (pieces live in players)
    HashTable tiles_;
    //std::vector<Tile*> tiles_;
    std::vector<Player*> players_;
    std::vector<PowerUp*> powerups_;

    // track what kind of game to run
    Difficulty difficulty_;

    // track currently selected Piece
    PiecePrototype* selected_;

    // track what players turn it is
    int current_player_;

    // check moves for different types of pieces
    int checkRegularMoves(Position t_pos, Position s_pos, bool red, bool jump);
    int checkKingMoves(Position t_pos, Position s_pos, bool red, bool jump);
    int checkTripleKMoves(Position t_pos, Position s_pos, bool red, bool jump);

    // see if moving to a position will leave a piece vulnerable to be jumped
    int couldGetJumped(Position t_pos, Position s_pos, bool red);

    // handle when a player wants to jump over a piece
    int jumpHelper(Position pos, bool red, bool jump);
    // only a TKing can jump over a friendly piece
    int friendlyJumpHelper(Position pos, bool red, bool jump);
    // handle jumping over 2 pieces at once, this is TKings special move
    int doubleJumpHelper(Position pos1, Position pos2, bool red, bool jump);

    // check if a tile is valid for a piece to move to, and return score of the move
    int checkValidity(Position t_pos, PiecePrototype* p, bool red, bool jump);
    // if a move was valid check if someone won
    int checkForWinner();
    // handles what happens when a tile is selected (helps pieceSelected_slot)
    void doMove(Position t_pos, Position s_pos, bool red);
    void checkLanding(Position t_pos, bool red);
    void handleSelected(Tile* t, bool red);

    // see if theres a powerup at a position and return string of its name
    std::string checkPowerup(Position pos);
    // remove a powerup from the game
    void removePowerup(Position pos);
    // find an open space to put the piece added from powerup to
    Position findPosForPowerup(bool red);
    void handlePowerup(Position t_pos, Position last_pos, bool red);
    // get all the valid tiles a piece could move to
    std::vector<Tile*> getPieceMoves(PiecePrototype* p);
    void doAITurn(int turn);

public:
    // default constructor
    GameBoard();

    // method to reset GameBoard to its state immediately after initialization
    void NewGame();
    // stop ai from running
    void StopTimers();

    // getters and setter
    std::vector<Tile*> get_tiles() const;
    std::vector<PowerUp*> get_powerups() const { return powerups_; }
    std::vector<PiecePrototype*> get_pieces() const;
    PiecePrototype* get_piece(Position pos) const;
    Player* get_player(int index) const { return players_[index]; }
    int get_current_player() const { return current_player_; }
    Difficulty get_difficulty() const { return difficulty_; }
    void set_difficulty(Difficulty difficulty) { difficulty_ = difficulty; }

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
    void tileSelected_slot(Tile* t);
    void pieceSelected_slot(PiecePrototype* p);

private slots:
    // slot for when SP ai needs to take its turn
    void black_timer_slot();
    void red_timer_slot();

};

#endif // GAMEBOARD_H

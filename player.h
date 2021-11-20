#ifndef PLAYER_H
#define PLAYER_H

#include <pieceprototype.h>

class Player
{

private:
    // if we want to add ability to have people enter names
    // otherwise use Player 1, Player 2 until then
    //std::string name_;

    // one Player is_red_, the other is not (to represent who is who)
    bool is_red_;
    // each player has a vec of their Pieces
    std::vector<PiecePrototype*> pieces_;

public:
    // paramterized constructor with initialization list
    Player(bool is_red, std::vector<PiecePrototype*> pieces)
        : is_red_(is_red), pieces_(pieces) {}

    // getters
    bool get_is_red() { return is_red_; }
    std::vector<PiecePrototype*> get_pieces() { return pieces_; }

    void addPiece(PiecePrototype* p) { pieces_.push_back(p); }

};

#endif // PLAYER_H

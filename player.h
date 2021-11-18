#ifndef PLAYER_H
#define PLAYER_H

#include <pieceprototype.h>

class Player
{

private:
    // if we want to add ability to have people enter names
    // otherwise use Player 1, Player 2 until then
    //std::string name_;

    bool is_red_;
    std::vector<PiecePrototype*> pieces_;

public:
    // should never get player num -1
    Player() { is_red_ = true; }
    Player(bool is_red, std::vector<PiecePrototype*> pieces)
        : is_red_(is_red), pieces_(pieces) {}

    bool get_is_red() { return is_red_; }
    std::vector<PiecePrototype*> get_pieces() { return pieces_; }

};

#endif // PLAYER_H

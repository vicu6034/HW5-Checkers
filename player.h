#ifndef PLAYER_H
#define PLAYER_H

#include <pieceprototype.h>

class Player
{

private:
    // if we want to add ability to have people enter names
    // otherwise use Player 1, Player 2 until then
    //std::string name_;

    int player_num_;
    std::vector<PiecePrototype*> pieces_;

public:
    // should never get player num -1
    Player() { player_num_ = -1; }
    Player(int player_num, std::vector<PiecePrototype*> pieces)
        : player_num_(player_num), pieces_(pieces) {}

    int get_player_num() { return player_num_; }

};

#endif // PLAYER_H

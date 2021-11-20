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
    // num wins
    int wins_;

public:
    // paramterized constructor with initialization list
    Player(bool is_red, std::vector<PiecePrototype*> pieces)
        : is_red_(is_red), pieces_(pieces), wins_(0) {}

    // getters
    bool get_is_red() { return is_red_; }
    std::vector<PiecePrototype*> get_pieces() const { return pieces_; }
    void set_pieces(std::vector<PiecePrototype*> pieces) { pieces_ = pieces; }

    int get_num_pieces() { return pieces_.size(); }
    int get_num_wins() { return wins_; }
    void set_num_wins(int wins) { wins_ = wins; }

    // add/remove piece
    void addPiece(PiecePrototype* p) { pieces_.push_back(p); }
    void removePiece(Position p);
};

#endif // PLAYER_H

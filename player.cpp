#include "player.h"

// delete a piece by position
void Player::removePiece(Position pos) {
    for (unsigned int i = 0 ; i < pieces_.size(); i++) {
        if (pieces_[i]->get_position() == pos) {
            delete pieces_[i];
            pieces_.erase(pieces_.begin() + i);
        }
    }
}

// update a pieces position
void Player::updatePiece(Position old_pos, Position new_pos) {
    for (unsigned int i = 0 ; i < pieces_.size(); i++) {
        if (pieces_[i]->get_position() == old_pos) {
            pieces_[i]->set_position(new_pos);
            pieces_[i]->set_highlighted(false);
        }
    }
}

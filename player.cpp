#include "player.h"

void Player::removePiece(Position pos) {
    for (unsigned int i = 0 ; i < pieces_.size(); i++) {
        if (pieces_[i]->get_position() == pos) {
            delete pieces_[i];
            pieces_.erase(pieces_.begin() + i);
        }
    }
}

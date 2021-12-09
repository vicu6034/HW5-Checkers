#include "player.h"

#include <pieceprototype.h>

/* Remove (delete) a piece by position
 * @param Position of piece we want to remove
*/
void Player::removePiece(Position pos) {
    for (unsigned int i = 0 ; i < pieces_.size(); i++) {
        if (pieces_[i]->get_position() == pos) {
            delete pieces_[i];
            pieces_.erase(pieces_.begin() + i);
        }
    }
}

/* Update position of piece
 * @param Position old_pos : position of piece we want to update
 * @param Position new_pos : position we want piece to go to
*/
void Player::updatePiece(Position old_pos, Position new_pos) {
    for (unsigned int i = 0 ; i < pieces_.size(); i++) {
        if (pieces_[i]->get_position() == old_pos) {
            pieces_[i]->set_position(new_pos);
            pieces_[i]->set_highlighted(false);
        }
    }
}

#include "gameboard.h"

GameBoard::GameBoard()
{
    current_player_ = 0;
    // here we will set up the starting state of the game

    // instantiate initial Tiles and Pieces
    std::vector<PiecePrototype*> red_pieces;
    std::vector<PiecePrototype*> black_pieces;
    bool switcher = false;
    for (int i = 0; i < 10; i++) {
        // create pieces
        if (i % 2 == 0) {
            black_pieces.push_back(factory_->CreatePiece(PieceType::RegularPiece, Position{i,0}, false));
            black_pieces.push_back(factory_->CreatePiece(PieceType::RegularPiece, Position{i,2}, false));
            red_pieces.push_back(factory_->CreatePiece(PieceType::RegularPiece, Position{i,8}, true));
        } else {
            black_pieces.push_back(factory_->CreatePiece(PieceType::RegularPiece, Position{i,1}, false));
            red_pieces.push_back(factory_->CreatePiece(PieceType::RegularPiece, Position{i,7}, true));
            red_pieces.push_back(factory_->CreatePiece(PieceType::RegularPiece, Position{i,9}, true));  
        }

        // create tiles
        for (int j = 0; j < 10; j++) {
            tiles_.push_back(new Tile(Position{i,j}, switcher));
            switcher = !switcher;
        }
        switcher = !switcher;

    }

    // create players with their initial pieces
    players_.push_back(new Player(true, red_pieces));
    players_.push_back(new Player(true, black_pieces));

    // add powerups to the game
    // PowerUp(pos, is_addPiece)
    powerups_.push_back(new PowerUp(Position{4,4}, true));
    powerups_.push_back(new PowerUp(Position{5,5}, false));

}

// method to return all pieces
std::vector<PiecePrototype*> GameBoard::getPieces() {
    std::vector<PiecePrototype*> vec1 = players_[0]->get_pieces();
    std::vector<PiecePrototype*> vec2 = players_[1]->get_pieces();
    vec1.insert(vec1.end(), vec2.begin(), vec2.end());
    return vec1;
}

// method to return the selected piece
PiecePrototype* GameBoard::getSelectedPiece() {
    // get all pieces
    for (PiecePrototype* p : getPieces()) {
        // if ones selected, return it
        if (p->get_selected()) return p;
    }
    // if no pieces selected, return null
    return nullptr;
}

PiecePrototype* GameBoard::getPiece(Position pos) {
    // get all pieces
    for (PiecePrototype* p : getPieces()) {
        // if ones selected, return it
        if (p->get_position() == pos) return p;
    }
    // if no pieces selected, return null
    return nullptr;
}

// method to deselect pieces
void GameBoard::deselectPiece() {
    // get all pieces
    for (PiecePrototype* p : getPieces()) {
        // set them all to not be selected
        if (p->get_selected()) p->set_selected(false);
    }
}

bool GameBoard::checkValidity(Tile* t, PiecePrototype*p) {
   Position p_pos = p->get_position();
   Position t_pos = t->get_position();

   if (p->get_is_red()) {
       // reds turn
       if ((t_pos.x == p_pos.x+1 || t_pos.x == p_pos.x-1) && t_pos.y == p_pos.y-1) {
           // try to go one space
           if (getPiece(t_pos) == nullptr) {
               return true;
           }
       } else if (t_pos.x == p_pos.x-2 && t_pos.y == p_pos.y-2) {
           // try to jump piece to left
           if (getPiece(Position{p_pos.x-1, p_pos.y-1}) != nullptr) {
               if (getPiece(t_pos) == nullptr && !getPiece(Position{p_pos.x-1, p_pos.y-1})->get_is_red()) {
                    // red single jump black
                    players_[1]->removePiece(Position{p_pos.x-1, p_pos.y-1});
                    emit updatePiecesLabel(false, players_[1]->get_num_pieces());
                    return true;
               }
           }
       } else if (t_pos.x == p_pos.x+2 && t_pos.y == p_pos.y-2) {
           // try to jump piece to right
           if (getPiece(Position{p_pos.x+1, p_pos.y-1}) != nullptr) {
               if (getPiece(t_pos) == nullptr && !getPiece(Position{p_pos.x+1, p_pos.y-1})->get_is_red()) {
                    // red single jump black
                    players_[1]->removePiece(Position{p_pos.x+1, p_pos.y-1});
                    emit updatePiecesLabel(false, players_[1]->get_num_pieces());
                    return true;
               }
           }
       }
   } else {
       // blacks turn
       if ((t_pos.x == p_pos.x+1 || t_pos.x == p_pos.x-1) && t_pos.y == p_pos.y+1) {
           // try to go one tile
           if (getPiece(t_pos) == nullptr) {
               return true;
           }
       } else if ((t_pos.x == p_pos.x-2) && (t_pos.y == p_pos.y+2)) {
           // try to jump a Piece to the left
           if (getPiece(Position{p_pos.x-1, p_pos.y+1}) != nullptr) {
               if (getPiece(t_pos) == nullptr && getPiece(Position{p_pos.x-1, p_pos.y+1})->get_is_red()) {
                    // black single jumps red
                    players_[0]->removePiece(Position{p_pos.x-1, p_pos.y+1});
                    emit updatePiecesLabel(true, players_[0]->get_num_pieces());
                    return true;
               }
           }
       } else if ((t_pos.x == p_pos.x+2) && (t_pos.y == p_pos.y+2)) {
           // try to jump a Piece to the right
           // check theres a piece to jump
           if (getPiece(Position{p_pos.x+1, p_pos.y+1}) != nullptr) {
               // check the tile we want to go to is open, and that the Piece to jump is correct color
               if (getPiece(t_pos) == nullptr && getPiece(Position{p_pos.x+1, p_pos.y+1})->get_is_red()) {
                    // black single jumps red
                    players_[0]->removePiece(Position{p_pos.x+1, p_pos.y+1});
                    emit updatePiecesLabel(true, players_[0]->get_num_pieces());
                    return true;
               }
           }
       }
   }

   return false;
}

// helper for when tile is selected
void GameBoard::handleSelected(Tile* t, PiecePrototype* p, bool red) {
    if (checkValidity(t, p)) {
        PieceType pt = p->get_type();
        PiecePrototype* newPiece = factory_->CreatePiece(pt, t->get_position(), red);
        connect(newPiece, SIGNAL(gotSelected()), this, SLOT(pieceSelected()));
        if (red) {
            players_[0]->removePiece(p->get_position());
            players_[0]->addPiece(newPiece);
            current_player_ = 1;
        } else {
            players_[1]->removePiece(p->get_position());
            players_[1]->addPiece(newPiece);
            current_player_ = 0;
        }
        emit updateTurnLabel(current_player_);
        emit addPiece(newPiece);
    }
}

// slot for when a tile gets selected
void GameBoard::tileSelected(Tile* t) {
    // first make sure we have a Piece selected, and that the tiles black
    if (getSelectedPiece() != nullptr && !t->get_is_red()) {
        PiecePrototype* p = getSelectedPiece();
        bool p_red = p->get_is_red();
        // check that the selected piece is the same color as current player
        if ((p_red && current_player_ == 0) || (!p_red && current_player_ == 1)) {
            handleSelected(t, p, p_red);
        }
    // do nothing if checks dont pass
    }
}

// when a piece is selected, deselect all other pieces
void GameBoard::pieceSelected() {
    deselectPiece();
}

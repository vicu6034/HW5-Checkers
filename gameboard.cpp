#include "gameboard.h"

GameBoard::GameBoard()
{
    selected_ = nullptr;
    current_player_ = 0;
    // here we will set up the starting state of the game

    // instantiate initial Tiles and Pieces
    std::vector<PiecePrototype*> red_pieces;
    std::vector<PiecePrototype*> black_pieces;
    bool switcher = false;
    for (int i = 0; i < 10; i++) {
        // create pieces
        if (i % 2 == 0) {
            PiecePrototype* b1 = factory_->CreatePiece(PieceType::RegularPiece, Position{i,0}, false);
            PiecePrototype* b2 = factory_->CreatePiece(PieceType::RegularPiece, Position{i,2}, false);
            PiecePrototype* r = factory_->CreatePiece(PieceType::RegularPiece, Position{i,8}, true);
            connect(b1, SIGNAL(gotSelected(PiecePrototype*)), this, SLOT(pieceSelected(PiecePrototype*)));
            connect(b2, SIGNAL(gotSelected(PiecePrototype*)), this, SLOT(pieceSelected(PiecePrototype*)));
            connect(r, SIGNAL(gotSelected(PiecePrototype*)), this, SLOT(pieceSelected(PiecePrototype*)));
            black_pieces.push_back(b1);
            black_pieces.push_back(b2);
            red_pieces.push_back(r);
        } else {
            PiecePrototype* b = factory_->CreatePiece(PieceType::RegularPiece, Position{i,1}, false);
            PiecePrototype* r1 = factory_->CreatePiece(PieceType::RegularPiece, Position{i,7}, true);
            PiecePrototype* r2 = factory_->CreatePiece(PieceType::RegularPiece, Position{i,9}, true);
            connect(b, SIGNAL(gotSelected(PiecePrototype*)), this, SLOT(pieceSelected(PiecePrototype*)));
            connect(r1, SIGNAL(gotSelected(PiecePrototype*)), this, SLOT(pieceSelected(PiecePrototype*)));
            connect(r2, SIGNAL(gotSelected(PiecePrototype*)), this, SLOT(pieceSelected(PiecePrototype*)));
            black_pieces.push_back(b);
            red_pieces.push_back(r1);
            red_pieces.push_back(r2);
        }

        // create tiles
        for (int j = 0; j < 10; j++) {
            Tile* tile = new Tile(Position{i,j}, switcher);
            connect(tile, SIGNAL(gotSelected(Tile*)), this, SLOT(tileSelected(Tile*)));
            tiles_.push_back(tile);
            switcher = !switcher;
        }
        switcher = !switcher;

    }

    // create players with their initial pieces
    players_.push_back(new Player(true, red_pieces));
    players_.push_back(new Player(false, black_pieces));

    // add powerups to the game
    // PowerUp(pos, is_addPiece)
    powerups_.push_back(new PowerUp(Position{4,4}, true));
    powerups_.push_back(new PowerUp(Position{5,5}, false));

}

void GameBoard::Reset() {
    current_player_ = 0;
    std::vector<PiecePrototype*> red_pieces;
    std::vector<PiecePrototype*> black_pieces;
    for (int i = 0; i < 10; i++) {
        // create pieces
        if (i % 2 == 0) {
            PiecePrototype* b1 = factory_->CreatePiece(PieceType::RegularPiece, Position{i,0}, false);
            PiecePrototype* b2 = factory_->CreatePiece(PieceType::RegularPiece, Position{i,2}, false);
            PiecePrototype* r = factory_->CreatePiece(PieceType::RegularPiece, Position{i,8}, true);
            connect(b1, SIGNAL(gotSelected()), this, SLOT(pieceSelected()));
            connect(b2, SIGNAL(gotSelected()), this, SLOT(pieceSelected()));
            connect(r, SIGNAL(gotSelected()), this, SLOT(pieceSelected()));
            black_pieces.push_back(b1);
            black_pieces.push_back(b2);
            red_pieces.push_back(r);
        } else {
            // odd black pieces
            PiecePrototype* b = factory_->CreatePiece(PieceType::RegularPiece, Position{i,1}, false);
            connect(b, SIGNAL(gotSelected()), this, SLOT(pieceSelected()));
            black_pieces.push_back(b);
            // odd red pieces
            PiecePrototype* r1 = factory_->CreatePiece(PieceType::RegularPiece, Position{i,7}, true);
            PiecePrototype* r2 = factory_->CreatePiece(PieceType::RegularPiece, Position{i,9}, true);
            connect(r1, SIGNAL(gotSelected()), this, SLOT(pieceSelected()));
            connect(r2, SIGNAL(gotSelected()), this, SLOT(pieceSelected()));
            red_pieces.push_back(r1);
            red_pieces.push_back(r2);
        }
     }
     players_[0]->set_pieces(red_pieces);
     players_[1]->set_pieces(black_pieces);
}

// method to return all pieces
std::vector<PiecePrototype*> GameBoard::getPieces() {
    std::vector<PiecePrototype*> vec1 = players_[0]->get_pieces();
    std::vector<PiecePrototype*> vec2 = players_[1]->get_pieces();
    vec1.insert(vec1.end(), vec2.begin(), vec2.end());
    return vec1;
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

bool GameBoard::checkValidity(Tile* t, PiecePrototype*p) {
   Position p_pos = p->get_position();
   Position t_pos = t->get_position();

   if (p->get_is_red()) {
       // reds turn
       if ((t_pos.x == p_pos.x+1 || t_pos.x == p_pos.x-1) && t_pos.y == p_pos.y-1) {
           // try to go one space
           return true;
       } else if (t_pos.x == p_pos.x-2 && t_pos.y == p_pos.y-2) {
           // try to jump piece to left
           PiecePrototype * p = getPiece(Position{p_pos.x-1, p_pos.y-1});
           if (p != nullptr) {
               if (!p->get_is_red()) {
                   players_[1]->removePiece(Position{p_pos.x-1, p_pos.y-1});
                   emit updatePiecesLabel(false, players_[1]->get_num_pieces());
                   return true;
               }
           }
       } else if (t_pos.x == p_pos.x+2 && t_pos.y == p_pos.y-2) {
           // try to jump piece to right
           PiecePrototype * p = getPiece(Position{p_pos.x+1, p_pos.y-1});
           if (p != nullptr) {
               if (!p->get_is_red()) {
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
           return true;
       } else if ((t_pos.x == p_pos.x-2) && (t_pos.y == p_pos.y+2)) {
           // try to jump a Piece to the left
           // check piece exists
           PiecePrototype * p = getPiece(Position{p_pos.x-1, p_pos.y+1});
           if (p != nullptr) {
               if (p->get_is_red()) {
                   players_[0]->removePiece(Position{p_pos.x-1, p_pos.y+1});
                   emit updatePiecesLabel(true, players_[0]->get_num_pieces());
                   return true;
               }
           }
       } else if ((t_pos.x == p_pos.x+2) && (t_pos.y == p_pos.y+2)) {
           // try to jump a Piece to the right
           // check theres a piece to jump
           PiecePrototype * p = getPiece(Position{p_pos.x+1, p_pos.y+1});
           if (p != nullptr) {
               if (p->get_is_red()) {
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
        connect(newPiece, SIGNAL(gotSelected(PiecePrototype*)), this, SLOT(pieceSelected(PiecePrototype*)));
        players_[current_player_]->removePiece(p->get_position());
        players_[current_player_]->addPiece(newPiece);
        if (red) {
            current_player_ = 1;
        } else {
            current_player_ = 0;
        }
        emit updateTurnLabel(current_player_);
        emit addPiece(newPiece);
    }
}

// slot for when a tile gets selected
void GameBoard::tileSelected(Tile* t) {
    // first make sure we have a Piece selected
    if (getSelectedPiece() != nullptr) {
        PiecePrototype* p = getSelectedPiece();
        bool p_red = p->get_is_red();
        // check that the selected piece is the same color as current player
        // check that theres no piece at the tile we want to go to
        if (((p_red && current_player_ == 0) || (!p_red && current_player_ == 1)) && (getPiece(t->get_position()) == nullptr)) {
            handleSelected(t, p, p_red);
        }
    // do nothing if checks dont pass
    }
}

// when a piece is selected, deselect all other pieces
void GameBoard::pieceSelected(PiecePrototype* p) {
    if ((p->get_is_red() && !current_player_) || (!p->get_is_red() && current_player_)) {
        selected_ = p;
    }
}

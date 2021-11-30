#include "gameboard.h"

GameBoard::GameBoard() {
    // create tiles
    bool switcher = false;
    for (int i = 0; i < 10; i++) {
        for (int j = 0; j < 10; j++) {
            Tile* tile = new Tile(Position{i,j}, switcher);
            connect(tile, SIGNAL(gotSelected(Tile*)), this, SLOT(tileSelected(Tile*)));
            tiles_.push_back(tile);
            switcher = !switcher;
        }
        switcher = !switcher;
    }

    // create players
    players_.push_back(new Player(true));
    players_.push_back(new Player(false));

    // create pieces
    NewGame();
}

// sets pieces up for a new game
void GameBoard::NewGame() {
    selected_ = nullptr;
    current_player_ = 0;
    std::vector<PiecePrototype*> red_pieces;
    std::vector<PiecePrototype*> black_pieces;

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
            // odd black pieces
            PiecePrototype* b = factory_->CreatePiece(PieceType::RegularPiece, Position{i,1}, false);
            connect(b, SIGNAL(gotSelected(PiecePrototype*)), this, SLOT(pieceSelected(PiecePrototype*)));
            black_pieces.push_back(b);
            // odd red pieces
            PiecePrototype* r1 = factory_->CreatePiece(PieceType::RegularPiece, Position{i,7}, true);
            PiecePrototype* r2 = factory_->CreatePiece(PieceType::RegularPiece, Position{i,9}, true);
            connect(r1, SIGNAL(gotSelected(PiecePrototype*)), this, SLOT(pieceSelected(PiecePrototype*)));
            connect(r2, SIGNAL(gotSelected(PiecePrototype*)), this, SLOT(pieceSelected(PiecePrototype*)));
            red_pieces.push_back(r1);
            red_pieces.push_back(r2);
        }
     }
     players_[0]->set_pieces(red_pieces);
     players_[1]->set_pieces(black_pieces);

     // add powerups to the game
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

PiecePrototype* GameBoard::getPiece(Position pos) {
    // get all pieces
    for (PiecePrototype* p : getPieces()) {
        // if ones selected, return it
        if (p->get_position() == pos) return p;
    }
    // if no pieces selected, return null
    return nullptr;
}

// return true for valid jump, false otherwise
bool GameBoard::jumpHelper(Position pos, bool red) {
    // get the piece we want to jump
    PiecePrototype * p = getPiece(pos);
    // check the piece exists
    if (p != nullptr) {
        // check the piece to jump is opposite color of the selected piece
        if (red != p->get_is_red()) {
            // remove jumped piece from other player, update pieces label
            players_[!current_player_]->removePiece(pos);
            emit updatePiecesLabel(!red, players_[!current_player_]->get_num_pieces());
            return true;
        }
    }
    return false;
}

// check all the tiles a regular piece could move to
bool GameBoard::checkRegularMoves(Position t_pos, Position s_pos, bool red) {
    if (red) {
        // reds turn
        if ((t_pos.x == s_pos.x+1 || t_pos.x == s_pos.x-1) && t_pos.y == s_pos.y-1) {
            // try to go one space
            return true;
        } else if (t_pos.x == s_pos.x-2 && t_pos.y == s_pos.y-2) {
            // try to jump piece to left
            return jumpHelper(Position{s_pos.x-1, s_pos.y-1}, red);
        } else if (t_pos.x == s_pos.x+2 && t_pos.y == s_pos.y-2) {
            // try to jump piece to right
            return jumpHelper(Position{s_pos.x+1, s_pos.y-1}, red);
        }
    } else {
        // blacks turn
        if ((t_pos.x == s_pos.x+1 || t_pos.x == s_pos.x-1) && t_pos.y == s_pos.y+1) {
            // try to go one tile
            return true;
        } else if ((t_pos.x == s_pos.x-2) && (t_pos.y == s_pos.y+2)) {
            // try to jump a Piece to the left
            return jumpHelper(Position{s_pos.x-1, s_pos.y+1}, red);
        } else if ((t_pos.x == s_pos.x+2) && (t_pos.y == s_pos.y+2)) {
            // try to jump a Piece to the right
            return jumpHelper(Position{s_pos.x+1, s_pos.y+1}, red);
        }
    }
    return false;
}

// check the tiles a king could move to
bool GameBoard::checkKingMoves(Position t_pos, Position s_pos, bool red) {
    if (red) {
        // reds turn
        if ((t_pos.x == s_pos.x+1 || t_pos.x == s_pos.x-1) && t_pos.y == s_pos.y+1) {
            // try to go one tile backwards
            return true;
        } else if (t_pos.x == s_pos.x-2 && t_pos.y == s_pos.y+2) {
            // try to jump piece to left backwards
            return jumpHelper(Position{s_pos.x-1, s_pos.y+1}, red);
        } else if (t_pos.x == s_pos.x+2 && t_pos.y == s_pos.y+2) {
            // try to jump piece to right backwards
            return jumpHelper(Position{s_pos.x+1, s_pos.y+1}, red);
        }
    } else {
        // blacks turn
        if ((t_pos.x == s_pos.x+1 || t_pos.x == s_pos.x-1) && t_pos.y == s_pos.y-1) {
            // try to go one tile backwards
            return true;
        } else if ((t_pos.x == s_pos.x-2) && (t_pos.y == s_pos.y-2)) {
            // try to jump a Piece to the left backwards
            return jumpHelper(Position{s_pos.x-1, s_pos.y-1}, red);
        } else if ((t_pos.x == s_pos.x+2) && (t_pos.y == s_pos.y-2)) {
            // try to jump a Piece to the right backwards
            return jumpHelper(Position{s_pos.x+1, s_pos.y-1}, red);
        }
    }
    return false;
}

bool GameBoard::friendlyJumpHelper(Position pos, bool red) {
    // get the piece we want to jump
    PiecePrototype * p = getPiece(pos);
    // check the piece exists
    if (p != nullptr) {
        // check the piece to jump is the same color of the selected piece
        if (red == p->get_is_red()) {
            return true;
        }
    }
    return false;
}

bool GameBoard::doubleJumpHelper(Position pos1, Position pos2, bool red) {
    // get the piece we want to jump
    PiecePrototype * p = getPiece(pos1);
    PiecePrototype * p2 = getPiece(pos2);
    // check the piece exists
    if ((p != nullptr) && (p2 != nullptr)) {
        // check the piece to jump is opposite color of the selected piece
        if ((red != p->get_is_red()) && (red != p2->get_is_red())) {
            // remove jumped piece from other player, update pieces label
            players_[!current_player_]->removePiece(pos1);
            players_[!current_player_]->removePiece(pos2);
            emit updatePiecesLabel(!red, players_[!current_player_]->get_num_pieces());
            return true;
        }
    }
    return false;
}

// check tiles triple king could move to
bool GameBoard::checkTripleKMoves(Position t_pos, Position s_pos, bool red) {
    if ((t_pos.x == s_pos.x+2) && (t_pos.y == s_pos.y+2)) {
        // jump a friendly piece bottom right
        return friendlyJumpHelper(Position{s_pos.x+1, s_pos.y+1}, red);

    } else if ((t_pos.x == s_pos.x-2) && (t_pos.y == s_pos.y+2)) {
        // jump a friendly piece bottom left
        return friendlyJumpHelper(Position{s_pos.x-1, s_pos.y+1}, red);

    } else if ((t_pos.x == s_pos.x+2) && (t_pos.y == s_pos.y-2)) {
        // jump a friendly piece top right
        return friendlyJumpHelper(Position{s_pos.x+1, s_pos.y-1}, red);

    } else if ((t_pos.x == s_pos.x-2) && (t_pos.y == s_pos.y-2)) {
        // jump a friendly piece top left
        return friendlyJumpHelper(Position{s_pos.x-1, s_pos.y-1}, red);

    } else if ((t_pos.x == s_pos.x+3) && (t_pos.y == s_pos.y+3)) {
        // jump 2 enemy pieces bottom right
        return doubleJumpHelper(Position{s_pos.x+1, s_pos.y+1}, Position{s_pos.x+2, s_pos.y+2}, red);

    } else if ((t_pos.x == s_pos.x-3) && (t_pos.y == s_pos.y+3)) {
        // jump 2 enemy pieces bottom left
        return doubleJumpHelper(Position{s_pos.x-1, s_pos.y+1}, Position{s_pos.x-2, s_pos.y+2}, red);

    } else if ((t_pos.x == s_pos.x+3) && (t_pos.y == s_pos.y-3)) {
        // jump 2 enemy pieces top right
        return doubleJumpHelper(Position{s_pos.x+1, s_pos.y-1}, Position{s_pos.x+2, s_pos.y-2}, red);

    } else if ((t_pos.x == s_pos.x-3) && (t_pos.y == s_pos.y-3)) {
        // jump 2 enemy pieces top left
        return doubleJumpHelper(Position{s_pos.x-1, s_pos.y-1}, Position{s_pos.x-2, s_pos.y-2}, red);

    }

    return false;
}

// return true for valid move, false otherwise
bool GameBoard::checkValidity(Tile* t, bool red) {
   Position s_pos = selected_->get_position();
   Position t_pos = t->get_position();

   switch (selected_->get_type()) {
        case PieceType::RegularPiece:
            return checkRegularMoves(t_pos, s_pos, red);
        case PieceType::KingPiece:
            if (checkRegularMoves(t_pos, s_pos, red)) {
                return true;
            } else {
                return checkKingMoves(t_pos, s_pos, red);
            }
        case PieceType::TripleKingPiece:
           if (checkRegularMoves(t_pos, s_pos, red)) {
               return true;
           } else if (checkKingMoves(t_pos, s_pos, red)) {
               return true;
           } else {
               return checkTripleKMoves(t_pos, s_pos, red);
           }
   }
}

std::string GameBoard::checkPowerup(Position pos) {
    for (PowerUp* powerup : powerups_ ) {
        if (powerup->get_position() == pos) {
            return powerup->get_type();
        }
    }
    return "none";
}

void GameBoard::removePowerup(Position pos) {
    for (unsigned int i = 0 ; i < powerups_.size(); i++) {
        if (powerups_[i]->get_position() == pos) {
            delete powerups_[i];
            powerups_.erase(powerups_.begin() + i);
        }
    }
}

void GameBoard::handlePowerup(Position t_pos, Position last_pos, bool red) {
    // check for landing on powerups
    if (checkPowerup(t_pos) == "add") {
        removePowerup(t_pos);
        // add a piece to current player
        // create piece with new type, connect it and add to scene
        PiecePrototype* p = factory_->CreatePiece(PieceType::RegularPiece, last_pos, red);
        players_[current_player_]->addPiece(p);
        connect(p, SIGNAL(gotSelected(PiecePrototype*)), this, SLOT(pieceSelected(PiecePrototype*)));
        emit addPiece(p);
        emit updatePiecesLabel(red, players_[current_player_]->get_num_pieces());
    } else if (checkPowerup(t_pos) == "level") {
        // level up piece that landed on the powerup
        if (selected_->get_type() == PieceType::RegularPiece) {
            // making a regular piece into a king
            players_[current_player_]->removePiece(last_pos);
            removePowerup(t_pos);
            // create piece with new type, connect it and add to scene
            PiecePrototype* p = factory_->CreatePiece(PieceType::KingPiece, t_pos, red);
            players_[current_player_]->addPiece(p);
            connect(p, SIGNAL(gotSelected(PiecePrototype*)), this, SLOT(pieceSelected(PiecePrototype*)));
            selected_ = p;
            emit addPiece(p);
        } else if (selected_->get_type() == PieceType::KingPiece) {
            players_[current_player_]->removePiece(last_pos);
            removePowerup(t_pos);
            // create piece with new type, connect it and add to scene
            PiecePrototype* p = factory_->CreatePiece(PieceType::TripleKingPiece, t_pos, red);
            players_[current_player_]->addPiece(p);
            connect(p, SIGNAL(gotSelected(PiecePrototype*)), this, SLOT(pieceSelected(PiecePrototype*)));
            selected_ = p;
            emit addPiece(p);
        }
    }
}

// helper for when tile is selected
void GameBoard::handleSelected(Tile* t, bool red) {
    if (checkValidity(t, red)) {
        Position last_pos = selected_->get_position();
        // check if we need piece upgrade after the move, if not just update the piece
        if (((t->get_position().y == 0) || (t->get_position().y == 9)) && selected_->get_type() == PieceType::RegularPiece) {
            // making a regular piece into a king
            players_[current_player_]->removePiece(last_pos);
            // create piece with new type, connect it and add to scene
            PiecePrototype* p = factory_->CreatePiece(PieceType::KingPiece, t->get_position(), red);
            players_[current_player_]->addPiece(p);
            connect(p, SIGNAL(gotSelected(PiecePrototype*)), this, SLOT(pieceSelected(PiecePrototype*)));
            selected_ = p;
            emit addPiece(p);
            handlePowerup(t->get_position(), last_pos, red);
        } else if (((t->get_position().y == 9) || (t->get_position().y == 0)) && selected_->get_type() == PieceType::KingPiece) {
            // making a king into a triple king
            players_[current_player_]->removePiece(last_pos);
            // create piece with new type, connect it and add to scene
            PiecePrototype* p = factory_->CreatePiece(PieceType::TripleKingPiece, t->get_position(), red);
            players_[current_player_]->addPiece(p);
            connect(p, SIGNAL(gotSelected(PiecePrototype*)), this, SLOT(pieceSelected(PiecePrototype*)));
            selected_ = p;
            emit addPiece(p);
            handlePowerup(t->get_position(), last_pos, red);
        } else {
            // if not changing the type just update piece
            selected_->set_position(t->get_position());
            players_[current_player_]->updatePiece(last_pos, t->get_position());
            selected_->set_highlighted(false);
            emit updatePiece(selected_);
            handlePowerup(t->get_position(), last_pos, red);
        }

        // iterate turn
        if (red) {
            current_player_ = 1;
        } else {
            current_player_ = 0;
        }
        // update turn label
        emit updateTurnLabel(current_player_);
    }
}

// slot for when a tile gets selected
void GameBoard::tileSelected(Tile* t) {
    // first make sure we have a Piece selected
    if (selected_ != nullptr) {
        bool p_red = selected_->get_is_red();
        // check that the selected piece is the same color as current player
        // check that theres no piece at the tile we want to go to
        if ((p_red != current_player_) && (getPiece(t->get_position()) == nullptr)) {
            handleSelected(t, p_red);
        }
    // do nothing if checks dont pass
    }
}

// slot for when a piece gets selected
void GameBoard::pieceSelected(PiecePrototype* p) {
    // if the piece is the color of the current player, select it
    if (p->get_is_red() != current_player_) {
        // unhighlight previously selected piece
        if (selected_ != nullptr) {
            selected_->set_highlighted(false);
            emit updatePiece(selected_);
        }

        selected_ = p;
        selected_->set_highlighted(true);
        emit updatePiece(selected_);
    }
}

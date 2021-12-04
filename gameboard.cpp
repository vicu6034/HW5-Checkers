#include "gameboard.h"

GameBoard::GameBoard() {
    difficulty_ = Difficulty::None;
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

     // remove powerups left from last game
     powerups_.clear();
     // add powerups to the game
     powerups_.push_back(new PowerUp(Position{4,4}, true));
     powerups_.push_back(new PowerUp(Position{5,5}, false));
}

// method to return all pieces
std::vector<PiecePrototype*> GameBoard::getPieces() const {
    // get both players pieces and return them all
    std::vector<PiecePrototype*> vec1 = players_[0]->get_pieces();
    std::vector<PiecePrototype*> vec2 = players_[1]->get_pieces();
    vec1.insert(vec1.end(), vec2.begin(), vec2.end());
    return vec1;
}

// get a piece by position
PiecePrototype* GameBoard::getPiece(Position pos) const {
    // get all pieces
    for (PiecePrototype* p : getPieces()) {
        // if ones selected, return it
        if (p->get_position() == pos) return p;
    }
    // if no pieces selected, return null
    return nullptr;
}

// check if someone won the game
int GameBoard::checkForWinner() {
    for (int i = 0; i < 2; i++) {
        // check if someone won the game
        if (players_[i]->get_num_pieces() == 0) {
            // return their index
            if (i == 0) {
                return 1;
            } else {
                return 0;
            }
        }
    }
    // if noone wins return -1
    return -1;
}

// return true for valid jump, false otherwise
int GameBoard::jumpHelper(Position pos, bool red, bool jump) {
    // get the piece we want to jump
    PiecePrototype * p = getPiece(pos);
    // check the piece exists
    if (p != nullptr) {
        // check the piece to jump is opposite color of the selected piece
        if (red != p->get_is_red()) {
            // remove jumped piece from other player, update pieces label
            if (jump) {
                players_[!current_player_]->removePiece(pos);
                emit updatePiecesLabel(!red, players_[!current_player_]->get_num_pieces());
                emit playJumpSound();
                return 2;
            } else {
                return 2;
            }

        }
    }
    return -1;
}

// helper for jumping over a friendly piece
int GameBoard::friendlyJumpHelper(Position pos, bool red, bool jump) {
    // get the piece we want to jump
    PiecePrototype * p = getPiece(pos);
    // check the piece exists
    if (p != nullptr) {
        // check the piece to jump is the same color of the selected piece
        if (red == p->get_is_red()) {
            if (jump) {
                emit playJumpSound();
                return 1;
            } else {
                return 1;
            }
        }
    }
    return -1;
}

// helper for jumping over 2 pieces at once
// this is TKings special move
int GameBoard::doubleJumpHelper(Position pos1, Position pos2, bool red, bool jump) {
    // get the piece we want to jump
    PiecePrototype * p = getPiece(pos1);
    PiecePrototype * p2 = getPiece(pos2);
    // check the piece exists
    if ((p != nullptr) && (p2 != nullptr)) {
        // check the piece to jump is opposite color of the selected piece
        if ((red != p->get_is_red()) && (red != p2->get_is_red())) {
            // remove jumped piece from other player, update pieces label
            if (jump) {
                players_[!current_player_]->removePiece(pos1);
                players_[!current_player_]->removePiece(pos2);
                emit updatePiecesLabel(!red, players_[!current_player_]->get_num_pieces());
                emit playJumpSound();
                return 4;
            } else {
                return 4;
            }

        }
    }
    return -1;
}

// check all the tiles a regular piece could move to
int GameBoard::checkRegularMoves(Position t_pos, Position s_pos, bool red, bool jump) {
    if (red) {
        // reds turn
        if ((t_pos.x == s_pos.x+1 || t_pos.x == s_pos.x-1) && t_pos.y == s_pos.y-1) {
            // try to go one space
            if (jump) {
                emit playSlideSound();
                return 1;
            } else {
                return 1;
            }
        } else if (t_pos.x == s_pos.x-2 && t_pos.y == s_pos.y-2) {
            // try to jump piece to left
            return jumpHelper(Position{s_pos.x-1, s_pos.y-1}, red, jump);
        } else if (t_pos.x == s_pos.x+2 && t_pos.y == s_pos.y-2) {
            // try to jump piece to right
            return jumpHelper(Position{s_pos.x+1, s_pos.y-1}, red, jump);
        }
    } else {
        // blacks turn
        if ((t_pos.x == s_pos.x+1 || t_pos.x == s_pos.x-1) && t_pos.y == s_pos.y+1) {
            // try to go one tile
            if (jump) {
                emit playSlideSound();
                return 1;
            } else {
                return 1;
            }
        } else if ((t_pos.x == s_pos.x-2) && (t_pos.y == s_pos.y+2)) {
            // try to jump a Piece to the left
            return jumpHelper(Position{s_pos.x-1, s_pos.y+1}, red, jump);
        } else if ((t_pos.x == s_pos.x+2) && (t_pos.y == s_pos.y+2)) {
            // try to jump a Piece to the right
            return jumpHelper(Position{s_pos.x+1, s_pos.y+1}, red, jump);
        }
    }
    return -1;
}

// check the tiles a king could move to
int GameBoard::checkKingMoves(Position t_pos, Position s_pos, bool red, bool jump) {
    if (red) {
        // reds turn
        if ((t_pos.x == s_pos.x+1 || t_pos.x == s_pos.x-1) && t_pos.y == s_pos.y+1) {
            // try to go one tile backwards
            if (jump) {
                emit playSlideSound();
                return 1;
            } else {
                return 1;
            }
        } else if (t_pos.x == s_pos.x-2 && t_pos.y == s_pos.y+2) {
            // try to jump piece to left backwards
            return jumpHelper(Position{s_pos.x-1, s_pos.y+1}, red, jump);
        } else if (t_pos.x == s_pos.x+2 && t_pos.y == s_pos.y+2) {
            // try to jump piece to right backwards
            return jumpHelper(Position{s_pos.x+1, s_pos.y+1}, red, jump);
        }
    } else {
        // blacks turn
        if ((t_pos.x == s_pos.x+1 || t_pos.x == s_pos.x-1) && t_pos.y == s_pos.y-1) {
            // try to go one tile backwards
            if (jump) {
                emit playSlideSound();
                return 1;
            } else {
                return 1;
            }
        } else if ((t_pos.x == s_pos.x-2) && (t_pos.y == s_pos.y-2)) {
            // try to jump a Piece to the left backwards
            return jumpHelper(Position{s_pos.x-1, s_pos.y-1}, red, jump);
        } else if ((t_pos.x == s_pos.x+2) && (t_pos.y == s_pos.y-2)) {
            // try to jump a Piece to the right backwards
            return jumpHelper(Position{s_pos.x+1, s_pos.y-1}, red, jump);
        }
    }
    return -1;
}

// check tiles triple king could move to
int GameBoard::checkTripleKMoves(Position t_pos, Position s_pos, bool red, bool jump) {
    if ((t_pos.x == s_pos.x+2) && (t_pos.y == s_pos.y+2)) {
        // jump a friendly piece bottom right
        return friendlyJumpHelper(Position{s_pos.x+1, s_pos.y+1}, red, jump);
    } else if ((t_pos.x == s_pos.x-2) && (t_pos.y == s_pos.y+2)) {
        // jump a friendly piece bottom left
        return friendlyJumpHelper(Position{s_pos.x-1, s_pos.y+1}, red, jump);
    } else if ((t_pos.x == s_pos.x+2) && (t_pos.y == s_pos.y-2)) {
        // jump a friendly piece top right
        return friendlyJumpHelper(Position{s_pos.x+1, s_pos.y-1}, red, jump);
    } else if ((t_pos.x == s_pos.x-2) && (t_pos.y == s_pos.y-2)) {
        // jump a friendly piece top left
        return friendlyJumpHelper(Position{s_pos.x-1, s_pos.y-1}, red, jump);
    } else if ((t_pos.x == s_pos.x+3) && (t_pos.y == s_pos.y+3)) {
        // jump 2 enemy pieces bottom right
        return doubleJumpHelper(Position{s_pos.x+1, s_pos.y+1}, Position{s_pos.x+2, s_pos.y+2}, red, jump);
    } else if ((t_pos.x == s_pos.x-3) && (t_pos.y == s_pos.y+3)) {
        // jump 2 enemy pieces bottom left
        return doubleJumpHelper(Position{s_pos.x-1, s_pos.y+1}, Position{s_pos.x-2, s_pos.y+2}, red, jump);
    } else if ((t_pos.x == s_pos.x+3) && (t_pos.y == s_pos.y-3)) {
        // jump 2 enemy pieces top right
        return doubleJumpHelper(Position{s_pos.x+1, s_pos.y-1}, Position{s_pos.x+2, s_pos.y-2}, red, jump);
    } else if ((t_pos.x == s_pos.x-3) && (t_pos.y == s_pos.y-3)) {
        // jump 2 enemy pieces top left
        return doubleJumpHelper(Position{s_pos.x-1, s_pos.y-1}, Position{s_pos.x-2, s_pos.y-2}, red, jump);
    } else {
        return -1;
    }
}

// return true for valid move, false otherwise
// use jump to toggle wether the jumps should actually occur
// ie if we're just checking for a move or actually moving
// -1 for invalid, 0 for get jumped, 1 for nothing, 2 for jump, 4 for double jump
int GameBoard::checkValidity(Tile* t, PiecePrototype* p, bool red, bool jump) {
   Position s_pos = p->get_position();
   Position t_pos = t->get_position();

   if (!getPiece(t_pos)) {
       switch (p->get_type()) {
            case PieceType::RegularPiece:
                return checkRegularMoves(t_pos, s_pos, red, jump);
            case PieceType::KingPiece:
                if (checkRegularMoves(t_pos, s_pos, red, false) != -1) {
                    return checkRegularMoves(t_pos, s_pos, red, jump);
                } else {
                    return checkKingMoves(t_pos, s_pos, red, jump);
                }
            case PieceType::TripleKingPiece:
               if (checkRegularMoves(t_pos, s_pos, red, false) != -1) {
                   return checkRegularMoves(t_pos, s_pos, red, jump);
               } else if (checkKingMoves(t_pos, s_pos, red, false) != -1)  {
                   return checkKingMoves(t_pos, s_pos, red, jump);
               } else {
                   return checkTripleKMoves(t_pos, s_pos, red, jump);
               }
       }
   } else {
       return -1;
   }
}

// return the name of a powerup if theres one at the given position
std::string GameBoard::checkPowerup(Position pos) {
    for (PowerUp* powerup : powerups_ ) {
        if (powerup->get_position() == pos) {
            return powerup->get_type();
        }
    }
    return "none";
}

// remove a powerup from the board
void GameBoard::removePowerup(Position pos) {
    for (unsigned int i = 0 ; i < powerups_.size(); i++) {
        if (powerups_[i]->get_position() == pos) {
            delete powerups_[i];
            powerups_.erase(powerups_.begin() + i);
        }
    }
}

Position GameBoard::findPosForPowerup(bool red) {
    if (!red) {
        for (int j = 0; j < 10; j++) {
            for (int i = 0; i < 10; i++) {
                if (!getPiece(Position{i,j}) && (((i % 2 != 0) && (j % 2 != 0)) || ((i % 2 == 0) && (j % 2 == 0)))) {
                    return Position{i,j};
                }
            }
        }
    } else {
        for (int i = 9; i >= 0; i--) {
            for (int j = 9; j >= 0; j--) {
                if (!getPiece(Position{j,i}) && (((i % 2 != 0) && (j % 2 != 0)) || ((i % 2 == 0) && (j % 2 == 0)))) {
                    return Position{j, i};
                }
            }
        }
    }
    return Position{100,100};
}

// check if theres a powerupo at the landed on position and handle it if so
void GameBoard::handlePowerup(Position t_pos, Position last_pos, bool red) {
    // check for landing on powerups
    if (checkPowerup(t_pos) == "add") {
        removePowerup(t_pos);
        // add a piece to current player
        // create piece with new type, connect it and add to scene
        PiecePrototype* p = factory_->CreatePiece(PieceType::RegularPiece, findPosForPowerup(red), red);
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
    if (checkValidity(t, selected_, red, true) != -1) {
        Position last_pos = selected_->get_position();
        // check if we need piece upgrade after the move, if not just update the piece
        if (((t->get_position().y == 0 && red) || (t->get_position().y == 9 && !red)) && selected_->get_type() == PieceType::RegularPiece) {
            // making a regular piece into a king
            players_[current_player_]->removePiece(last_pos);
            // create piece with new type, connect it and add to scene
            PiecePrototype* p = factory_->CreatePiece(PieceType::KingPiece, t->get_position(), red);
            players_[current_player_]->addPiece(p);
            connect(p, SIGNAL(gotSelected(PiecePrototype*)), this, SLOT(pieceSelected(PiecePrototype*)));
            selected_ = p;
            emit addPiece(p);
            handlePowerup(t->get_position(), last_pos, red);
        } else if (((t->get_position().y == 9 && red) || (t->get_position().y == 0 && !red)) && selected_->get_type() == PieceType::KingPiece) {
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
        int winner = checkForWinner();
        if (winner == -1) {
            if (red) {
                current_player_ = 1;
            } else {
                current_player_ = 0;
            }
            // update turn label
            emit updateTurnLabel(current_player_);
        } else {
            players_[winner]->set_num_wins(players_[winner]->get_num_wins()+1);
            emit gameOver(winner);
        }
    } else {
        emit playDeniedSound();
    }
}

// slot for when a tile gets selected
void GameBoard::tileSelected(Tile* t) {
    // first make sure we have a Piece selected
    if (selected_) {
        bool p_red = selected_->get_is_red();
        // check that the selected piece is the same color as current player
        // check that theres no piece at the tile we want to go to
        if ((p_red != current_player_) && (getPiece(t->get_position()) == nullptr)) {
            handleSelected(t, p_red);
        }
    } else {
        emit playDeniedSound();
    }
}

// slot for when a piece gets selected
void GameBoard::pieceSelected(PiecePrototype* p) {
    // if the piece is the color of the current player, select it
    // and its not the computers turn
    if ((p->get_is_red() != current_player_) && !(difficulty_ != Difficulty::None && current_player_ == 1)) {
        // unhighlight previously selected piece
        if (selected_ != nullptr) {
            selected_->set_highlighted(false);
            emit updatePiece(selected_);
        }

        selected_ = p;
        selected_->set_highlighted(true);
        emit updatePiece(selected_);
    } else {
        emit playDeniedSound();
    }
}

// get tiles a piece could move to
std::vector<Tile*> GameBoard::getPieceMoves(PiecePrototype* p) {
    std::vector<Tile*> valid_tiles;
    for (Tile* t : tiles_) {
        if (checkValidity(t, p, false, false) != -1) {
            valid_tiles.push_back(t);
        }
    }
    return valid_tiles;
}

// handle the timeout from the AI timer to make the AIs move if its their turn
void GameBoard::AI_Timer_slot() {
    if (current_player_ == 1) {
        // find pieces with valid moves
        std::vector<PiecePrototype*> valid_pieces;
        for (PiecePrototype* piece : players_[1]->get_pieces()) {
            if (getPieceMoves(piece).size()) {
                valid_pieces.push_back(piece);
            }
        }

        if (valid_pieces.size() == 0) {
            // if no pieces have moves they lose
            emit gameOver(0);
        } else {
            // pick move based on difficulty
            if (difficulty_ == Difficulty::Easy) {
                // pick a random move
                int p_i = arc4random()%valid_pieces.size();
                //pieceSelected(valid_pieces[p_i]);
                selected_ = valid_pieces[p_i];
                // get tiles to move to
                std::vector<Tile*> valid_tiles = getPieceMoves(valid_pieces[p_i]);
                int t_i = arc4random()%valid_tiles.size();
                tileSelected(valid_tiles[t_i]);
            } else if (difficulty_ == Difficulty::Medium) {
                // pick the best move for that turn
                std::vector<Move> moves;
                for (PiecePrototype* piece : valid_pieces) {
                    for (Tile* tile : tiles_) {
                        if (checkValidity(tile, piece, false, false) != -1) {
                            moves.push_back(Move{piece, tile, checkValidity(tile, piece, false, false)});
                        }
                    }
                }
                std::vector<Move> jump_moves;
                std::vector<Move> dbl_jump_moves;
                for (Move m : moves) {
                    if (m.score_ == 2) {
                        jump_moves.push_back(m);
                    } else if (m.score_ == 4) {
                        dbl_jump_moves.push_back(m);
                    }
                }
                if (dbl_jump_moves.size() > 0) {
                    selected_ = dbl_jump_moves[0].piece_;
                    tileSelected(dbl_jump_moves[0].tile_);
                } else if (jump_moves.size() > 0) {
                    selected_ = jump_moves[0].piece_;
                    tileSelected(jump_moves[0].tile_);
                } else {

                    int m_i = arc4random()%moves.size();
                    selected_ = moves[m_i].piece_;
                    tileSelected(moves[m_i].tile_);
                }
            }
        }
    }
}

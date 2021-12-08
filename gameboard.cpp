#include "gameboard.h"

#include <QThread>

#include <tile.h>
#include <pieceprototype.h>
#include <regularpiece.h>
#include <kingpiece.h>
#include <triplekingpiece.h>
#include <player.h>
#include <powerup.h>

/* Initialize the game board with tiles and players
 * Set up initial game state
*/
GameBoard::GameBoard() {
    // set up timers
    red_timer_ = new QTimer(this);
    connect(red_timer_, SIGNAL(timeout()), this, SLOT(red_Timer_slot()));
    black_timer_ = new QTimer(this);
    connect(black_timer_, SIGNAL(timeout()), this, SLOT(black_Timer_slot()));

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

/* Reset game to initial game state
 * Reset pieces, powerups, and set game info
*/
void GameBoard::NewGame() {
    // make no piece selected and reset current player
    selected_ = nullptr;
    current_player_ = 0;
    std::vector<PiecePrototype*> red_pieces;
    std::vector<PiecePrototype*> black_pieces;

    // create starting set of pieces
    for (int i = 0; i < 10; i++) {
        // rows go back and forth between how to line up
        if (i % 2 == 0) {
            // add pieces to even rows
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
            // add pieces for odd rows
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

     // add powerups to random positions between middle two rows
     int rando = arc4random_uniform(2);

     int a = arc4random_uniform(5) * 2;
     int b = arc4random_uniform(5) * 2 + 1;

     powerups_.push_back(new PowerUp(Position{a,4}, rando));
     powerups_.push_back(new PowerUp(Position{b,5}, !rando));

     if (difficulty_ == Difficulty::Simulation) {
         // for simulations play both AIs
         red_timer_->start(300);
         black_timer_->start(500);
     } else if (difficulty_ != Difficulty::None) {
         // for single player stop red AI and start black
         red_timer_->stop();
         black_timer_->start(3000);
     } else { // Difficulty == None
         // stop both timers in multiplayer
         red_timer_->stop();
         black_timer_->stop();
     }
}

/* Get all pieces in the game
 * @return vec<Piece*> all pieces
*/
std::vector<PiecePrototype*> GameBoard::get_pieces() const {
    // get both players pieces and return them all
    std::vector<PiecePrototype*> vec1 = players_[0]->get_pieces();
    std::vector<PiecePrototype*> vec2 = players_[1]->get_pieces();
    vec1.insert(vec1.end(), vec2.begin(), vec2.end());
    return vec1;
}

/* Get a piece by position, nullptr if none
 * @param Position to search for
 * @return Piece* if found at that Position
*/
PiecePrototype* GameBoard::get_piece(Position pos) const {
    // get all pieces
    for (PiecePrototype* p : get_pieces()) {
        // if ones selected, return it
        if (p->get_position() == pos) return p;
    }
    // if no pieces selected, return null
    return nullptr;
}

/* Check if someone won the game
 * @return int winner of game or -1 for none
*/
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

/* See if you can jump over an enemy
 * @param Position of piece we want to jump
 * @param bool red : color of piece thats jumping
 * @param bool jump : if we're actually jumping or just doing a check
 * @return int score of the move (-1 for invalid, 2 for a jump)
*/
int GameBoard::jumpHelper(Position pos, bool red, bool jump) {
    // get the piece we want to jump
    PiecePrototype * p = get_piece(pos);
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

/* See if you can jump over a friendly piece
 * @param Position we want to jump over
 * @param bool red : color of piece thats jumping
 * @param bool jump : if we're actually jumping or just doing a check
 * @return int score of the move (-1 for invalid, 1 for a jump)
*/
int GameBoard::friendlyJumpHelper(Position pos, bool red, bool jump) {
    // get the piece we want to jump
    PiecePrototype * p = get_piece(pos);
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

/* See if you can jump over a friendly piece
 * @param Position pos1 : position of first piece in the jump
 * @param Position pos2 : position of second piece in the jump
 * @param bool red : color of piece thats jumping
 * @param bool jump : if we're actually jumping or just doing a check
 * @return int score of the move (-1 for invalid, 4 for a jump)
*/
int GameBoard::doubleJumpHelper(Position pos1, Position pos2, bool red, bool jump) {
    // get the piece we want to jump
    PiecePrototype * p = get_piece(pos1);
    PiecePrototype * p2 = get_piece(pos2);
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

/* See if a piece will be vulnerable to getting jumped
 * @param Position t_pos : tile we want to move to
 * @param Position s_pos : position piece is moving from
 * @param bool red : color of piece that wants to move
 * @return int score of the move (-1 for invalid, 0 for able to be jumped)
*/
int GameBoard::couldGetJumped(Position t_pos, Position s_pos, bool red) {
    // cant get jumped on edge tiles
    if ((t_pos.x == 0) || (t_pos.x == 9)) {
        return 1;
    }
    if (red) {
        PiecePrototype * p = get_piece(Position{t_pos.x+1, t_pos.y-1});
        PiecePrototype * p2 = get_piece(Position{t_pos.x-1, t_pos.y-1});
        // check the piece exists
        if (p != nullptr) {
            // check the piece that could jump is the opposite color of the selected piece
            if (red != p->get_is_red()) {
                // see if theres a piece blocking the jump
                if ((get_piece(Position{t_pos.x-1, t_pos.y+1}) == nullptr) || (s_pos == Position{t_pos.x-1, t_pos.y+1})) {
                    return 0;
                }
            }
        } else if (p2 != nullptr) {
            // check the piece that could jump is the opposite color of the selected piece
            if (red != p2->get_is_red()) {
                // see if theres a piece blocking the jump
                if ((get_piece(Position{t_pos.x+1, t_pos.y+1}) == nullptr) || (s_pos == Position{t_pos.x+1, t_pos.y+1})) {
                    return 0;
                }
            }
        }
    } else {
        PiecePrototype * p = get_piece(Position{t_pos.x+1, t_pos.y+1});
        PiecePrototype * p2 = get_piece(Position{t_pos.x-1, t_pos.y+1});
        // check the piece exists
        if (p != nullptr) {
            // check the piece that could jump is the opposite color of the selected piece
            if (red != p->get_is_red()) {
                // make sure a piece isnt blocking the jump
                if (get_piece(Position{t_pos.x-1, t_pos.y-1}) == nullptr || (s_pos == Position{t_pos.x-1, t_pos.y-1})) {
                    return 0;
                }
            }
        } else if (p2 != nullptr) {
            // check the piece that could jump is the opposite color of the selected piece
            if (red != p2->get_is_red()) {
                // see if theres a piece blocking the jump
                if ((get_piece(Position{t_pos.x+1, t_pos.y-1}) == nullptr) || (s_pos == Position{t_pos.x+1, t_pos.y-1})) {
                    return 0;
                }
            }
        }
    }


    return 1;
}

// check all the tiles a regular piece could move to
int GameBoard::checkRegularMoves(Position t_pos, Position s_pos, bool red, bool jump) {
    if (red) {
        // reds turn
        if ((t_pos.x == s_pos.x+1 || t_pos.x == s_pos.x-1) && t_pos.y == s_pos.y-1) {
            // try to go one space
            if (jump) {
                emit playSlideSound();
                return couldGetJumped(t_pos, s_pos, red);
            } else {
                return couldGetJumped(t_pos, s_pos, red);
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
                return couldGetJumped(t_pos, s_pos, red);
            } else {
                return couldGetJumped(t_pos, s_pos, red);
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
                return couldGetJumped(t_pos, s_pos, red);
            } else {
                return couldGetJumped(t_pos, s_pos, red);
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
                return couldGetJumped(t_pos, s_pos, red);
            } else {
                return couldGetJumped(t_pos, s_pos, red);
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

   if (!get_piece(t_pos)) {
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
                if (!get_piece(Position{i,j}) && (((i % 2 != 0) && (j % 2 != 0)) || ((i % 2 == 0) && (j % 2 == 0)))) {
                    return Position{i,j};
                }
            }
        }
    } else {
        for (int i = 9; i >= 0; i--) {
            for (int j = 9; j >= 0; j--) {
                if (!get_piece(Position{j,i}) && (((i % 2 != 0) && (j % 2 != 0)) || ((i % 2 == 0) && (j % 2 == 0)))) {
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

// after mkoving piece, see if we need to upgrade piece or handle any powerups
void GameBoard::checkLanding(Position t_pos, bool red) {
    Position last_pos = selected_->get_position();
    // check if we need piece upgrade after the move, if not just update the piece
    if (((t_pos.y == 0 && red) || (t_pos.y == 9 && !red)) && selected_->get_type() == PieceType::RegularPiece) {
        // making a regular piece into a king
        if (get_piece(last_pos)) { players_[current_player_]->removePiece(last_pos); }
        // create piece with new type, connect it and add to scene
        PiecePrototype* p = factory_->CreatePiece(PieceType::KingPiece, t_pos, red);
        players_[current_player_]->addPiece(p);
        connect(p, SIGNAL(gotSelected(PiecePrototype*)), this, SLOT(pieceSelected(PiecePrototype*)));
        selected_ = p;
        emit addPiece(p);
    } else if (((t_pos.y == 9 && red) || (t_pos.y == 0 && !red)) && selected_->get_type() == PieceType::KingPiece) {
        // making a king into a triple king
        if (get_piece(last_pos)) { players_[current_player_]->removePiece(last_pos); }
        // create piece with new type, connect it and add to scene
        PiecePrototype* p = factory_->CreatePiece(PieceType::TripleKingPiece, t_pos, red);
        players_[current_player_]->addPiece(p);
        connect(p, SIGNAL(gotSelected(PiecePrototype*)), this, SLOT(pieceSelected(PiecePrototype*)));
        selected_ = p;
        emit addPiece(p);
    } else {
        // if not changing the type just update piece
        selected_->set_position(t_pos);
        players_[current_player_]->updatePiece(last_pos, t_pos);
        selected_->set_highlighted(false);
        emit updatePiece(selected_);
        handlePowerup(t_pos, last_pos, red);
    }
}


// helper for when tile is selected
void GameBoard::handleSelected(Tile* t, bool red) {
    int score = checkValidity(t, selected_, red, true);
    if (score != -1) {
        if (score == 0 || score == 1) {
            checkLanding(t->get_position(), red);
        } else if (score == 2 || score == 4) {
            // if we just jumped a piece, check for double jump
            checkLanding(t->get_position(), red);
            for (Tile* p_t : getPieceMoves(selected_)) {
                int temp = checkValidity(p_t, selected_, red, false);
                if ((temp == 2) || (temp == 4)) {
                    handleSelected(p_t, red);
                }
            }
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
        if ((p_red != current_player_) && (get_piece(t->get_position()) == nullptr)) {
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
        if (checkValidity(t, p, p->get_is_red(), false) != -1) {
            valid_tiles.push_back(t);
        }
    }
    return valid_tiles;
}

void GameBoard::doAITurn(int turn) {
    if (current_player_ == turn) {
        // find pieces with valid moves
        std::vector<PiecePrototype*> valid_pieces;
        for (PiecePrototype* piece : players_[turn]->get_pieces()) {
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
            } else if (difficulty_ == Difficulty::Medium || difficulty_ == Difficulty::Simulation) {
                // pick the best move for that turn
                std::vector<Move> moves;
                for (PiecePrototype* piece : valid_pieces) {
                    for (Tile* tile : tiles_) {
                        if (checkValidity(tile, piece, piece->get_is_red(), false) != -1) {
                            moves.push_back(Move{piece, tile, checkValidity(tile, piece, piece->get_is_red(), false)});
                        }
                    }
                }
                std::vector<Move> safe_moves;
                std::vector<Move> jump_moves;
                std::vector<Move> dbl_jump_moves;
                for (Move m : moves) {
                    if (m.score == 1) {
                        safe_moves.push_back(m);
                    } else if (m.score == 2) {
                        jump_moves.push_back(m);
                    } else if (m.score == 4) {
                        dbl_jump_moves.push_back(m);
                    }
                }
                if (dbl_jump_moves.size() > 0) {
                    selected_ = dbl_jump_moves[0].piece;
                    tileSelected(dbl_jump_moves[0].tile);
                } else if (jump_moves.size() > 0) {
                    selected_ = jump_moves[0].piece;
                    tileSelected(jump_moves[0].tile);
                } else if (safe_moves.size() > 0) {
                    int m_i = arc4random()%safe_moves.size();
                    selected_ = safe_moves[m_i].piece;
                    tileSelected(safe_moves[m_i].tile);
                }else {
                    int m_i = arc4random()%moves.size();
                    selected_ = moves[m_i].piece;
                    tileSelected(moves[m_i].tile);
                }
            }
        }
    }
}

// handle the timeout from the AI timer to make the AIs move if its their turn
void GameBoard::black_Timer_slot() {
    doAITurn(1);
}

void GameBoard::red_Timer_slot() {
    doAITurn(0);
}

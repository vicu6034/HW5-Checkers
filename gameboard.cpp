#include "gameboard.h"

#include <QThread>
#include <QTime>
#include <QTimer>

#include <pieceprototype.h>
#include <pieces.h>
#include <tile.h>
#include <player.h>
#include <powerup.h>

/* Initialize the game board with tiles and players
 * Set up initial game state
*/
GameBoard::GameBoard() {
    // set up timers
    red_timer_ = new QTimer(this);
    connect(red_timer_, SIGNAL(timeout()), this, SLOT(red_timer_slot()));
    black_timer_ = new QTimer(this);
    connect(black_timer_, SIGNAL(timeout()), this, SLOT(black_timer_slot()));
    // set difficulty to MP by default
    difficulty_ = Difficulty::None;
    tiles_ = HashTable();
    // create tiles
    auto switcher = false;
    for (int i = 0; i < 10; i++) {
        for (int j = 0; j < 10; j++) {
            Tile* tile = new Tile(Position{i,j}, switcher);
            connect(tile, SIGNAL(gotSelected(Tile*)), this, SLOT(tileSelected_slot(Tile*)));
            tiles_.insertTile(tile);
            //tiles_.push_back(tile);
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
            connect(b1, SIGNAL(gotSelected(PiecePrototype*)), this, SLOT(pieceSelected_slot(PiecePrototype*)));
            connect(b2, SIGNAL(gotSelected(PiecePrototype*)), this, SLOT(pieceSelected_slot(PiecePrototype*)));
            connect(r, SIGNAL(gotSelected(PiecePrototype*)), this, SLOT(pieceSelected_slot(PiecePrototype*)));
            black_pieces.push_back(b1);
            black_pieces.push_back(b2);
            red_pieces.push_back(r);
        } else {
            // add pieces for odd rows
            // odd black pieces
            PiecePrototype* b = factory_->CreatePiece(PieceType::RegularPiece, Position{i,1}, false);
            connect(b, SIGNAL(gotSelected(PiecePrototype*)), this, SLOT(pieceSelected_slot(PiecePrototype*)));
            black_pieces.push_back(b);
            // odd red pieces
            PiecePrototype* r1 = factory_->CreatePiece(PieceType::RegularPiece, Position{i,7}, true);
            PiecePrototype* r2 = factory_->CreatePiece(PieceType::RegularPiece, Position{i,9}, true);
            connect(r1, SIGNAL(gotSelected(PiecePrototype*)), this, SLOT(pieceSelected_slot(PiecePrototype*)));
            connect(r2, SIGNAL(gotSelected(PiecePrototype*)), this, SLOT(pieceSelected_slot(PiecePrototype*)));
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

/* Reset both players wins to 0
*/
void GameBoard::ResetWins() {
    players_[0]->set_num_wins(0);
    players_[1]->set_num_wins(0);
}

/* Get all tiles
 * @return vec<Tile*>
*/
std::vector<Tile*> GameBoard::get_tiles() const {
    std::vector<Tile*> tiles;
    for (int i = 0; i < 10; i++) {
        for (int j = 0; j < 10; j++) {
            tiles.push_back(tiles_.getTile(Position{i,j}));
        }
    }
    return tiles;
}

/* Get all pieces in the game
 * @return vec<Piece*>
*/
std::vector<PiecePrototype*> GameBoard::get_pieces() const {
    // get both players pieces and return them all
    auto vec1 = players_[0]->get_pieces();
    auto vec2 = players_[1]->get_pieces();
    vec1.insert(vec1.end(), vec2.begin(), vec2.end());
    return vec1;
}

/* Get a piece by position, nullptr if none
 * @param Position to search for
 * @return Piece* if found at that Position
*/
PiecePrototype* GameBoard::get_piece(Position pos) const {
    // get all pieces
    for (const auto& p : get_pieces()) {
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
    auto p = get_piece(pos);
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
    auto p = get_piece(pos);
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
    auto p = get_piece(pos1);
    auto p2 = get_piece(pos2);
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
 * @param Position t_pos : position of tile we want to move to
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
        auto p = get_piece(Position{t_pos.x+1, t_pos.y-1});
        auto p2 = get_piece(Position{t_pos.x-1, t_pos.y-1});
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
        auto p = get_piece(Position{t_pos.x+1, t_pos.y+1});
        auto p2 = get_piece(Position{t_pos.x-1, t_pos.y+1});
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

/* See if a regular piece can move to tile
 * @param Position t_pos : position of tile we want to move to
 * @param Position s_pos : position piece is moving from
 * @param bool red : color of piece that wants to move
 * @param bool jump : if we're doing the move or just checking
 * @return int score of the move
*/
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

/* See if a King piece can move to tile
 * @param Position t_pos : position of tile we want to move to
 * @param Position s_pos : position piece is moving from
 * @param bool red : color of piece that wants to move
 * @param bool jump : if we're doing the move or just checking
 * @return int score of the move
*/
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

/* See if a TripleK piece can move to tile
 * @param Position t_pos : position of tile we want to move to
 * @param Position s_pos : position piece is moving from
 * @param bool red : color of piece that wants to move
 * @param bool jump : if we're doing the move or just checking
 * @return int score of the move
*/
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

/* See if a piece can move to a tile
 * @param Tile t : tile we want to move to
 * @param Piece p : piece that wants to move
 * @param bool red : color of piece that wants to move
 * @param bool jump : if we're doing the move or just checking
 * @return int score of the move
 * score of -1 is invalid, 0 is unsafe, 1 is safe, 2 is jump, 4 is doublejump
*/
int GameBoard::checkValidity(Position t_pos, PiecePrototype* p, bool red, bool jump) {
   auto s_pos = p->get_position();

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

/* See if theres a powerup at a tile
 * @param Position to check
 * @return string name of the move ("none" if none)
*/
std::string GameBoard::checkPowerup(Position pos) {
    for (const auto& powerup : powerups_ ) {
        if (powerup->get_position() == pos) {
            return powerup->get_type();
        }
    }
    return "none";
}

/* Delete a powerup
 * @param Position to delete powerup from
*/
void GameBoard::removePowerup(Position pos) {
    for (unsigned int i = 0 ; i < powerups_.size(); i++) {
        if (powerups_[i]->get_position() == pos) {
            delete powerups_[i];
            powerups_.erase(powerups_.begin() + i);
        }
    }
}

/* Find the closest open tile from the 'back' to add a piece to
 * @param bool red or black player
 * @return Position to add piece to
*/
Position GameBoard::findPosForPowerup(bool red) const {
    if (!red) {
        // for black player start from top of board moving looking for first black tile with no pieces
        for (int j = 0; j < 10; j++) {
            for (int i = 0; i < 10; i++) {
                if (!get_piece(Position{i,j}) && (((i % 2 != 0) && (j % 2 != 0)) || ((i % 2 == 0) && (j % 2 == 0)))) {
                    return Position{i,j};
                }
            }
        }
    } else {
        // for red player start from bottom of board looking for first black tile with no pieces
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

/* Handle what happens when someone lands on a powerup
 * @param Position t_pos : position powerup is at
 * @param Position last_pos : position piece moved from
 * @return Position to add piece to
*/
void GameBoard::handlePowerup(Position t_pos, Position last_pos, bool red) {
    // check for landing on powerups
    if (checkPowerup(t_pos) == "add") {
        //removePowerup(t_pos);
        // add a piece to current player
        // create piece with new type, connect it and add to scene
        PiecePrototype* p = factory_->CreatePiece(PieceType::RegularPiece, findPosForPowerup(red), red);
        players_[current_player_]->addPiece(p);
        connect(p, SIGNAL(gotSelected(PiecePrototype*)), this, SLOT(pieceSelected_slot(PiecePrototype*)));
        emit addPiece(p);
        emit updatePiecesLabel(red, players_[current_player_]->get_num_pieces());
    } else if (checkPowerup(t_pos) == "level") {
        // level up piece that landed on the powerup
        players_[current_player_]->removePiece(last_pos);
        PiecePrototype* p;
        if (selected_->get_type() == PieceType::RegularPiece) {
            p = factory_->CreatePiece(PieceType::KingPiece, t_pos, red);
        } else if (selected_->get_type() == PieceType::KingPiece) {
            p = factory_->CreatePiece(PieceType::TripleKingPiece, t_pos, red);
        } else {
            return;
        }
        players_[current_player_]->addPiece(p);
        connect(p, SIGNAL(gotSelected(PiecePrototype*)), this, SLOT(pieceSelected_slot(PiecePrototype*)));
        selected_ = p;
        emit addPiece(p);
    }
    removePowerup(t_pos);
}

/* Check if we need to upgrade piece or handle a powerup after landing
 * @param Position we landed on
 * @param bool color of player
*/
void GameBoard::checkLanding(Position t_pos, bool red) {
    auto last_pos = selected_->get_position();
    // check if we need piece upgrade after the move, if not just update the piece
    if (((t_pos.y == 0 && red) || (t_pos.y == 9 && !red)) && selected_->get_type() == PieceType::RegularPiece) {
        // making a regular piece into a king
        if (get_piece(last_pos)) { players_[current_player_]->removePiece(last_pos); }
        // create piece with new type, connect it and add to scene
        PiecePrototype* p = factory_->CreatePiece(PieceType::KingPiece, t_pos, red);
        players_[current_player_]->addPiece(p);
        connect(p, SIGNAL(gotSelected(PiecePrototype*)), this, SLOT(pieceSelected_slot(PiecePrototype*)));
        selected_ = p;
        emit addPiece(p);
    } else if (((t_pos.y == 9 && red) || (t_pos.y == 0 && !red)) && selected_->get_type() == PieceType::KingPiece) {
        // making a king into a triple king
        if (get_piece(last_pos)) { players_[current_player_]->removePiece(last_pos); }
        // create piece with new type, connect it and add to scene
        PiecePrototype* p = factory_->CreatePiece(PieceType::TripleKingPiece, t_pos, red);
        players_[current_player_]->addPiece(p);
        connect(p, SIGNAL(gotSelected(PiecePrototype*)), this, SLOT(pieceSelected_slot(PiecePrototype*)));
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


/* When a tiles selected see if the seleced piece can move to it
 * @param Position t_pos : position powerup is at
 * @param Position last_pos : position piece moved from
 * @return Position to add piece to
*/
void GameBoard::handleSelected(Tile* t, bool red) {
    // try do the move
    auto score = checkValidity(t->get_position(), selected_, red, true);
    if (score != -1) {
        // valid moves
        if (score == 0 || score == 1) {
            // check for upgrades/powerups
            checkLanding(t->get_position(), red);
        } else if (score == 2 || score == 4) {
            // check for upgrades/powerups
            checkLanding(t->get_position(), red);
            // after a jump, check if another can immediately be made
            for (Tile* p_t : getPieceMoves(selected_)) {
                int temp = checkValidity(p_t->get_position(), selected_, red, false);
                if ((temp == 2) || (temp == 4)) {
                    handleSelected(p_t, red);
                }
            }
        }
        // see if theres a winner
        auto winner = checkForWinner();
        if (winner == -1) {
            // if no winner iterate turn
            if (red) {
                current_player_ = 1;
            } else {
                current_player_ = 0;
            }
            // update turn label
            emit updateTurnLabel(current_player_);
        } else {
            // if someone one give them a win and dispplay winner popup
            players_[winner]->set_num_wins(players_[winner]->get_num_wins()+1);
            emit gameOver(winner);
        }
    } else {
        // if the tiles not valid play error sound
        emit playDeniedSound();
    }
}

/* When a tile is selected, check we have a selected piece and its an empty tile
 * If so try to do the move
 * @param Tile that was selected
*/
void GameBoard::tileSelected_slot(Tile* t) {
    // first make sure we have a Piece selected
    if (selected_) {
        auto p_red = selected_->get_is_red();
        // check that the selected piece is the same color as current player
        // check that theres no piece at the tile we want to go to
        if ((p_red != current_player_) && (get_piece(t->get_position()) == nullptr)) {
            handleSelected(t, p_red);
        }
    } else {
        emit playDeniedSound();
    }
}

/* When a piece is selected deselect old piece and make it highlighted
 * @param Piece that was selected
*/
void GameBoard::pieceSelected_slot(PiecePrototype* p) {
    // if the piece is the color of the current player, select it
    // and its not the computers turn or simulation
    if (difficulty_ != Difficulty::Simulation) {
        if ((p->get_is_red() != current_player_) && !(difficulty_ != Difficulty::None && current_player_ == 1)) {
            // unhighlight previously selected piece
            if (selected_ != nullptr) {
                selected_->set_highlighted(false);
                selected_->update();
            }
            selected_ = p;
            selected_->set_highlighted(true);
            selected_->update();
        } else {
            emit playDeniedSound();
        }
    }
}

/* Get all tiles a piece could move to
 * @param Piece we want to check moves for
 * @return vec<Tile> all the Tiles the Piece can move to
*/
std::vector<Tile*> GameBoard::getPieceMoves(PiecePrototype* p) {
    std::vector<Tile*> valid_tiles;
    for (const auto& pos : p->GetPossibleMoves()) {
        if (checkValidity(pos, p, p->get_is_red(), false) != -1) {
            valid_tiles.push_back(tiles_.getTile(pos));
        }
    }
    return valid_tiles;
}

// method to compare moves (for sorting move vec)
/*
bool compareMoves(Move m1, Move m2) {
    return (m1.score > m2.score);
}
*/

/* Take a turn using the AI
 * @param int what side the AI should take a move for
*/
void GameBoard::doAITurn(int turn) {
    // check its the player we want to moves turn
    if (current_player_ == turn) {
        // find pieces with valid moves
        std::vector<PiecePrototype*> valid_pieces;
        for (const auto& piece : players_[turn]->get_pieces()) {
            if (getPieceMoves(piece).size()) {
                valid_pieces.push_back(piece);
            }
        }

        if (valid_pieces.size() == 0) {
            // if no pieces have moves they lose
            if (turn == 0) {
                emit gameOver(1);
            } else {
                emit gameOver(0);
            }
        } else {
            // pick move based on difficulty
            if (difficulty_ == Difficulty::Easy) {
                // pick a random move
                int p_i = arc4random()%valid_pieces.size();
                //pieceSelected_slot(valid_pieces[p_i]);
                selected_ = valid_pieces[p_i];
                // get tiles to move to
                std::vector<Tile*> valid_tiles = getPieceMoves(valid_pieces[p_i]);
                int t_i = arc4random_uniform(valid_tiles.size());
                tileSelected_slot(valid_tiles[t_i]);
            } else if (difficulty_ == Difficulty::Medium || difficulty_ == Difficulty::Simulation) {
                // find all the moves we can make
                std::vector<Move> moves;
                for (PiecePrototype* piece : valid_pieces) {
                    for (Position pos : piece->GetPossibleMoves()) {
                        if (checkValidity(pos, piece, piece->get_is_red(), false) != -1) {
                            Tile* tile = tiles_.getTile(pos);
                            moves.push_back(Move{piece, tile, checkValidity(tile->get_position(), piece, piece->get_is_red(), false)});
                        }
                    }
                }
                // pick best move
                // picking move from sorting vector
                /*
                sort(moves.begin(), moves.end(), compareMoves);
                if (moves[0].score > 1) {
                    selected_ = moves[0].piece;
                    tileSelected_slot(moves[0].tile);
                } else {
                    int rando = arc4random_uniform(moves.size() / 2);
                    selected_ = moves[rando].piece;
                    tileSelected_slot(moves[rando].tile);
                }
                */
                // picking move by specific type
                std::vector<Move> safe_moves;
                std::vector<Move> jump_moves;
                std::vector<Move> dbl_jump_moves;
                for (const auto& m : moves) {
                    if (m.score == 1) {
                        safe_moves.push_back(m);
                    } else if (m.score == 2) {
                        jump_moves.push_back(m);
                    } else if (m.score == 4) {
                        dbl_jump_moves.push_back(m);
                    }
                }
                // make best possible move
                // if multiple non-jump moves have same score pick randomly
                if (dbl_jump_moves.size() > 0) {
                    selected_ = dbl_jump_moves[0].piece;
                    tileSelected_slot(dbl_jump_moves[0].tile);
                } else if (jump_moves.size() > 0) {
                    selected_ = jump_moves[0].piece;
                    tileSelected_slot(jump_moves[0].tile);
                } else if (safe_moves.size() > 0) {
                    int m_i = arc4random_uniform(safe_moves.size());
                    selected_ = safe_moves[m_i].piece;
                    tileSelected_slot(safe_moves[m_i].tile);
                }else {
                    int m_i = arc4random_uniform(moves.size());
                    selected_ = moves[m_i].piece;
                    tileSelected_slot(moves[m_i].tile);
                }
            }
        }
    }
}

// do turn for black when their timer goes off
void GameBoard::black_timer_slot() {
    doAITurn(1);
}

// do turn for red when their timer goes off
void GameBoard::red_timer_slot() {
    doAITurn(0);
}

// stop all ai from playing
void GameBoard::StopTimers() {
    red_timer_->stop();
    black_timer_->stop();
}

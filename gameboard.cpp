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

// return true for valid move, false otherwise
bool GameBoard::checkValidity(Tile* t, bool red) {
   Position s_pos = selected_->get_position();
   Position t_pos = t->get_position();

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
   // return false if the Tile isnt possible to move to
   return false;
}

// helper for when tile is selected
void GameBoard::handleSelected(Tile* t, bool red) {
    if (checkValidity(t, red)) {
        // if the move is valid, change the pieces position and update scene
        selected_->set_position(t->get_position());
        emit updatePiece(selected_);
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
        selected_ = p;
    }
}

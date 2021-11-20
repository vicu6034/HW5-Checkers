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
            red_pieces.push_back(factory_->CreatePiece(PieceType::RegularPiece, Position{i,8}, true));
            black_pieces.push_back(factory_->CreatePiece(PieceType::RegularPiece, Position{i,0}, false));
        } else {
            red_pieces.push_back(factory_->CreatePiece(PieceType::RegularPiece, Position{i,9}, true));
            black_pieces.push_back(factory_->CreatePiece(PieceType::RegularPiece, Position{i,1}, false));
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

std::vector<PiecePrototype*> GameBoard::getPieces() {
    std::vector<PiecePrototype*> vec1 = players_[0]->get_pieces();
    std::vector<PiecePrototype*> vec2 = players_[1]->get_pieces();
    vec1.insert(vec1.end(), vec2.begin(), vec2.end());
    return vec1;
}

PiecePrototype* GameBoard::getSelectedPiece() {
    std::vector<PiecePrototype*> vec1 = players_[0]->get_pieces();
    std::vector<PiecePrototype*> vec2 = players_[1]->get_pieces();
    vec1.insert(vec1.end(), vec2.begin(), vec2.end());
    for (PiecePrototype* p : vec1) {
        if (p->get_selected()) return p;
    }
    return nullptr;
}

void GameBoard::deselectPiece() {
    std::vector<PiecePrototype*> vec1 = players_[0]->get_pieces();
    std::vector<PiecePrototype*> vec2 = players_[1]->get_pieces();
    vec1.insert(vec1.end(), vec2.begin(), vec2.end());
    for (PiecePrototype* p : vec1) {
        if (p->get_selected()) p->set_selected(false);
    }
}

void GameBoard::tileSelected(Tile* t) {
    if (getSelectedPiece() != nullptr) {
        PiecePrototype* p = getSelectedPiece();
        bool p_red = p->get_is_red();
        if (p_red && current_player_ == 0) {
            PieceType pt = p->get_type();
            delete p;
            PiecePrototype* newPiece = factory_->CreatePiece(pt, t->get_position(), p_red);
            connect(newPiece, SIGNAL(gotSelected()), this, SLOT(pieceSelected()));
            players_[0]->addPiece(newPiece);
            current_player_ = 1;
            emit updateTurnLabel(current_player_);
            emit addPiece(newPiece);
        } else if (!p_red && current_player_ == 1) {
            PieceType pt = p->get_type();
            delete p;
            PiecePrototype* newPiece = factory_->CreatePiece(pt, t->get_position(), p_red);
            connect(newPiece, SIGNAL(gotSelected()), this, SLOT(pieceSelected()));
            players_[1]->addPiece(newPiece);
            current_player_ = 0;
            emit updateTurnLabel(current_player_);
            emit addPiece(newPiece);
        }


    }
}

void GameBoard::pieceSelected() {
    deselectPiece();
}

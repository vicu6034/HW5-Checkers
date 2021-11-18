#include "gameboard.h"

GameBoard::GameBoard()
{
    // here we will set up the starting state of the game

    // TODO: positions need to be converted to grid (1,1 through 10,10)
    //       they currently are still just the pixel values
    // Then we can instantiate all the tiles here
    // Tile(Pos, is_red)
    tiles_.push_back(new Tile(Position{0,0}, true));
    tiles_.push_back(new Tile(Position{0,1}, false));
    tiles_.push_back(new Tile(Position{0,2}, true));
    tiles_.push_back(new Tile(Position{10,10}, true));

    // initialize factory (this is called implicitly so unnecessary)
    // factory_ = new PieceFactory();

    // make Pieces like this
    // CreatePiece(Type, Pos, is_red)
    PiecePrototype* r = factory_->CreatePiece(PieceType::RegularPiece, Position{0,0}, true);
    PiecePrototype* k = factory_->CreatePiece(PieceType::KingPiece, Position{0,1}, false);
    PiecePrototype* t = factory_->CreatePiece(PieceType::TripleKingPiece, Position{10,10}, true);

    // proving they're correct types
    r->Test();
    k->Test();
    t->Test();

    // giving test pieces to a player so we can return them to the view
    std::vector<PiecePrototype*> pieces;
    pieces.push_back(r);
    pieces.push_back(k);
    pieces.push_back(t);

    // add players to the game
    // Player(is_red, vec<PiecePrototype*>)
    players_.push_back(new Player(false, pieces));

    // add powerups to the game
    // PowerUp(pos, is_addPiece)
    powerups_.push_back(new PowerUp(Position{10,0}, true));
    powerups_.push_back(new PowerUp(Position{0,10}, false));

}

std::vector<PiecePrototype*> GameBoard::getPieces() {
    /* ACTUAL METHOD
    std::vector<PiecePrototype*> vec1 = players_[0]->get_pieces();
    std::vector<PiecePrototype*> vec2 = players_[1]->get_pieces();
    vec1.insert(vec1.end(), vec2.begin(), vec2.end());
    return vec1;
    */

    //using for testing
    return players_.front()->get_pieces();
}

#include "gameboard.h"

GameBoard::GameBoard()
{
    // here we will set up the starting state of the game

    // TODO: positions need to be converted to grid (1,1 through 10,10)
    //       they currently are still just the pixel values
    // Then we can instantiate all the tiles here
    tiles_.push_back(new Tile(Position{1,1}, true));
    tiles_.push_back(new Tile(Position{1,30}, false));

    // old factory usage
    // We're gonna want to go back to something like this
    // I just made it more simple for now so it works
    /*
    PiecePrototype* pt = factory_->CreatePrototype(PieceType::RegularPiece);
    pieces.push_back(pt);
    */

    // new factory setup
    r_factory_ = new RegularPieceFactory();
    k_factory_ = new KingPieceFactory();
    t_factory_ = new TripleKingPieceFactory();

    // creating pieces with new factories
    PiecePrototype* r = r_factory_->CreatePrototype(Position{40,1}, true);
    PiecePrototype* k = k_factory_->CreatePrototype(Position{80,1}, false);
    PiecePrototype* t = t_factory_->CreatePrototype(Position{120,1}, true);

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
    players_.push_back(new Player(false, pieces));

    // add powerups to the game
    powerups_.push_back(new PowerUp(Position{100,100}, true));
    powerups_.push_back(new PowerUp(Position{150,150}, false));

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

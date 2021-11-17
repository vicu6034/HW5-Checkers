#include "gameboard.h"

GameBoard::GameBoard()
{
    factory_ = new PiecePrototypeFactory();
    PiecePrototype pt = factory_->CreatePrototype(PieceType::RegularPiece);
}

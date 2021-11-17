#ifndef TRIPLEKINGPIECE_H
#define TRIPLEKINGPIECE_H

#include <pieceprototype.h>

class TripleKingPiece : public PiecePrototype
{

public:
    TripleKingPiece(std::string prototype_name, Position pos)
        : PiecePrototype(prototype_name, pos) {}

};

#endif // TRIPLEKINGPIECE_H

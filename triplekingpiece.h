#ifndef TRIPLEKINGPIECE_H
#define TRIPLEKINGPIECE_H

#include <pieceprototype.h>

class TripleKingPiece : public PiecePrototype
{

public:
    TripleKingPiece(std::string prototype_name)
        : PiecePrototype(prototype_name) {}

    PiecePrototype *Clone() const override {
        return new TripleKingPiece(*this);
    }

};

#endif // TRIPLEKINGPIECE_H

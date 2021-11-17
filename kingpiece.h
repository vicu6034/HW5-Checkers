#ifndef KINGPIECE_H
#define KINGPIECE_H

#include <pieceprototype.h>

class KingPiece : public PiecePrototype
{

public:
    KingPiece(std::string prototype_name)
        : PiecePrototype(prototype_name) {}

    PiecePrototype *Clone() const override {
        return new KingPiece(*this);
    }

};

#endif // KINGPIECE_H

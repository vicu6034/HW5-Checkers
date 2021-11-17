#ifndef REGULARPIECE_H
#define REGULARPIECE_H

#include <pieceprototype.h>

class RegularPiece : public PiecePrototype
{

public:
    RegularPiece(std::string prototype_name)
        : PiecePrototype(prototype_name) {}

    PiecePrototype *Clone() const override {
        return new RegularPiece(*this);
    }

};

#endif // REGULARPIECE_H

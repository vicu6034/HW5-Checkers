#ifndef PIECEPROTOTYPEFACTORY_H
#define PIECEPROTOTYPEFACTORY_H

#include <unordered_map>

#include <regularpiece.h>
#include <kingpiece.h>
#include <triplekingpiece.h>

class PiecePrototypeFactory
{

private:
    std::unordered_map<PieceType, PiecePrototype*, std::hash<int>> prototypes_;

public:
    PiecePrototypeFactory() {
        prototypes_[PieceType::RegularPiece] = new RegularPiece("RegularPiece");
        prototypes_[PieceType::KingPiece] = new KingPiece("KingPiece");
        prototypes_[PieceType::RegularPiece] = new TripleKingPiece("TripleKingPiece");
    }

    ~PiecePrototypeFactory() {
        delete prototypes_[PieceType::RegularPiece];
        delete prototypes_[PieceType::KingPiece];
        delete prototypes_[PieceType::TripleKingPiece];
    }

    PiecePrototype *CreatePrototype(PieceType type) {
        return prototypes_[type]->Clone();
    }

};

#endif // PIECEPROTOTYPEFACTORY_H

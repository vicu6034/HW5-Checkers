#ifndef PIECEPROTOTYPEFACTORY_H
#define PIECEPROTOTYPEFACTORY_H

#include <regularpiece.h>
#include <kingpiece.h>
#include <triplekingpiece.h>

class PieceFactory
{

public:
    PiecePrototype* CreatePiece(PieceType type, Position pos, bool is_red) {
        switch (type){
            case PieceType::RegularPiece:
                return new RegularPiece(pos,is_red);
            case PieceType::KingPiece:
                return new KingPiece(pos,is_red);
            case PieceType::TripleKingPiece:
                return new TripleKingPiece(pos,is_red);

        }
    }

};

#endif // PIECEPROTOTYPEFACTORY_H


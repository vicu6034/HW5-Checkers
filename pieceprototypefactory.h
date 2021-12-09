/*
 * PieceFactory header
 * Creates pieces given a type, position, and color
 * CSCI 3010 Homework 5
 * By: Vincent Curran & Philip Knott
*/

#ifndef PIECEPROTOTYPEFACTORY_H
#define PIECEPROTOTYPEFACTORY_H

#include <pieces.h>

class PieceFactory
{

public:
    // Factory creates pieces with Type, Pos, color (bool)
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


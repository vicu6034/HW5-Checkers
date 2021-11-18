#ifndef PIECEPROTOTYPEFACTORY_H
#define PIECEPROTOTYPEFACTORY_H

#include <regularpiece.h>
#include <kingpiece.h>
#include <triplekingpiece.h>

class PieceFactory
{

public:
    virtual ~PieceFactory() {}

    virtual PiecePrototype* CreatePrototype(Position pos, bool is_red) const = 0;

};

class RegularPieceFactory : PieceFactory {
public:
    PiecePrototype* CreatePrototype(Position pos, bool is_red) const override { return new RegularPiece(pos,is_red); }
};

class KingPieceFactory : PieceFactory {
public:
    PiecePrototype* CreatePrototype(Position pos, bool is_red) const override { return new KingPiece(pos,is_red); }
};

class TripleKingPieceFactory : PieceFactory {
public:
    PiecePrototype* CreatePrototype(Position pos, bool is_red) const override { return new TripleKingPiece(pos,is_red); }
};

#endif // PIECEPROTOTYPEFACTORY_H


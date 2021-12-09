#include "pieces.h"

#include <QtWidgets>

// override paint to give Regular Pieces their own appearance
void RegularPiece::paint(QPainter *painter, const QStyleOptionGraphicsItem *item, QWidget *widget) {
    QString piecetype = is_red_ ? "redchecker" : "blackchecker";
    paintHelper(painter, item, widget, piecetype);
}

// override paint to give Kings their own appearance
void KingPiece::paint(QPainter *painter, const QStyleOptionGraphicsItem *item, QWidget *widget) {
    QString piecetype = is_red_ ? "redking" : "blackking";
    paintHelper(painter, item, widget, piecetype);
}

// override paint to give TripleKing Pieces their own appearance
void TripleKingPiece::paint(QPainter *painter, const QStyleOptionGraphicsItem *item, QWidget *widget) {
    QString piecetype = is_red_ ? "redtriple" : "blacktriple";
    paintHelper(painter, item, widget, piecetype);
}

#ifndef HASHTABLE_H
#define HASHTABLE_H

#include "tile.h"

struct Hnode
{
    Tile* tile_;
    struct Hnode* next_;

    Hnode() : tile_(0), next_(0) {};
    Hnode(Tile* tile) : tile_(tile), next_(0) {};

};

class HashTable
{
    Hnode** table_;

public:
    HashTable();

    void insertTile(Tile* tile);
    int hashFunction(Position pos) const { return ((pos.x+pos.y)%131); };
    Tile* getTile(Position pos) const;

};

#endif // HASHTABLE_H

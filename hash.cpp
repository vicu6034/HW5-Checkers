#include "hash.h"
#include <QDebug>

HashTable::HashTable() {
    table_ = new Hnode*[131];
    for (int i = 0; i < 131; i++) {
        table_[i] = 0;
    }
}

void HashTable::insertTile(Tile *tile) {
    auto newNode = new Hnode(tile);
    auto index = hashFunction(tile->get_position());
    if (table_[index] == 0) {
        table_[index] = newNode;
    } else {
        Hnode* curr = table_[index];
        while (curr->next_ != NULL) {
            curr = curr->next_;
        }
        curr->next_ = newNode;
    }
}

Tile* HashTable::getTile(Position pos) const {
    auto index = hashFunction(pos);
    if (table_[index] == 0) {
        return nullptr;
    } else if (table_[index]->tile_->get_position() == pos) {
        return table_[index]->tile_;
    } else {
        auto curr = table_[index];
        while (curr != 0) {
            if (curr->tile_->get_position() == pos) { return curr->tile_; }
            else { curr = curr->next_; }
        }
        return 0;
    }
}

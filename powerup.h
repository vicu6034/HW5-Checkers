#ifndef POWERUP_H
#define POWERUP_H

#include <string>
#include <player.h>

class PowerUp
{

private:
    std::string powerup_;

public:
    PowerUp();

    void Activate(Player* player);

};

#endif // POWERUP_H

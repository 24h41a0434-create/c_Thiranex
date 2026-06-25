#include "player.h"

Player::Player(string playerName, char playerMark) {
    name = playerName;
    mark = playerMark;
}

string Player::getName() {
    return name;
}

char Player::getMark() {
    return mark;
}

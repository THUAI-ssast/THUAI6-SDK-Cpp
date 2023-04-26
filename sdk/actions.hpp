#ifndef ACTIONS_HPP
#define ACTIONS_HPP

#include "sdk/datatypes.hpp"

using namespace std;

typedef json Action;

Action Move(ForwardOrBackWard direction) {
    return {{"type", "Move"}, {"direction", direction}};
}

Action Rotate(LeftOrRight direction) {
    return {{"type", "Rotate"}, {"direction", direction}};
}

Action Shoot() {
    return {{"type", "Shoot"}};
}

Action ChangeBullet() {
    return {{"type", "ChangeBullet"}};
}

Action PlaceBomb(Vector2Int target) {
    return {{"type", "PlaceBomb"}, {"target", target}};
}

Action AddLine(Direction direction) {
    return {{"type", "AddLine"}, {"direction", direction}};
}

Action RemoveLine(Direction direction) {
    return {{"type", "RemoveLine"}, {"direction", direction}};
}

Action ActivatePortal(Vector2Int destination) {
    return {{"type", "ActivatePortal"}, {"destination", destination}};
}

Action Idle() {
    return {{"type", "Idle"}};
}

#endif // ACTIONS_HPP
#include <random>

#include "sdk/actions.hpp"
#include "sdk/controller.hpp"
#include "sdk/datatypes.hpp"
#include "sdk/utils.hpp"

#include <iostream>

using namespace std;

map<Vector2Int, vector<Bomb>> bombs_map;
map<Vector2Int, Portal> portals_map;
vector<vector<int>> map_info;
int my_id;
Team my_team;

void init(json obs) {
    map_info = obs["map"];
    my_id = obs["myId"];
    my_team = obs["myTeam"];
    srand(9973);
}

Action get_action(json obs) {
    vector<Player> players = get_players(obs);
    vector<Bomb> bombs = get_bombs(obs);
    map<PortalPattern, vector<Portal>> portals_by_pattern = get_portals_by_pattern(obs);

    bombs_map.clear();
    for (Bomb bomb : bombs)
        bombs_map[bomb.position].push_back(bomb);
    portals_map.clear();
    for (auto& portals : portals_by_pattern)
        for (auto& portal : portals.second)
            portals_map[portal.position] = portal;

    Player my_player = players[my_id];
    PlayerState my_state = my_player.state;
    Vector2Int my_cell = to_cell(my_player.position);

    double p = (double)rand() / RAND_MAX;

    if (p < 0.01 && my_player.bomb_count > 0 && my_state.can_place_bomb)
        return PlaceBomb(my_cell);
    else if (p < 0.02) {
        if (my_state.can_activate_portal && (portals_map.find(my_cell)) != portals_map.end()) {
            Portal my_portal = portals_map[my_cell];
            vector<Portal> dest_portals = portals_by_pattern[my_portal.pattern];
            Portal dest_portal = dest_portals[rand() % dest_portals.size()];
            return ActivatePortal(dest_portal.position);
        }
        if (my_state.can_modify_portal) {
            while (true) {
                Direction direction;
                switch (rand() % 4) {
                case 0:
                    direction = Direction::Up;
                    break;
                case 1:
                    direction = Direction::Down;
                    break;
                case 2:
                    direction = Direction::Left;
                    break;
                case 3:
                    direction = Direction::Right;
                    break;
                }

                if (can_modify_portal_line(my_cell, direction, map_info))
                    return AddLine(direction);
            }
        }
        return Idle();
    } else if (p < 0.2)
        return Rotate(LeftOrRight::Left);
    else if (p < 0.25)
        return Move(ForwardOrBackWard::Backward);
    else if (p < 0.5 && my_player.ammo > 0)
        return Shoot();
    else if (p < 0.5)
        return ChangeBullet();
    else
        return Move(ForwardOrBackWard::Forward);

    return Idle();
}

int main() {
    run_ai(init, get_action);
    return 0;
}
#ifndef UTILS_HPP
#define UTILS_HPP

#include <cmath>
#include <iostream>
#include "sdk/datatypes.hpp"

using namespace std;

Vector2 to_cell_center(Vector2Int pos) {
    return {pos.x + 0.5, pos.y + 0.5};
}

Vector2Int to_cell(Vector2 pos) {
    return {int(pos.x), int(pos.y)};
}

float distance(Vector2 pos1, Vector2 pos2) {
    return sqrt((pos1.x - pos2.x) * (pos1.x - pos2.x) + (pos1.y - pos2.y) * (pos1.y - pos2.y));
}

float angle(Vector2 pos1, Vector2 pos2) {
    return atan2(pos2.y - pos1.y, pos2.x - pos1.x);
}

bool is_in_portal(Vector2 position, Vector2Int portal_position) {
    return position.x >= portal_position.x && position.x < portal_position.x + 1 &&
           position.y >= portal_position.y && position.y < portal_position.y + 2;
}

bool is_in_map(Vector2Int cell_position, vector<vector<int>> map_info) {
    return cell_position.x >= 0 && cell_position.x < map_info.size() &&
           cell_position.y >= 0 && cell_position.y < map_info[0].size();
}

bool is_road(Vector2Int cell_position, vector<vector<int>> map_info) {
    return is_in_map(cell_position, map_info) && map_info[cell_position.x][cell_position.y] == 0;
}

bool can_modify_portal_line(Vector2Int cell_position, Direction direction, vector<vector<int>> map_info) {
    if (!is_road(cell_position, map_info)) return false;
    if (direction == Direction::Up)
        return is_road(cell_position + Vector2Int(0, 1), map_info);
    else if (direction == Direction::Down)
        return is_road(cell_position + Vector2Int(0, -1), map_info);
    else if (direction == Direction::Left)
        return is_road(cell_position + Vector2Int(-1, 0), map_info);
    else if (direction == Direction::Right)
        return is_road(cell_position + Vector2Int(1, 0), map_info);
    else
        return false;
}

vector<Player> get_players(json obs) {
    vector<Player> players;
    for (json player : obs["players"])
        players.push_back(Player(player));
    return players;
}

vector<Bomb> get_bombs(json obs) {
    vector<Bomb> bombs;
    for (json bomb : obs["bombs"])
        bombs.push_back(Bomb(bomb));
    return bombs;
}
map<PortalPattern, vector<Portal>> get_portals_by_pattern (json obs) {
    map<PortalPattern, vector<Portal>> result;

    for (auto& el : obs["portalsClassifiedByPattern"].items()) {
        PortalPattern pattern = atoi(el.key().c_str());

        vector<Portal> portals;
        for (json portal : el.value()) {
            portals.push_back(portal);
        }
        result[pattern] = portals;
    }
    return result;
}

#endif // UTILS_HPP
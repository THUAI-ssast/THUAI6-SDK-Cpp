#ifndef DATATYPES_HPP
#define DATATYPES_HPP

#include <nlohmann/json.hpp>

using namespace std;
using json = nlohmann::json;

enum class Team
{
    Red,
    Blue
};

Team get_opposite_team(Team team)
{
    return team == Team::Red ? Team::Blue : Team::Red;
}

enum class ForwardOrBackWard
{
    Forward,
    Backward
};

enum class LeftOrRight
{
    Left,
    Right
};

enum class LineInPortalPattern : int
{
    LeftUp = 1,
    LeftDown = 2,
    Up = 4,
    RightUp = 8,
    RightDown = 16,
    Center = 32,
    Down = 64
};

enum class Direction : int
{
    Up = (int)(LineInPortalPattern::Center),
    Down = (int)(LineInPortalPattern::Down),
    Left = (int)(LineInPortalPattern::LeftDown),
    Right = (int)(LineInPortalPattern::RightDown)
};

typedef int PortalPattern;

struct Vector2Int
{
    int x;
    int y;

    Vector2Int(int x = 0, int y = 0) : x(x), y(y) {}

    Vector2Int operator+(const Vector2Int &other)
    {
        return {x + other.x, y + other.y};
    }

    Vector2Int operator-(const Vector2Int &other)
    {
        return {x - other.x, y - other.y};
    }

    bool operator<(const Vector2Int &other) const
    {
        return x < other.x || (x == other.x && y < other.y);
    }

    NLOHMANN_DEFINE_TYPE_INTRUSIVE(Vector2Int, x, y)
};

struct Vector2
{
    double x;
    double y;

    Vector2(double x = 0.0, double y = 0.0) : x(x), y(y) {}

    Vector2 operator+(const Vector2 &other)
    {
        return {x + other.x, y + other.y};
    }

    Vector2 operator-(const Vector2 &other)
    {
        return {x - other.x, y - other.y};
    }

    NLOHMANN_DEFINE_TYPE_INTRUSIVE(Vector2, x, y)
};

struct PlayerState
{
    bool is_alive;
    bool is_shooting;
    bool is_changing_bullet;
    bool is_placing_bomb;
    bool is_modifying_portal;
    bool is_activating_portal;

    bool can_move;
    bool can_rotate;
    bool can_shoot;
    bool can_change_bullet;
    bool can_place_bomb;
    bool can_modify_portal;
    bool can_activate_portal;

    PlayerState() {}

    PlayerState(const json &player_state_dict)
    {
        is_alive = player_state_dict["isAlive"];
        is_shooting = player_state_dict["isShooting"];
        is_changing_bullet = player_state_dict["isChangingBullet"];
        is_placing_bomb = player_state_dict["isPlacingBomb"];
        is_modifying_portal = player_state_dict["isModifyingPortal"];
        is_activating_portal = player_state_dict["isActivatingPortal"];

        can_move = player_state_dict["canMove"];
        can_rotate = player_state_dict["canRotate"];
        can_shoot = player_state_dict["canShoot"];
        can_change_bullet = player_state_dict["canChangeBullet"];
        can_place_bomb = player_state_dict["canPlaceBomb"];
        can_modify_portal = player_state_dict["canModifyPortal"];
        can_activate_portal = player_state_dict["canActivatePortal"];
    }
};

struct Player
{
    // static const
    float MAX_VELOCITY = 4.0;
    float ROTATION_SPEED = 600.0;
    int MAX_HP = 100;
    int MAX_AMMO = 30;
    int MAX_BOMB_COUNT = 1;
    float SHOOT_INTERVAL = 0.1;
    float CHANGE_BULLET_TIME = 2.5;
    float PLACE_BOMB_TIME = 2.0;
    float MODIFY_PORTAL_TIME = 0.5;
    float ACTIVATE_PORTAL_TIME = 1.0;
    float RESPAWN_TIME = 8.0;
    int BULLET_DAMAGE = 10;
    float BULLET_RANGE = 20.0;
    float MAX_BOMB_DISTANCE = 5.0;

    int id;
    Team team;
    PlayerState state;
    int hp;
    int ammo;
    int bomb_count;
    Vector2 position;
    float rotation;

    Player(json player_dict)
    {
        id = player_dict["id"];
        team = player_dict["team"];
        state = PlayerState(player_dict["state"]);
        hp = player_dict["hp"];
        ammo = player_dict["ammo"];
        bomb_count = player_dict["bombCount"];
        position = Vector2(player_dict["position"]["x"], player_dict["position"]["y"]);
        rotation = player_dict["rotation"];
    }
};

struct Bomb
{
    // static const
    float EXPLOSION_TIME = 5.0;
    float EXPLOSION_RADIUS = 2.0;
    int EXPLOSION_DAMAGE = 100;

    Vector2Int position;

    Bomb(json bomb_dict)
    {
        position = Vector2Int(bomb_dict["position"]["x"], bomb_dict["position"]["y"]);
    }
};

struct Portal
{
    // static const
    float WAIT_TIME = 3.0;

    Vector2Int position;
    PortalPattern pattern;
    bool is_activated;

    Portal() {}
    Portal(json portal_dict)
    {
        position = Vector2Int(portal_dict["position"]["x"], portal_dict["position"]["y"]);
        pattern = portal_dict["pattern"];
        is_activated = portal_dict["isActivated"];
    }
};

#endif // DATATYPES_HPP
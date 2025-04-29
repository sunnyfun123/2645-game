#ifndef RECYCLERENGINE_H
#define RECYCLERENGINE_H

enum Difficulty { EASY, HARD };
#include "mbed.h"
#include "Joystick.h"
#include "N5110.h"

// Game object structure
struct Item {
    int x, y;
    int type;      // 0-2: recyclable, 3: harmful
    bool active;
};

struct Player {
    int x, y;
};

class RecyclerEngine {
public:
    RecyclerEngine();
    void init();
    int update(UserInput input);
    void draw(N5110 &lcd);
    void setDifficulty(Difficulty d);

private:
    Difficulty difficulty;
    int drop_speed;
    int harmful_chance;

    void move_player(UserInput input);
    void spawn_items();
    void update_items();
    void check_collisions();

    Player player;
    Item items[5];
    int score;
    int lives;
    int spawn_timer;
};

#endif

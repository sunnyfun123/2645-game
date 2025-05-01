#ifndef RECYCLERENGINE_H
#define RECYCLERENGINE_H

#include "mbed.h"
#include "N5110.h"
#include "Joystick.h"

enum Difficulty { EASY, HARD };

struct Player {
    int x;
    int y;
};

struct Item {
    int x;
    int y;
    int type;      // 0,1,2: recyclable; 3: harmful
    bool active;
};


class RecyclerEngine {
public:
    RecyclerEngine();
    void init();
    int update(UserInput input);
    void draw(N5110 &lcd);
    void setDifficulty(Difficulty d);
    int get_score();


private:
    void move_player(UserInput input);
    void spawn_items();
    void update_items();
    void check_collisions();

    Player player;
    Item items[5];

    int score;
    int lives;
    int spawn_timer;
    int drop_speed;
    int harmful_chance;
    int last_shield_score; // 🛡️ Track score for adding shield
    Difficulty difficulty;
};

#endif

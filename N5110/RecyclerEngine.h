#ifndef RECYCLERENGINE_H
#define RECYCLERENGINE_H

#include "mbed.h"
#include "Joystick.h"
#include "N5110.h"

// Define a falling item object (could be recyclable or harmful)
struct Item {
    int x, y;
    int type;     // 0~2: recyclable, 3: harmful
    bool active;  // whether the item is falling
};

// Define the player (recycling cart)
struct Player {
    int x, y;
};

// Game logic engine class
class RecyclerEngine {
public:
    RecyclerEngine();
    void init();                          // Reset game state
    int update(UserInput input);          // Process game update each frame
    void draw(N5110 &lcd);                // Render to LCD

private:
    void move_player(UserInput input);    // Control player
    void spawn_items();                   // Randomly spawn new items
    void update_items();                  // Move items downward
    void check_collisions();              // Handle item collection or damage

    Player player;
    Item items[5];                        // Max 5 items on screen
    int score;
    int lives;
    int spawn_timer;
};

#endif

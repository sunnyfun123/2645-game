#include "RecyclerEngine.h"
#include "Joystick.h"

RecyclerEngine::RecyclerEngine() {}

void RecyclerEngine::init() {
    player.x = 42; player.y = 44;   // Center bottom
    score = 0; lives = 3;
    spawn_timer = 0;

    // Deactivate all items
    for (int i = 0; i < 5; i++) {
        items[i].active = false;
    }
}

int RecyclerEngine::update(UserInput input) {
    move_player(input);
    spawn_items();
    update_items();
    check_collisions();
    return lives;  // Report remaining lives
}

void RecyclerEngine::move_player(UserInput input) {
    if (input.direction == Direction::E && player.x < 78)
        player.x += 2;
    if (input.direction == Direction::W && player.x > 2)
        player.x -= 2;
}

void RecyclerEngine::spawn_items() {
    spawn_timer++;
    if (spawn_timer > 10) {
        for (int i = 0; i < 5; i++) {
            if (!items[i].active) {
                items[i].x = rand() % 80;
                items[i].y = 0;
                items[i].type = rand() % 4;
                items[i].active = true;
                break;
            }
        }
        spawn_timer = 0;
    }
}

void RecyclerEngine::update_items() {
    for (int i = 0; i < 5; i++) {
        if (items[i].active) {
            items[i].y += 2;
            if (items[i].y > 48) items[i].active = false;
        }
    }
}

void RecyclerEngine::check_collisions() {
    for (int i = 0; i < 5; i++) {
        if (items[i].active &&
            abs(items[i].x - player.x) < 5 &&
            items[i].y > 40) {
            if (items[i].type == 3) {
                lives--;
            } else {
                score += 5;
            }
            items[i].active = false;
        }
    }
}

void RecyclerEngine::draw(N5110 &lcd) {
    // Draw player cart
    lcd.drawRect(player.x, player.y, 6, 3, FILL_BLACK);

    // Draw items (circle for recyclable, square for harmful)
    for (int i = 0; i < 5; i++) {
        if (items[i].active) {
            if (items[i].type == 3)
                lcd.drawRect(items[i].x, items[i].y, 3, 3, FILL_BLACK);
            else
                lcd.drawCircle(items[i].x, items[i].y, 2, FILL_BLACK);
        }
    }

    // Display score and lives
    char buffer[14];
    sprintf(buffer, "Score:%d", score);
    lcd.printString(buffer, 0, 0);
    sprintf(buffer, "Lives:%d", lives);
    lcd.printString(buffer, 50, 0);
}

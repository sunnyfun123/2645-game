#include "RecyclerEngine.h"
#include "Joystick.h"

extern PwmOut buzzer;  // Use global passive buzzer

RecyclerEngine::RecyclerEngine() {}

void RecyclerEngine::setDifficulty(Difficulty d) {
    difficulty = d;

    if (d == EASY) {
        drop_speed = 2;
        harmful_chance = 25;
        lives = 3;
    } else {
        drop_speed = 4;
        harmful_chance = 60;
        lives = 1;
    }
}

void RecyclerEngine::init() {
    player.x = 42;
    player.y = 44;
    score = 0;
    spawn_timer = 0;

    for (int i = 0; i < 5; i++) {
        items[i].active = false;
    }
}

int RecyclerEngine::update(UserInput input) {
    move_player(input);
    spawn_items();
    update_items();
    check_collisions();
    return lives;
}

void RecyclerEngine::move_player(UserInput input) {
    if (input.direction == Direction::E && player.x < 78)
        player.x += 4;  // increased speed
    if (input.direction == Direction::W && player.x > 2)
        player.x -= 4;  // increased speed
}

void RecyclerEngine::spawn_items() {
    spawn_timer++;
    if (spawn_timer > 10) {
        for (int i = 0; i < 5; i++) {
            if (!items[i].active) {
                items[i].x = rand() % 80;
                items[i].y = 0;
                int chance = rand() % 100;
                items[i].type = (chance < harmful_chance) ? 3 : rand() % 3;
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
            items[i].y += drop_speed;  // speed depends on difficulty
            if (items[i].y > 48)
                items[i].active = false;
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

                // Louder 2.5kHz tone for 1 second
                buzzer.period(1.0 / 2500);
                buzzer.write(0.8);
                thread_sleep_for(1000);
                buzzer.write(0);
            } else {
                score += 5;
            }
            items[i].active = false;
        }
    }
}

void RecyclerEngine::draw(N5110 &lcd) {
    lcd.drawRect(player.x, player.y, 6, 3, FILL_BLACK);

    for (int i = 0; i < 5; i++) {
        if (items[i].active) {
            if (items[i].type == 3)
                lcd.drawRect(items[i].x, items[i].y, 3, 3, FILL_BLACK);
            else
                lcd.drawCircle(items[i].x, items[i].y, 2, FILL_BLACK);
        }
    }

    char buffer[14];
    sprintf(buffer, "Score:%d", score);
    lcd.printString(buffer, 0, 0);
    sprintf(buffer, "Lives:%d", lives);
    lcd.printString(buffer, 50, 0);
}
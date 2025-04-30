#include "mbed.h"
#include "N5110.h"
#include "Joystick.h"
#include "RecyclerEngine.h"

// Define difficulty levels

Difficulty difficulty = EASY;

// Hardware setup
N5110 lcd(PC_7, PA_9, PB_10, PB_5, PB_3, PA_10);  // LCD pins
Joystick joystick(PC_1, PC_0);                    // Joystick pins
DigitalIn buttonA(BUTTON1);                       // Blue on-board button (PC_13)
DigitalIn easyBtn(PC_10, PullDown);               // Easy mode button
DigitalIn hardBtn(PC_12, PullDown);               // Hard mode button

RecyclerEngine recycler;
PwmOut buzzer(PC_8);                              // Passive buzzer using PWM

void init();
void render();
void welcome();
void game_over();
void select_difficulty();

int main() {
    init();
    welcome();               // Show welcome screen, wait for blue button
    select_difficulty();     // Wait for easy/hard button press
    recycler.setDifficulty(difficulty); // Set difficulty for game
    render();

    int fps = 10;
    int lives = 3;

    while (lives > 0) {
        UserInput input = {joystick.get_direction(), joystick.get_mag()};
        lives = recycler.update(input);
        render();
        thread_sleep_for(1000 / fps);
    }

    game_over();
}

// Initialization of peripherals
void init() {
    lcd.init(LPH7366_1);
    lcd.setContrast(0.5);
    lcd.setBrightness(0.5);
    joystick.init();
    recycler.init();
}

// Render all visuals to LCD
void render() {
    lcd.clear();
    recycler.draw(lcd);
    lcd.refresh();
}

// Show welcome message and wait for blue button
void welcome() {
    lcd.clear();
    lcd.printString("Recycler Pioneer", 0, 1);
    lcd.printString("Press Blue Btn", 0, 3);
    lcd.refresh();

    while (buttonA.read() == 1) {
        thread_sleep_for(100);
    }
}

// Wait for difficulty selection via buttons
void select_difficulty() {
    lcd.clear();
    lcd.printString("Select Difficulty", 0, 0);
    lcd.printString("Easy -> PC_10", 0, 2);
    lcd.printString("Hard -> PC_12", 0, 3);
    lcd.refresh();

    while (true) {
        if (easyBtn.read() == 1) {
            difficulty = EASY;
            break;
        } else if (hardBtn.read() == 1) {
            difficulty = HARD;
            break;
        }
        thread_sleep_for(100);
    }
}

// Game over screen with 3s passive buzzer sound
void game_over() {
    lcd.clear();
    lcd.printString(" Game Over ", 0, 2);
    lcd.refresh();

    // Play louder 2.5kHz tone for 3 seconds
    buzzer.period(1.0 / 2500);
    buzzer.write(0.8);
    thread_sleep_for(3000);
    buzzer.write(0);         // stop sound

    while (1);
}
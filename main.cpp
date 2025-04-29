#include "mbed.h"
#include "N5110.h"
#include "Joystick.h"
#include "RecyclerEngine.h"

// Initialize the LCD display with specific pin connections
N5110 lcd(PC_7, PA_9, PB_10, PB_5, PB_3, PA_10);

// Initialize joystick with vertical and horizontal analog input pins
Joystick joystick(PC_1, PC_0);

// Use built-in blue button on the Nucleo board
DigitalIn buttonA(BUTTON1);

// Game logic handler
RecyclerEngine recycler;

void init();       // Setup all systems
void render();     // Draw the game frame
void welcome();    // Display welcome screen
void game_over();  // Handle game over display

int main() {
    init();
    welcome();
    render();

    int fps = 10;
    int lives = 3;

    // Main game loop: updates state every frame
    while (lives > 0) {
        UserInput input = {joystick.get_direction(), joystick.get_mag()};
        lives = recycler.update(input);  // Update game logic and get remaining lives
        render();
        thread_sleep_for(1000 / fps);  // Control frame rate
    }

    game_over();
}

void init() {
    lcd.init(LPH7366_1);       // Initialize LCD (specific controller type)
    lcd.setContrast(0.5);      // Adjust contrast
    lcd.setBrightness(0.5);    // Adjust backlight
    joystick.init();           // Calibrate joystick center
    recycler.init();           // Initialize game engine state
}

void render() {
    lcd.clear();
    recycler.draw(lcd);        // Render game elements to LCD
    lcd.refresh();
}

void welcome() {
    lcd.clear();
    lcd.printString("Recycler Pioneer", 0, 1);
    lcd.printString("Press Button", 0, 3);
    lcd.refresh();

    while (buttonA.read() == 1) {
        ThisThread::sleep_for(100ms);  // Wait until button is pressed
    }
}

void game_over() {
    lcd.clear();
    lcd.printString(" Game Over ", 0, 2);
    lcd.refresh();
    while (1);  // Stop execution
}

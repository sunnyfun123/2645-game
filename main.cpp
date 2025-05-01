#include "mbed.h"
#include "N5110.h"
#include "Joystick.h"
#include "RecyclerEngine.h"
#include "SevenSegmentDisplay.h"

Difficulty difficulty = EASY;

N5110 lcd(PC_7, PA_9, PB_10, PB_5, PB_3, PA_10);
Joystick joystick(PC_1, PC_0);
DigitalIn buttonA(BUTTON1);
DigitalIn easyBtn(PC_10, PullDown);
DigitalIn hardBtn(PC_12, PullDown);
DigitalIn resetBtn(PC_15, PullUp);  // Reset button
DigitalOut led(PC_9);
PwmOut buzzer(PC_8);

SevenSegmentDisplay display(PB_9, PB_8, PB_13, PB_14, PB_15, PA_12, PA_11);
RecyclerEngine recycler;

void init();
void render();
void welcome();
void select_difficulty();
void play_melody();  // 

int main() {
    int state = 0;
    int lives = 3;
    int fps = 10;

    init();

    while (true) {
        if (state == 0) {
            welcome();
            state = 1;
        }

        if (state == 1) {
            select_difficulty();
            recycler.setDifficulty(difficulty);
            recycler.init();
            lcd.clear();
            state = 2;
        }

        if (state == 2) {
            UserInput input = {joystick.get_direction(), joystick.get_mag()};
            lives = recycler.update(input);
            render();
            thread_sleep_for(1000 / fps);
            if (lives <= 0) {
                state = 3;
            }
        }

        if (state == 3) {
            lcd.clear();
            lcd.printString("Game Over", 15, 1);

            int final_score = recycler.get_score();

            if (final_score < 50) {
                lcd.printString("Do more to", 0, 2);
                lcd.printString("protect env", 0, 3);
            } else if (final_score < 100) {
                lcd.printString("Nice try,", 0, 2);
                lcd.printString("keep going!", 0, 3);
            } else if (final_score < 200) {
                lcd.printString("You're making", 0, 2);
                lcd.printString("a difference!", 0, 3);
            } else {
                lcd.printString("Amazing!", 0, 2);
                lcd.printString("True recycling", 0, 3);
                lcd.printString("pioneer!", 0, 4);

                play_melody();  // play melody if high score
            }

            lcd.printString("Press Btn", 18, 5);
            lcd.refresh();

            //  Louder end beep (1000Hz, 3s)
            buzzer.period(1.0 / 1000);
            buzzer.write(0.8);
            thread_sleep_for(3000);
            buzzer.write(0);

            while (resetBtn.read() == 1) {
                thread_sleep_for(100);
            }

            lcd.clear();
            state = 0;
        }
    }
}

void init() {
    lcd.init(LPH7366_1);
    lcd.setContrast(0.5);
    lcd.setBrightness(0.5);
    joystick.init();
    recycler.init();
    led = 0;
}

void render() {
    lcd.clear();
    recycler.draw(lcd);
    lcd.refresh();
}

void welcome() {
    lcd.clear();
    lcd.printString("Recycler Pioneer", 0, 1);
    lcd.printString("Press Blue Btn", 0, 3);
    lcd.refresh();

    while (buttonA.read() == 1) {
        led = !led;
        thread_sleep_for(300);
    }
    led = 0;
}

void select_difficulty() {
    lcd.clear();
    lcd.printString("Select Difficulty", 0, 0);
    lcd.printString("Easy -> Btn1", 0, 2);
    lcd.printString("Hard -> Btn2", 0, 3);
    lcd.refresh();

    while (true) {
        led = !led;
        if (easyBtn.read() == 1) {
            difficulty = EASY;
            break;
        } else if (hardBtn.read() == 1) {
            difficulty = HARD;
            break;
        }
        thread_sleep_for(300);
    }
    led = 0;
}

//  Melody for score ≥ 200
void play_melody() {
    int notes[] = {262, 294, 330, 392, 294, 392, 330}; // C D E G D G E
    int duration[] = {300, 300, 300, 400, 300, 400, 500};

    for (int i = 0; i < 7; i++) {
        buzzer.period(1.0 / notes[i]);
        buzzer.write(0.6);  // suitable duty for passive buzzer
        thread_sleep_for(duration[i]);
        buzzer.write(0);
        thread_sleep_for(100);  // gap between notes
    }
}
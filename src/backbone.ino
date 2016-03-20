/*
 * Author : Aurélie Suzanne
 * Created : 02-29-2016
 */

#include <LiquidCrystal.h>
LiquidCrystal lcd(12, 11, 6, 5, 4, 3);

// Input / output pins
int ledG = 7;
int ledR = 13;
int reset = 2;
int speakerA = 8;
int speakerB = 9;

// Global variables
int printNb = 0;
int won = 0;

void setup(){
    Serial.begin(9600);

    pinMode(reset, INPUT);
    pinMode(ledG, OUTPUT);
    pinMode(ledR, OUTPUT);
    pinMode(speakerA, OUTPUT);
    pinMode(speakerB, OUTPUT);

    setup_interruption();

    lcd.begin(16, 2);
}

void loop(){
    if (printNb == 0) {
      lcd.setCursor(0,0);
      lcd.print("Find Zelda Song"); // max 16 caracters
      lcd.setCursor(0,1);
      lcd.print("and you'll see");
      printNb = 1;
    }
    loop_zelda_lullaby();
}

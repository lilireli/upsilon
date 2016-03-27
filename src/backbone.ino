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
int rows[8] = {6, 12, 13, 3, 18, 4, 8, 9};
int cols[8] = {19, 11, 17, 10, 5, 16, 7, 2};

// Global variables
int printNb = 0;
int won = 0;

void setup(){
    Serial.begin(9600);

    /*pinMode(reset, INPUT);
    pinMode(ledG, OUTPUT);
    pinMode(ledR, OUTPUT);
    pinMode(speakerA, OUTPUT);
    pinMode(speakerB, OUTPUT);

    setup_interruption();

    lcd.begin(16, 2);*/

    // setup for 8 by 8 led matrix
    for (int i=0; i<8; i++){
      // set cols to high
      pinMode(cols[i], OUTPUT);
      digitalWrite(cols[i], LOW);

      // set rows to low
      pinMode(rows[i], OUTPUT);
      digitalWrite(rows[i], HIGH);
    }
}

void loop(){
    /*if (printNb == 0) {
      lcd.setCursor(0,0);
      lcd.print("Find Zelda Song"); // max 16 caracters
      lcd.setCursor(0,1);
      lcd.print("and you'll see");
      printNb = 1;
    }
    loop_zelda_lullaby();*/
    loop_8_by_8_led();
}

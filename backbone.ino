/*
 * Author : Aurélie Suzanne
 * Created : 02-29-2016
 */

#include <LiquidCrystal.h>
LiquidCrystal lcd(12, 11, 6, 5, 4, 3);

int speakerA = 8;
int speakerB = 9;
int reset = 2;

void setup(){
    Serial.begin(9600);
    
    pinMode(speakerA, OUTPUT);
    pinMode(speakerB, OUTPUT);
    pinMode(reset, INPUT);
    
    setup_interruption();
    
    lcd.begin(16, 2);
    lcd.print("Find Zelda Song"); // max 16 caracters
    lcd.setCursor(0,1);
    lcd.print("and you'll see");
}

void loop(){
    loop_zelda_lullaby();
}
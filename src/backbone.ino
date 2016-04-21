/*
 * Author : Aurélie Suzanne
 * Created : 02-29-2016
 */

#include <LiquidCrystal.h>

/*
 * Input / output pins
 */
// Analog inputs
int pianoEntry = A0;
int selectRow = A1;
int selectCol = A2;

// Digital inputs/outputs
LiquidCrystal lcd(7, 6, 5, 4, 3, 2);
int speakerA = 8;
int speakerB = 9;
int speakerC = 10;
int ledG = 11;
int ledR = 12;
int cols[3] = {22, 23, 24};
int allow_cols = 25;
int rows[8] = {26, 27, 28, 29, 30, 31, 32, 33};

// counter
unsigned int count = 0; // for the interruption service
int freq_interrupt = 8000; // interruption frequency is 8000kHz
int resetCount = 80000000; // number of count until the counter is reset

// Global variables
int printNb = 0;
int won = 0;

// Hijack the interrupt function
void setup_interruption() {
  //set timer2 interrupt at 8kHz
  TCCR2A = 0;// set entire TCCR2A register to 0
  TCCR2B = 0;// same for TCCR2B
  TCNT2  = 0;//initialize counter value to 0
  // set compare match register for 8khz increments
  //  compare match register = [ 16,000,000Hz/ (prescaler * desired interrupt frequency) ] - 1
  //remember that when you use timers 0 and 2 this number must be less than 256, and less than 65536 for timer1
  OCR2A = 249;// = (16*10^6) / (8000*8) - 1 (must be <256)
  // turn on CTC mode
  TCCR2A |= (1 << WGM21);
  // Set CS21 bit for 8 prescaler
  TCCR2B |= (1 << CS21);
  // enable timer compare interrupt
  TIMSK2 |= (1 << OCIE2A);

  sei();//allow interrupts
}

void setup(){
    Serial.begin(9600);

    pinMode(ledG, OUTPUT);
    pinMode(ledR, OUTPUT);
    pinMode(speakerA, OUTPUT);
    pinMode(speakerB, OUTPUT);
    pinMode(speakerC, OUTPUT);

    setup_interruption();

    lcd.begin(16, 2);

    // setup for 8 by 8 led matrix
    for (int i=0; i<3; i++){
      // set cols to high
      pinMode(cols[i], OUTPUT);
      digitalWrite(cols[i], LOW);
    }
    pinMode(allow_cols, OUTPUT);
    digitalWrite(allow_cols, LOW);

    for (int i=0; i<8; i++){
      // set rows to low
      pinMode(rows[i], OUTPUT);
      digitalWrite(rows[i], LOW);
    }
}

void loop(){
    if (won == 0) {
        if (printNb == 0) {
            lcd.setCursor(0,0);
            lcd.print("Find Zelda Song"); // max 16 caracters
            lcd.setCursor(0,1);
            lcd.print("and you'll see");
            printNb = 1;
        }
        loop_zelda_lullaby();
    }
    else if (won == 1) {
        if (printNb == 1) {
            lcd.setCursor(0,0);
            lcd.print("Find the        ");
            lcd.setCursor(0,1);
            lcd.print("mushroom        ");
            printNb = 2;
        }
        loop_8_by_8_led();
    }
}

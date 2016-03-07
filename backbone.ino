/*
 * Author : Aurélie Suzanne
 * Created : 02-29-2016
 */

int speakerA = 8;
int speakerB = 9;
int reset = 2;

void setup(){
    Serial.begin(9600);
    
    pinMode(speakerA, OUTPUT);
    pinMode(speakerB, OUTPUT);
    pinMode(reset, INPUT);
    
    setup_interruption();
}

void loop(){
    loop_zelda_lullaby();
}
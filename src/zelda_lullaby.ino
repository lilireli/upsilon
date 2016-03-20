/*
 * Author : Aurélie Suzanne
 * Created : 02-29-2016
 */

unsigned int count = 0;
unsigned int quaver_in_song = 0;
int way_to_win = 0;
int freq_interrupt = 8000; // interruption frequency is 8000kHz
int resetCount = 80000000; // number of count until the counter is reset

/*
 * Zelda song
 */
int song[] = {
     440, 440, 440, 440, 440, 440,
     494, 494, 494, 494, 587, 587,
     880, 880, 880, 880, 784, 784,
     587, 587, 587, 587, 524, 494,
     440, 440, 440, 440, 440, 440,
     494, 494, 494, 494, 587, 587,
     440, 440, 440, 440, 392, 440,
     494, 494, 494, 494, 587, 587,
     440, 440, 440, 440, 440, 440,
     494, 494, 494, 494, 587, 587,
     880, 880, 880, 880, 784, 784,
     1175, 1175, 1175, 1175, 1175, 1175,
     1175, 1175, 1175, 1175, 1175, 1175, 1175, 0,
     1175, 1175, 1175, 1175, 1046, 988, 1046, 988,
     784, 784, 784, 784, 1046, 1046, 1046, 1046,
     988, 880, 988, 880, 659, 659, 659, 659,
     1175, 1175, 1175, 1175, 1046, 988, 1046, 988,
     784, 784, 1046, 1046, 1568, 1568, 1568, 1568,
     1568, 1568, 1568, 1568, 1568, 1568, 1568, 1568
    };

int len_song = sizeof(song) / sizeof(song[0]);

/*
 * intial notes are C4, D4, E4, F4
 * in frequencies those are {262, 294, 330, 349} (freq)
 * interruptions will be at a frequence of 8kHz (interrupt)
 * to have one note we have to count (interrup/freq)
 */
int notes[] = {349, 330, 294, 262}; //reverse order to make it in real order int notes[] = {262, 294, 330, 349};
int play_notes[] = {0, 0, 0, 0};
int len_notes = 4;
int winning_sequence[] = {2, 0, 3, 2, 0, 3};
int len_win = sizeof(winning_sequence) / sizeof(winning_sequence[0]);
int past_note = -1;

// choose if piezo is up or down
// first toggle is for first piezo and real note
// second toggle is for second piezo and second harmonic
int toggle1 = 0, toggle2 = 0;

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

/*
 * Play notes and stop playing
 */
void play_one_note(int note){
    int freq_note = freq_interrupt / note;

    if (count % freq_note == 0) {
        toggle1 = !toggle1;
        digitalWrite(speakerA, toggle1 == 0 ? HIGH : LOW);
    }

    if (count % freq_note/2 == 0) { // construct the first harmonic
        toggle2 = !toggle2;
        digitalWrite(speakerB, toggle2 == 0 ? HIGH : LOW);
    }
}

void stop_sound(){
    digitalWrite(speakerA, LOW);
    digitalWrite(speakerB, LOW);
}

/*
 * Functions working during interruptions
 */
void piano_sound(){
    for (int i=0; i<len_notes; i++){
        if (play_notes[i] == 1) {
            play_one_note(notes[i]);
        }
    }
}

void zelda_lullaby_sound(){
    if (quaver_in_song < len_song) {
        if (count % (freq_interrupt/6) == 0){ // we divide by two to get quaver and by two to get tempo (tempo is 120)
            quaver_in_song += 1;
        }
        if (quaver_in_song + 1 < len_song){
            // make some pause between the notes
            if ((song[quaver_in_song+1] != song[quaver_in_song]) &&
                (count % (freq_interrupt/6) < 20))
            {
                stop_sound();
            }
            else play_one_note(song[quaver_in_song]);
        }
        else play_one_note(song[quaver_in_song]);
    }
    else {
        stop_sound();
    }
}

ISR (TIMER2_COMPA_vect) {
//This is the interrupt service routine for TIMER2 Interrupt
    count++; //Increment our count variable
    if (count==resetCount) { count=0; }

    if (won==0) piano_sound();
    else if (won==1) zelda_lullaby_sound();
}

/*
 * Main loop
 */
void loop_zelda_lullaby() {
    int resetState = digitalRead(reset);

    if (resetState == HIGH){ // reset button
        Serial.print("Reset");
        won = 0;
        way_to_win = 0;
        for(int i=0; i<len_notes; i++) play_notes[i] = 0;
        stop_sound();
        quaver_in_song = 0;
    }

    int keyVal = analogRead(A0);
    int note = -1;
    //Serial.print("Note is "); Serial.println(keyVal);

    // get the value of the resistance and buttons
    if (keyVal > 1010) note = 0;
    else if (keyVal >= 990 && keyVal <= 1010) note = 1;
    else if (keyVal >= 490 && keyVal <= 520) note = 2;
    else if (keyVal >= 7 && keyVal <= 20) note = 3;
    else if (keyVal == 0) note = -1;
    else note = past_note;

    if (note != -1){ // one button is activated
        play_notes[note] = 1; // play the note

        if (past_note != note) { // check the winning sequence
            way_to_win = winning_sequence[way_to_win] == note ? way_to_win + 1 : 0;
            if (way_to_win > 0) {
              digitalWrite(ledG, HIGH);
              digitalWrite(ledR, LOW);
            }
            else {
              digitalWrite(ledR, HIGH);
              digitalWrite(ledG, LOW);
            }
            Serial.print("note :"); Serial.print(note);
            Serial.print("button value :"); Serial.print(keyVal);
            Serial.print("new note :"); Serial.println(way_to_win);
        }
    }
    else { // no button activated
        for(int i=0; i<len_notes; i++) play_notes[i] = 0;
        stop_sound();
    }

    past_note = note; // keep trace of old value

    if (way_to_win == len_win) won = 1; // the first notes of the lullaby have been found,
                                        // can continue to zelda lullaby
}

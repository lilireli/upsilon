/**
* @Author: Aurelie Suzanne <aurelie>
* @Date:   29-Feb-2016
* @Project: Upsilon
* @Last modified by:   aurelie
* @Last modified time: 24-Aug-2016
*/

/*
 * Variables to define temporality
 */
unsigned int quaver_in_song = 0;
int way_to_win = 0;
int won_zelda = 0;

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
int nb_past_note = 0;

// choose if piezo is up or down
// first toggle is for first piezo and real note
// second toggle is for second piezo and second harmonic
int toggle1 = 0, toggle2 = 0, toggle3 = 0;

/*
 * Play notes and stop playing
 */
void play_one_note(int note){
    int freq_note = freq_interrupt / note;

    if (count % freq_note == 0) {
        toggle1 = !toggle1;
        digitalWrite(speakerA, toggle1 == 0 ? HIGH : LOW);
    }

    if (count % freq_note/2 == 0) { // construct the first octave harmonic
        toggle2 = !toggle2;
        digitalWrite(speakerB, toggle2 == 0 ? HIGH : LOW);
    }

    if (count % freq_note/3 == 0) { // construct the second octave harmonic
        toggle3 = !toggle3;
        digitalWrite(speakerC, toggle3 == 0 ? HIGH : LOW);
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
        won += 1;
    }
}

/*
 * New interruption function
 */
ISR (TIMER2_COMPA_vect) {
//This is the interrupt service routine for TIMER2 Interrupt
    count++; //Increment our count variable
    if (count==resetCount) { count=0; }

    if (won_zelda==0) piano_sound();
    else if (won_zelda==1) zelda_lullaby_sound();
}

/*
 * Main loop
 */
void loop_zelda_lullaby() {
    int keyVal = analogRead(pianoEntry);
    int note = -1;
    //Serial.println(keyVal);

    // get the value of the resistance and buttons
    /*if (keyVal > 1010) note = 0;
    else if (keyVal >= 990 && keyVal <= 1010) note = 1;
    else if (keyVal >= 490 && keyVal <= 520) note = 2;
    else if (keyVal >= 80 && keyVal <= 100) note = 3;
    else if (keyVal == 0) note = -1;
    else note = past_note;*/
    double resistance[4] = {0.0, 0.56, 10.0, 100.0};
    note = getButton(resistance, 4 , keyVal, past_note);

    if (note != -1){ // one button is activated
        play_notes[note] = 1; // play the note

        if (nb_past_note == 100) { // check the winning sequence when the note is activated for at least 100 loops
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

    nb_past_note = note == past_note ? nb_past_note + 1 : 0; // keep trace of how long have we heard the last_note
    past_note = note; // keep trace of old value

    if (way_to_win == len_win) won_zelda = 1; // the first notes of the lullaby have been found,
                                        // can continue to zelda lullaby
}

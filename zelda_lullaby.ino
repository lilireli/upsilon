/*
 * Author : Aurélie Suzanne
 * Created : 02-29-2016
 */

unsigned int count = 0;
unsigned int quaver_in_song = 0;
int freq_interrupt = 8000; // interruption frequency is 8000kHz
int resetCount = 80000000; // reset each second

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
int won = 0;
int way_to_win = 0;
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

void piano_sound(){
    for (int i=0; i<len_notes; i++){
        if (play_notes[i] == 1) {
            play_one_note(notes[i]);
        }
    }
}

void zelda_lullaby_sound(){
    int song[] = 
        {494, 494, 494, 494, 587, 587, 
         880, 880, 880, 880, 784, 784, 
         587, 587, 587, 587, 524, 494, 
         440, 440, 440, 440, 440, 440};
    int len_song = sizeof(song) / sizeof(song[0]);
    
    if (count % freq_interrupt/6 == 0){ // we divide by two to get quaver and by two to get tempo (tempo is 120)
        quaver_in_song += 1;
    }
    
    if (quaver_in_song < len_song) { play_one_note(song[quaver_in_song]); }
    else { quaver_in_song = 0; }
}

ISR (TIMER2_COMPA_vect) {
//This is the interrupt service routine for TIMER2 Interrupt
    count++; //Increment our count variable
    if (count==resetCount) { count=0; }
    
    if(won==0){ piano_sound(); }
    else { zelda_lullaby_sound(); }
}

void loop_zelda_lullaby() {
    int resetState = digitalRead(reset);
    
    if (resetState == HIGH){
        won = 0;
        way_to_win = 0;
    }
    
    int keyVal = analogRead(A0);
    int note = -1;
    //Serial.print("Note is ");
    //Serial.println(keyVal);
    
    if (keyVal > 1010) note = 0;
    else if (keyVal >= 990 && keyVal <= 1010) note = 1;
    else if (keyVal >= 490 && keyVal <= 520) note = 2;
    else if (keyVal >= 7 && keyVal <= 20) note = 3;
    else note = -1;
    
    if (note != -1){
        play_notes[note] = 1;
        
        if (past_note != note) {
            way_to_win = winning_sequence[way_to_win] == note ? way_to_win + 1 : 0;
            Serial.print("note :");
            Serial.print(note);
            Serial.print("button value :");
            Serial.print(keyVal);
            Serial.print("new note :");
            Serial.println(way_to_win);
        }
    } 
    else { 
        for(int i=0; i<len_notes; i++) play_notes[i] = 0;
        digitalWrite(speakerA, LOW);
        digitalWrite(speakerB, LOW);
    }
    
    past_note = note;
    
    if (way_to_win == len_win) won = 1; // the first notes of the lullaby have been found, 
                                        // can continue to zelda lullaby
}
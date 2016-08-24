/*
* Author : Aurélie Suzanne
* Created : 03-21-2016
*/

int mushroom_matrix[] = {
    0, 0, 1, 1, 1, 1, 0, 0,
    0, 1, 1, 1, 0, 1, 1, 0,
    1, 1, 1, 1, 1, 0, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1,
    0, 1, 1, 1, 1, 1, 1, 0,
    0, 0, 1, 0, 0, 1, 0, 0,
    0, 0, 1, 0, 0, 1, 0, 0,
    0, 0, 0, 1, 1, 0, 0, 0
};

int playing_matrix[] = {
    0, 0, 1, 1, 1, 0, 1, 1,
    0, 1, 1, 1, 0, 0, 0, 1,
    1, 1, 1, 1, 1, 1, 0, 0,
    1, 1, 1, 1, 1, 1, 1, 1,
    0, 1, 1, 1, 1, 1, 1, 0,
    0, 0, 1, 0, 0, 1, 0, 0,
    0, 0, 1, 0, 0, 1, 0, 0,
    0, 0, 0, 1, 1, 0, 0, 0
};

int size_square = 8;

// numbers for a one digit LED from a to g
// each row corresponds to the number
int digitNumbers[10][7] = {
    {1, 1, 1, 1, 1, 1, 0},
    {0, 1, 1, 0, 0, 0, 0},
    {1, 1, 0, 1, 1, 0, 1},
    {1, 1, 1, 1, 0, 0, 1},
    {0, 1, 1, 0, 0, 1, 1},
    {1, 0, 1, 1, 0, 1, 1},
    {1, 0, 1, 1, 1, 1, 1},
    {1, 1, 1, 0, 0, 0, 0},
    {1, 1, 1, 1, 1, 1, 1},
    {1, 1, 1, 1, 0, 1, 1}
};

int size_digit = 7;

int lifes = 3;
int won_mario = 0;

void print_matrix(int matrix[], int size_square){
    // we need row LOW and col HIGH to have led on
    for (int i=0; i<size_square; i++) {
        digitalWrite(allow_cols, LOW); // unallow write to be seen
        for(int n=0; n<3; n++){
            if((i% (int)pow(2, n+1) != 0) && (i-(int)pow(2, n) >= 0)){
                digitalWrite(cols[n], HIGH);
            }
            else {
                digitalWrite(cols[n], LOW);
            }
        }
        digitalWrite(allow_cols, HIGH); // allow write to be seen

        for (int j=0; j<size_square; j++) {
            int pixel = matrix[i*(size_square) + j];
            if ( pixel == 1){
                digitalWrite(rows[j], HIGH); // write right row
            }

            digitalWrite(rows[j], LOW); // delete write
        }
    }
}

void print_number(int number){
    for(int i=0; i<size_digit; i++){
        digitalWrite(digit[i], digitNumbers[number][i] == 1 ? HIGH : LOW);
    }
}

void invert_playing_matrix(int row, int col){
    int i_begin=0, i_end=0, j_begin=0, j_end=0;

    i_begin = row > 0 ? row - 1 : 0;
    i_end = row < 7 ? row + 1: 7;
    j_begin = col > 0 ? col - 1 : 0;
    j_end = col < 7 ? col + 1 : 7;

    for (int i=i_begin; i<=i_end; i++) {
        for (int j=j_begin; j<=j_end; j++){
            playing_matrix[i*(size_square) + j] = !playing_matrix[i*(size_square) + j];
        }
    }
}

int compare_matrixes(){
    int same = 1;

    for(int i=0; i < size_square*size_square; i++){
        if(playing_matrix[i] != mushroom_matrix[i]){
            same = 0;
        }
    }

    return same;
}

void loop_8_by_8_led(){
    int keyValRow = analogRead(selectRow);
    int keyValCol = analogRead(selectCol);
    int photoVal = analogRead(photoRes);

    print_number(lifes);

    if(photoVal < 400){
        print_matrix(mushroom_matrix, 8);
    }
    else {
        int rowSelect = -1, colSelect = -1,rowPast = -1, colPast = -1;
        int rowPlayed = -1, colPlayed = -1;

        double resistance[8] = {0.0, 0.22, 0.56, 1.0, 4.7, 10.0, 47.0, 100.0};

        rowSelect = getButton(resistance, 8, keyValRow, rowPast);
        if (rowSelect != -1){ // one button is activated
            Serial.print("Button"); Serial.println(rowSelect);

            if (rowPast == -1) { // it has just been activated
                rowPlayed = rowSelect;
            }
        }
        rowPast = rowSelect;

        colSelect = getButton(resistance, 8, keyValCol, colPast);
        if (colSelect != -1){ // one button is activated
            if (colPast == -1) { // it has just been activated
                colPlayed = colSelect;
            }
        }
        colPast = colSelect;

        if(rowPlayed >= 0 && colPlayed >= 0){
            Serial.print("Row"); Serial.print(rowPlayed); Serial.print(" ,Col"); Serial.println(colPlayed);
            invert_playing_matrix(rowPlayed, colPlayed);
            lifes -= 1;
            colPlayed = -1;
            rowPlayed = -1;
        }

        print_matrix(playing_matrix, 8);

        if(lifes == 0){
            won -= 1;
        }

        won_mario = compare_matrixes();
        if(won_mario == 1){
            won += 1;
        }
    }
}

/*
 * Author : Aurélie Suzanne
 * Created : 03-21-2016
 */

 int matrix[] = {
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
   1, 1, 1, 1, 1, 1, 1, 1,
   1, 1, 1, 1, 1, 1, 1, 1,
   1, 1, 1, 1, 1, 1, 1, 1,
   1, 1, 1, 1, 1, 1, 1, 1,
   1, 1, 1, 1, 1, 1, 1, 1,
   1, 1, 1, 1, 1, 1, 1, 1,
   1, 1, 1, 1, 1, 1, 1, 1,
   1, 1, 1, 1, 1, 1, 1, 1
 };

int size_square = 8;

void print_matrix(int matrix[], int size_square){
  // we need row LOW and col HIGH to have led on
  for (int i=0; i<size_square; i++) {
    digitalWrite(rows[i], LOW);

    for (int j=0; j<size_square; j++) {
      int pixel = matrix[i*(size_square) + j];
      if ( pixel == 1){
        digitalWrite(cols[j], HIGH);
        digitalWrite(cols[j], LOW);
      }
    }

    digitalWrite(rows[i], HIGH);
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

void loop_8_by_8_led(){
  int keyValRow = analogRead(selectRow);
  int keyValCol = analogRead(selectCol);

  int rowSelect = -1, colSelect = -1,rowPast = -1, colPast = -1;
  int rowPlayed = -1, colPlayed = -1;

  //int i = 0;
  // while(i<10000){
  //   print_matrix(matrix, 8);
  //   i++;
  // }

  // resistances are in order 100 Ohm, 47k Ohm, 100k Ohm
  // formula to get trigger is (5 - (5*r)/(r+10)) / 5 * 1023
  // with 10 being the second resistance and r the resistance in k Ohm
  if (keyValRow > 1020) rowSelect = 0;
  else if (keyValRow >= 1000 && keyValRow <= 1020) rowSelect = 1;
  else if (keyValRow >= 170 && keyValRow <= 190) rowSelect = 2;
  else if (keyValRow >= 80 && keyValRow <= 100) rowSelect = 3;
  else if (keyValRow == 0) rowSelect = -1;
  else rowSelect = rowPast;

  if (rowSelect != -1){ // one button is activated
      Serial.print("Button"); Serial.println(rowSelect);

      if (rowPast == -1) { // it has just been activated
          rowPlayed = rowSelect;
      }
  }

  rowPast = rowSelect;

  if (keyValCol > 1020) colSelect = 0;
  else if (keyValCol >= 1000 && keyValCol <= 1020) colSelect = 1;
  else if (keyValCol >= 170 && keyValCol <= 190) colSelect = 2;
  else if (keyValCol >= 80 && keyValCol <= 100) colSelect = 3;
  else if (keyValCol == 0) colSelect = -1;
  else colSelect = colPast;

  if (colSelect != -1){ // one button is activated
      if (colPast == -1) { // it has just been activated
          colPlayed = colSelect;
      }
  }

  colPast = colSelect;

  if(rowPlayed >= 0 && colPlayed >= 0){
      Serial.print("Row"); Serial.print(rowPlayed); Serial.print(" ,Col"); Serial.println(colPlayed);
      invert_playing_matrix(rowPlayed, colPlayed);
      colPlayed = -1;
      rowPlayed = -1;
  }
  print_matrix(playing_matrix, 8);

}

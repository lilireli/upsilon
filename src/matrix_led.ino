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
  int i = 0;

  while(i<10000){
    print_matrix(matrix, 8);
    i++;
  }

  invert_playing_matrix(4, 4);

  i=0;
  while(i<10000){
    print_matrix(playing_matrix, 8);
    i++;
  }
}

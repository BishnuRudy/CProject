#include<stdio.h>
#include<ctype.h>
#include<time.h>
#include<stdlib.h>
#include<stdbool.h>

#define N_ROWS 10
#define N_COLS 10
#define LEFT   0
#define UP    1
#define DOWN   2
#define RIGHT  3
#define NUM_MOVES  4
#define EMPTY_CELL '.'

int move;
bool potential_move[4];
bool is_movement_avilable;
bool bound_not_crossed;
char board[N_ROWS][N_COLS];

void fill_board(void);
void random_walk(void);
void print_board(void);
int generate_move(void);

int main() {
   srand((unsigned) time(NULL));
   fill_board();
   random_walk();
   print_board();
   return 0;
}

void random_walk(void) {
   char letter;
   int row, col, i;

   row = 0; col = 0;

   for (letter = 'A'; letter <= 'Z'; letter++) {
      for (i = 0; i < NUM_MOVES; potential_move[i]=false,i++);
      is_movement_avilable = false;
      board[row][col] = letter;

      if (row+1 <= N_ROWS-1 && board[row+1][col] == '.') {
         potential_move[DOWN] = true;
         is_movement_avilable = true;
      }
      if (row-1 >= 0 && board[row-1][col] == '.') {
         potential_move[UP] = true;
         is_movement_avilable = true;
      }
      if (col+1 <= N_COLS-1 && board[row][col+1] == '.') {
         potential_move[RIGHT] = true;
         is_movement_avilable = true;
      }
      if (col-1 >= 0 && board[row][col-1] == '.') {
         potential_move[LEFT] = true;
         is_movement_avilable = true;
      }

      if (!is_movement_avilable)
         break;

      generate_move();

      switch (move) {
         case LEFT:
            col--; break;
         case UP:
            row--; break;
         case DOWN:
            row++; break;
         case RIGHT:
            col++; break;

      }
   }
}

void fill_board(void) {
   for (int i = 0; i < N_ROWS; i++) {
      for (int j = 0; j < N_COLS; j++) {
         board[i][j] = EMPTY_CELL;
      }
   }
}

void print_board(void) {
   putchar('\n');
   for (int i = 0; i < N_ROWS; i++) {
      for (int j = 0; j < N_COLS; j++) {
         printf("%c ", board[i][j]);
      }
      putchar('\n');
   }
}

int generate_move(void) {
   do {
      move = rand() % NUM_MOVES;
   } while (!potential_move[move]);

   return move;
}

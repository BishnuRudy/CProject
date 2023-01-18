#include<time.h>
#include<ctype.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<stdbool.h>

#define NUM_ROWS 3
#define NUM_COLS 5
#define NUM_CELLS 9

#define cint(n) n-'0'

/*
   ----------------------------------------------
         inializers and ui functions
   ----------------------------------------------
*/
void instruction(void);
void initialize_attrs(void);
void display_board(void);
void initialize_board(void);

/*
   -----------------------------------------------
         location and location tester function
   -----------------------------------------------
*/
int *allocate_position(char ch);
bool is_valid_pos(int row, int col);
bool cmp_cell_r(int r, int c1, int c2, int c3);
bool cmp_cell_c(int c, int r1, int r2, int r3);
bool cmp_cell_diag(void);
int check_cell_move(char ch);
int check_cell_move_right(char ch);
/*
   ------------------------------------------------
           player and ai gameplay functions
   ------------------------------------------------
*/
int move_generator(void);
int generate_hard_move(void);
void ai_play(void);
void play(void);

/*
   -------------------------------------------------
               winner checker functions
                 and other functions
   -------------------------------------------------
*/
void check_win_status(void);
bool any(char ch, char ch1, char ch2, char ch3);
/*
   -------------------------------------------------
               board at very initial state
   -------------------------------------------------
*/
char board[NUM_ROWS][NUM_COLS] = {
                     {'_', '|', '_', '|', '_'},
                     {'_', '|', '_', '|', '_'},
                     {' ', '|', ' ', '|', ' '}
                   };

/*
   --------------------------------------------------
               ch for user input
   --------------------------------------------------
*/
char ch, choice;

int space, player_score, ai_score, ai_win_count, player_win_count;
bool hard_mode;

int main() {
   printf("%d", any('1', '2', '3', '4'));
   srand((unsigned) time(NULL));
   instruction();
   play();
   display_board();
   return 0;
}


void instruction(void) {
   printf("%60s","Enter respective numbers to fill....\n");
   char board_temp[NUM_ROWS][NUM_COLS] = {
                     {'1', '|', '2', '|', '3'},
                     {'4', '|', '5', '|', '6'},
                     {'7', '|', '8', '|', '9'}
                   };
   for (int i = 0; i < NUM_ROWS; i++) {
      printf("%35s", " ");
      for (int j = 0; j < NUM_COLS; j++) {
         printf("%c", board_temp[i][j]);
      }
      putchar('\n');
   }
   printf("\n\n");
}

void initialize_attrs(void) {
   space = 8;
   player_score = ai_score = ai_win_count = player_win_count = 0;
   hard_mode = false;
}

void initialize_board(void) {
   int i, j;
   for (i = 0; i < NUM_ROWS-1; i++) {
      for (j = 0; j < NUM_COLS; j++) {
         if (j%2 == 0)
            board[i][j] = '_';
         else
            board[i][j] = '|';

      }
   }
   board[2][0] = ' ';
   board[2][2] = ' ';
   board[2][4] = ' ';
}

void display_board(void) {
   for (int i = 0; i < NUM_ROWS; i++) {
      for (int j = 0; j < NUM_COLS; j++) {
         printf("%c", board[i][j]);
      }
      putchar('\n');
   }
   putchar('\n');
}


int *allocate_position(char ch) {
   static int position[2];
   int row, col;

   switch(ch) {
      case '1': case '2': case '3':
         row = 0; col = 2*(ch-'1'); break;
      case '4': case '5': case '6':
         row = 1; col = 2*(ch-'4'); break;
      case '7': case '8': case '9':
         row = 2; col = 2*(ch-'7'); break;
   }

   position[0] = row;
   position[1] = col;

   return position;
}

bool is_valid_pos(int row, int col) {
   if (board[row][col] == '_' || board[row][col] == ' ')
      return true;
   return false;
}

bool cmp_cell_r(int r, int c1, int c2, int c3) {
   return (board[r][c1] == board[r][c2] &&
           board[r][c2] == board[r][c3]);
}

bool cmp_cell_c(int c, int r1, int r2, int r3) {
   return (board[r1][c] == board[r2][c] &&
           board[r2][c] == board[r3][c]);
}

bool cmp_cell_diag(void) {
   return (
           (board[0][0] == board[1][2] &&
            board[1][2] == board[2][4]) ||
           (board[0][4] == board[1][2] &&
            board[1][2] == board[2][0])
           );
}

int check_cell_move_dir(char start, char end, int inc) {
   if (start == '0') return 0;

   int filled_cell_counter = 0, total = 0, cell_sum = 0;
   int *pos, row, col;

   for (char init = start; init <= end; init += inc) {
      pos = allocate_position(init);
      row = pos[0]; col = pos[1];
      if (board[row][col] == 'O'){
         filled_cell_counter++;
         cell_sum += cint(init);
      }
      total += cint(init);
   }
   return filled_cell_counter == 2 ? total-cell_sum : 0;
}

int check_cell_move(char ch) {
   char start_hr = (ch <= '3' ? '1' : ch <= '6' ?   '4' :
                 '7'
                );
   char start_vr = any(ch, '1', '4', '7') ? '1' : any(ch, '2', '5',
                   '8') ? '2' : '3';
   char start_diag_tlc = any(ch, '1', '5', '9') ? '1' : '0';
   char start_diag_trc = any(ch, '3', '5', '7') ? '3' : '0';

   int move_right, move_bottom, move_diag_tlbr, move_diag_trbl;

   move_right = check_cell_move_dir(
                              start_hr,
                              start_hr+2, 1);
   move_bottom = check_cell_move_dir(
                              start_vr,
                              start_vr+6, 3);
   move_diag_tlbr = check_cell_move_dir(
                              start_diag_tlc,
                              start_diag_tlc+8, 4);
   move_diag_trbl = check_cell_move_dir(
                              start_diag_trc,
                              start_diag_trc+4, 2);

   return move_right ? move_right : move_bottom ?
          move_bottom : move_diag_tlbr ?
          move_diag_tlbr : move_diag_trbl ?
          move_diag_trbl : rand()%NUM_CELLS;
}


int generate_move(void) {
   return rand() % 9;
}

int generate_hard_move(void) {
   return check_cell_move(ch);
}

void ai_play(void) {
   int count = 0;
   char avilable_moves[] = {'1', '2', '3', '4', '5', '6', '7', '8', '9'};
   char move, curr_move;
   int *pos, row, col;
   do {
      curr_move = generate_hard_move()-1;
      while (curr_move <= 0) {
         curr_move = generate_hard_move()-1;
      }
      while (true) {
         pos = allocate_position(avilable_moves[curr_move]);
         row = pos[0]; col = pos[1];
         if (is_valid_pos(row, col))
            break;
         else {
            curr_move = generate_move();
            if (space <= 0) {
               printf("Do you want to continue (y/n)");
               scanf("%c", &choice); getchar();
               if (toupper(choice) == 'Y')
               play();
            else
               exit(EXIT_SUCCESS);
            }
         }
      }
      move = avilable_moves[curr_move];
      pos  = allocate_position(move);
      row = pos[0];
      col = pos[1];
   } while (!is_valid_pos(row, col));

   board[row][col] = 'X';
   space--;

   printf("AI play: \n");
   display_board();
}

void play(void) {
   initialize_attrs();
   initialize_board();
   int row, col, *pos;
   while (space >= 0) {
      printf("your play: ");
      scanf("%c", &ch); getchar();
      if (ch-'0' >= 1 && ch-'0' <= 9) {
         pos = allocate_position(ch);
         row = pos[0];
         col = pos[1];
         if (is_valid_pos(row, col)) {
            board[row][col] = 'O';
            display_board();
            space--;
            ai_play();
            check_win_status();
         } else {
            printf("Can not use non empty cell...\n");
         }
      } else {
         printf("%c is invalid entry..\n", ch);
         getchar();
      }
   }
}


void check_win_status(void) {
   int row, col;
   char winner = ' ';

   if (cmp_cell_r(0, 0, 2, 4))
      winner = board[0][0];
   else if (cmp_cell_r(1, 0, 2, 4))
      winner = board[1][0];
   else if (cmp_cell_r(2, 0, 2, 4))
      winner = board[2][2];
   else if (cmp_cell_c(0, 0, 1, 2))
      winner = board[0][0];
   else if (cmp_cell_c(2, 0, 1, 2))
      winner = board[0][2];
   else if (cmp_cell_c(4, 0, 1, 2))
      winner = board[0][4];
   else if (cmp_cell_diag())
      winner = board[1][2];


   if (winner == 'X') {
      printf("AI won!");
      printf("Do you want to continue (y/n)");
      scanf("%c", &choice); getchar();
      if (toupper(choice) == 'Y')
         play();
      else
         exit(EXIT_SUCCESS);
   } else if (winner == 'O') {
      printf("Player won\n");
      printf("Do you want to continue (y/n)");
      scanf("%c", &choice); getchar();
      if (toupper(choice) == 'Y')
         play();
      else
         exit(EXIT_SUCCESS);
   }
}

bool any(char ch, char ch1, char ch2, char ch3) {
   return (ch == ch1 || ch == ch2 || ch == ch3);
}

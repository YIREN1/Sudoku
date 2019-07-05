/////////////////////////////////////////////////////////////////////////////
// INTEGRITY INSTRUCTIONS

// Explicitly state the level of collaboration on this question
// Examples:
//   * I discussed ideas with classmate(s) [include name(s)]
//   * I worked together with classmate(s) in the lab [include name(s)]
//   * Classmate [include name] helped me debug my code
//   * I consulted website [include url]
//   * None
// A "None" indicates you completed this question entirely by yourself
// (or with assistance from course staff)
/////////////////////////////////////////////////////////////////////////////
// INTEGRITY STATEMENT:
// I received help from the following sources:

// none

// Name: REN YI
// login ID: y78ren
/////////////////////////////////////////////////////////////////////////////

#include "cs136.h"
#include "sudoku.h"
#include "array.h"

const int DIM = 9;

static const int EMPTY = 0;
static const char BLANK = '-';
static const int MIN = 1;
static const int MAX = 9;

static const int SUCCESS = 0;
static const int ERROR = -1;
static const int ERASE_EMPTY_CELL = -2;
static const int ERASE_FILLED_CELL = -3;
static const int ERROR_NEXT_CELL = -4;

struct sudoku {
  int puzzle[DIM * DIM];
  int sol[DIM * DIM];
};

// You need to implement the following four functions.

bool vio_box(const struct sudoku *s, int row, int col, int num){
  int x=col/3;
  int y=row/3;
  
  int start=27*y+3*x;
  int b[9]={};
  for(int i=0;i<9;++i){
    int k=9*(i/3)+i%3;
    b[i]=s->sol[start+k];
  }
  for(int i=0;i<DIM;++i){
    if(b[i]==num){
      return true;
    }
  }
  return false;
}


int fill_cell(struct sudoku *s, int row, int col, int num){
  
  // Implement this function
 
  for(int i=0;i<DIM;++i){
    if(s->sol[row * DIM + i]==num){
      return -1;
    }
    if(s->sol[i * DIM + col]==num){
      return -1;
    }
  }
  if(vio_box(s,row,col,num)){
      return ERROR;
  }
  s->sol[row * DIM + col]=num;
  return 0;
}

bool vaild(const struct sudoku *s, int row, int col, int num){
   for(int i=0;i<DIM;++i){
    if(s->sol[row * DIM + i]==num){
      return false;
    }
    if(s->sol[i * DIM + col]==num){
      return false;
    }
    
  }
  if(vio_box(s,row,col,num)){
      return false;
  }
  return true;
}




void choices_cell(const struct sudoku *s, int row, int col, 
                 int choices[], int *num_choices) {
  // Implement this function.
  int k=0;
  if(s->puzzle[9*row+col]==0){
    
  for(int i=MIN;i<=MAX;++i){
    
    if(vaild(s,row,col,i)){
      choices[k]=i;
      ++k;
    }
    *num_choices=k;
  }
  }
}







bool solved_puzzle(const struct sudoku *s) {
  // Implement this function
  for(int i=0;i<81;++i){
    if(s->sol[i]==0){
      return false;
    }
    //if(!vaild(s,i/9,i%9,s->sol[i])){
   //   return false;
   // } 
  }
  return true;
}

  
  
  
  
bool solve(struct sudoku *s) {
  // Implement this function
  if(solved_puzzle(s)){
    return true;
  }
  int i=0;
  while(1){
    if(s->sol[i]==0){
      break;
    }
    ++i;
  }
  int choices[9]={};
  int num_choices=0;
  choices_cell(s,i/9,i%9,choices,&num_choices);
  if(num_choices==0){
    return false;
  }else {
    while(num_choices>0){
       s->sol[i]=choices[num_choices-1];
       num_choices--;
       if(solve(s)){
         return true;
       }
    }
    s->sol[i]=0;
    return false;
  }
}
      
  
  
  
  
  
  
  


// There is no need to modify the rest of the functions.

struct sudoku *read_sudoku(void) {
  struct sudoku *s = malloc(sizeof(struct sudoku));
  char c = 0;
  for (int row = 0; row < DIM; ++row) {
    for (int col = 0; col < DIM; ++col) {
        scanf(" %c", &c);
      if (c == BLANK) {
        s->puzzle[row * DIM + col] = 0;
      } else {
        s->puzzle[row * DIM + col] = c - '0';
      }
    }
  }

  // copy puzzle to solution
  reset_sol(s);

  return s;
}

void sudoku_destroy(struct sudoku *s) {
  assert(s);
  free(s);
}

void print_sol(const struct sudoku *s) {
  assert(s);

  printf("\n");
  for (int row = 0; row < DIM; ++row) {
    for (int col = 0; col < DIM; ++col) {
      int num = s->sol[row * DIM + col];
      if (num == EMPTY) {
        printf("%c", BLANK);
      } else {
        printf("%d", num);
      }  
    }
    printf("\n");
  }
  printf("\n");
}

void reset_sol(struct sudoku *s) {
  assert(s);

  for (int row = 0; row < DIM; ++row) {
    for (int col = 0; col < DIM; ++col) {
      s->sol[row * DIM + col] = s->puzzle[row * DIM + col];
    }
  }  
}

// cell_empty(board, row, col) returns true
// if cell (row,col) is empty on board.
// requires: board is a valid sudoku puzzle.
static bool cell_empty(const int board[], int row, int col) {
  assert(board);
  assert(0 <= row && row <= DIM - 1);
  assert(0 <= col && col <= DIM - 1);

  return board[row * DIM + col] == EMPTY;
}

int erase_cell(struct sudoku *s, int row, int col) {
  assert(s);
  assert(0 <= row && row <= DIM - 1);
  assert(0 <= col && col <= DIM - 1);

  if (cell_empty(s->sol, row, col)) {
    return ERASE_EMPTY_CELL;
  }
  if (!cell_empty(s->puzzle, row, col)) {
    return ERASE_FILLED_CELL;
  }
  s->sol[row * DIM + col] = EMPTY;
  return SUCCESS;
}


int next_cell(const struct sudoku *s, int *row, int *col) {
  assert(s);
  assert(row);
  assert(col);

  int choices[DIM];
  int num_choices = 0;
  for (int i = 0; i < DIM; ++i) {
    for (int j = 0; j < DIM; ++j) {
      if (!cell_empty(s->sol, i, j)) continue;
      choices_cell(s, i, j, choices, &num_choices);
      if (num_choices == 1) {
        *row = i;
        *col = j;
        return SUCCESS;
      }
    }
  }
  return ERROR_NEXT_CELL;
}

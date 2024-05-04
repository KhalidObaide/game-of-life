#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include<unistd.h>

int WIDTH = 25, HEIGHT = 25;
int EPISODES = 1000;

void reset_board(int board[HEIGHT][WIDTH], int randomize);
void render_board(int board[HEIGHT][WIDTH]);
void step_board(int board[HEIGHT][WIDTH]);
void copy_board(int src[HEIGHT][WIDTH], int dest[HEIGHT][WIDTH]);
int get_alive_neighbors(int board[HEIGHT][WIDTH], int row, int cell);
const char* repr_tag(int tag);

int main() {
  srand(time(NULL));
  int board[HEIGHT][WIDTH];
  reset_board(board, 1);
  for (int i=0; i<EPISODES; i++) {
    step_board(board);
    render_board(board);
  }
  return 0;
}

void reset_board(int board[HEIGHT][WIDTH], int randomize) {
  for (int row=0; row<HEIGHT; row++) {
    for (int cell=0; cell<WIDTH; cell++) {
      board[row][cell] = randomize == 1 ? rand()%2 : 0;
    }
  }
}

const char* repr_tag(int tag) {
  return tag == 0 ? "⬜️" : "⬛️";
}

void render_board(int board[HEIGHT][WIDTH]) {
  system("clear");
  printf("\n");
  for (int row=0; row<HEIGHT; row++) {
    printf("    ");
    for (int cell=0; cell<WIDTH; cell++) {
      printf("%s", repr_tag(board[row][cell]));
    }
    printf("\n");
  }
  printf("\n");
  usleep(100*1000); //100ms
}

void copy_board(int src[HEIGHT][WIDTH], int dest[HEIGHT][WIDTH]) {
  for (int row=0; row<HEIGHT; row++) {
    for (int cell=0; cell<WIDTH; cell++) {
      dest[row][cell] = src[row][cell];
    }
  }
}

void step_board(int board[HEIGHT][WIDTH]) {
  int new_board[HEIGHT][WIDTH];
  copy_board(board, new_board);

  for (int row=0; row<HEIGHT; row++) {
    for (int cell=0; cell<WIDTH; cell++) {
      int current_state = board[row][cell];
      int alive_neighbors = get_alive_neighbors(board, row, cell);
      int new_state = current_state;
      // underpopulation
      if (current_state == 1 && alive_neighbors < 2) new_state = 0;
      // normal ( continue to next gen )
      else if (current_state == 1 && alive_neighbors <= 3) new_state = 1;
      // overpopulation
      else if (current_state == 1 && alive_neighbors > 3) new_state = 0;
      // reproduction
      else if (current_state == 0 && alive_neighbors == 3) new_state = 1;
      new_board[row][cell] = new_state;
    }
  }

  copy_board(new_board, board);
}

int get_alive_neighbors(int board[HEIGHT][WIDTH], int row, int cell) {
  int result = 0;
  // top
  if (board[(row-1)%HEIGHT][cell]) result++;
  // bottom
  if (board[(row+1)%HEIGHT][cell]) result++;
  // right
  if (board[row][(cell+1)%WIDTH]) result++;
  // left
  if (board[row][(cell-1)%WIDTH]) result++;
  // top-right
  if (board[(row-1)%HEIGHT][(cell+1)%WIDTH]) result++;
  // top-left
  if (board[(row-1)%HEIGHT][(cell-1)%WIDTH]) result++;
  // bottom-right
  if (board[(row+1)%HEIGHT][(cell+1)%WIDTH]) result++;
  // bottom-left
  if (board[(row+1)%HEIGHT][(cell-1)%WIDTH]) result++;
  return result;
}


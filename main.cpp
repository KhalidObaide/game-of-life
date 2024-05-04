#include <iostream>
#include <SDL2/SDL.h>
#include <vector>

typedef struct { 
  int x;
  int y;
} Coordinate;


const unsigned SCALE = 1;
const Coordinate SIZE = {720, 720};
const Coordinate BOARD_SIZE = {720/SCALE, 720/SCALE};
const Coordinate CELL_SIZE = {1*SCALE, 1*SCALE};

int main() {
  srand(time(NULL));

  // init SDL ( window & renderer )
  SDL_Init(SDL_INIT_VIDEO);
  SDL_Window* window = SDL_CreateWindow(
    "Runner", 
    SDL_WINDOWPOS_CENTERED, 
    SDL_WINDOWPOS_CENTERED, 
    SIZE.x, 
    SIZE.y, 
    SDL_WINDOW_SHOWN
  );
  SDL_Renderer* renderer = SDL_CreateRenderer(
    window, -1, SDL_RENDERER_ACCELERATED
  );

  int world[BOARD_SIZE.y] [BOARD_SIZE.y];
  for (int row=0; row<BOARD_SIZE.y; row++) {
    for (int cell=0; cell<BOARD_SIZE.x; cell++) {
      world[row][cell] = rand() % 2;
    }
  }

  // main loop
  bool running = true;
  while (running) {
    // events
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
      if (event.type == SDL_QUIT) {
        running = false;
      }
    }

    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderClear(renderer);

    int new_world[BOARD_SIZE.y][BOARD_SIZE.x];
    for (int row=0; row<BOARD_SIZE.y; row++) {
      for (int cell=0; cell<BOARD_SIZE.x; cell++) {
        int alive_sides = world[row][cell] ? -1 : 0;
        for (int r=row-1; r<=row+1; r++) {
          for (int c=cell-1; c<=cell+1; c++) {
            alive_sides += world[r%BOARD_SIZE.y][c%BOARD_SIZE.x];
          }
        }
        int state = world[row][cell];
        if (state && alive_sides < 2) state = 0;
        else if (!state && alive_sides == 3) state = 1;
        else if (state && alive_sides > 3) state = 0;
        new_world[row][cell] = state;
      }
    }

    // deep clone
    for (int row=0; row<BOARD_SIZE.y; row++) {
      for (int cell=0; cell<BOARD_SIZE.x; cell++) {
        world[row][cell] = new_world[row][cell];
      }
    }

    for (int row=0; row<BOARD_SIZE.y; row++) {
      for (int cell=0; cell<BOARD_SIZE.x; cell++) {
        int color = world[row][cell] ? 255 : 0;
        SDL_Rect repr = {
          cell*CELL_SIZE.x, row*CELL_SIZE.y, CELL_SIZE.x, CELL_SIZE.y
        };
        SDL_SetRenderDrawColor(renderer, color, color, color, 255);
        SDL_RenderFillRect(renderer, &repr);
      }
    }

    SDL_RenderPresent(renderer);
    // SDL_Delay(1000/60);
  }

  // Clean up
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  SDL_Quit();
  return 0;
}


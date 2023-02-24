#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include "SDL2/SDL.h"

// Constants
const int WINDOW_WIDTH = 640;
const int WINDOW_HEIGHT = 640;
const int CELL_SIZE = 200;
const int BOARD_SIZE = 3;
const int BOARD_MARGIN = 10;

// Globals
SDL_Window *window = NULL;
SDL_Renderer *renderer = NULL;
bool quit = false;

//Structs
typedef struct board_position {
    int x;
    int y;
} BoardPosition;

typedef struct result {
    bool game_over;
    BoardPosition positions[3];
} Result;

int board[3][3] = {{0, 0, 0},
                   {0, 0, 0},
                   {0, 0, 0}};
int currentPlayer = 1;

// Function prototypes
void initSDL();

void closeSDL();

void drawBoard(Result *result);

void drawX(int x, int y);

void drawO(int x, int y);

bool isMoveValid(int x, int y);

Result isGameOver();

void restartGame();

void handleEvent(SDL_Event *event, Result *result);

void FillWinnerPositions(BoardPosition positions[3], Result *result);

// Entry point
int main(int argc, char *argv[]) {
    initSDL();

    SDL_Event event;
    while (!quit) {

        Result result = isGameOver();

        while (SDL_PollEvent(&event)) {
            handleEvent(&event, &result);
            if (event.type == SDL_QUIT) {
                quit = true;
            }
        }

        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderClear(renderer);

        drawBoard(&result);

        SDL_RenderPresent(renderer);

    }

    closeSDL();
    return 0;
}

// Initialize SDL
void initSDL() {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
        exit(EXIT_FAILURE);
    }

    window = SDL_CreateWindow("Tic Tac Toe",
                              SDL_WINDOWPOS_UNDEFINED,
                              SDL_WINDOWPOS_UNDEFINED,
                              WINDOW_WIDTH,
                              WINDOW_HEIGHT,
                              SDL_WINDOW_SHOWN);
    if (window == NULL) {
        printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
        exit(EXIT_FAILURE);
    }

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (renderer == NULL) {
        printf("Renderer could not be created! SDL_Error: %s\n", SDL_GetError());
        exit(EXIT_FAILURE);
    }
}

// Close SDL
void closeSDL() {
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

// Draw the game board
void drawBoard(Result *result) {
    SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);

    // Draw vertical lines
    for (int i = 1; i < BOARD_SIZE; i++) {
        int x = i * CELL_SIZE + BOARD_MARGIN;
        SDL_RenderDrawLine(renderer, x, BOARD_MARGIN, x, WINDOW_HEIGHT - BOARD_MARGIN);
    }

    // Draw horizontal lines
    for (int i = 1; i < BOARD_SIZE; i++) {
        int y = i * CELL_SIZE + BOARD_MARGIN;
        SDL_RenderDrawLine(renderer, BOARD_MARGIN, y, WINDOW_WIDTH - BOARD_MARGIN, y);
    }

    // Draw X's and O's
    for (int i = 0; i < BOARD_SIZE; i++) {
        for (int j = 0; j < BOARD_SIZE; j++) {
            if (board[i][j] == 1) {
                drawX(i, j);
            } else if (board[i][j] == 2) {
                drawO(i, j);
            }
        }
    }

    if (result->game_over) {

        for (int i = 0; i < 2; ++i) {
            int startX = result->positions[i].x * CELL_SIZE + BOARD_MARGIN * BOARD_MARGIN;
            int startY = result->positions[i].y * CELL_SIZE + BOARD_MARGIN * BOARD_MARGIN;
            int endX = (result->positions[i + 1].x) * CELL_SIZE + BOARD_MARGIN * BOARD_MARGIN;
            int endY = (result->positions[i + 1].y) * CELL_SIZE + BOARD_MARGIN * BOARD_MARGIN;

            SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);

            SDL_RenderDrawLine(renderer, startX, startY, endX, endY);

        }
    }
}

// Draw an X at the given board_position
void drawX(int x, int y) {
    int startX = x * CELL_SIZE + BOARD_MARGIN + CELL_SIZE / 4;
    int startY = y * CELL_SIZE + BOARD_MARGIN + CELL_SIZE / 4;
    int endX = (x + 1) * CELL_SIZE - BOARD_MARGIN - CELL_SIZE / 4;
    int endY = (y + 1) * CELL_SIZE - BOARD_MARGIN - CELL_SIZE / 4;

    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
    SDL_RenderDrawLine(renderer, startX, startY, endX, endY);
    SDL_RenderDrawLine(renderer, startX, endY, endX, startY);
}


// Draw an O at the given board_position
void drawO(int x, int y) {
    int centerX = x * CELL_SIZE + BOARD_MARGIN + CELL_SIZE / 2;
    int centerY = y * CELL_SIZE + BOARD_MARGIN + CELL_SIZE / 2;
    int radius = CELL_SIZE / 4;

    SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
    for (int i = 0; i < radius * 2; i++) {
        for (int j = 0; j < radius * 2; j++) {
            int dx = i - radius;
            int dy = j - radius;
            if ((dx * dx + dy * dy) <= (radius * radius)) {
                SDL_RenderDrawPoint(renderer, centerX + dx, centerY + dy);
            }
        }
    }
}

// Check if a move is valid
bool isMoveValid(int x, int y) {
    if (x < 0 || x >= BOARD_SIZE || y < 0 || y >= BOARD_SIZE) {
        return false;
    }

    if (board[x][y] != 0) {
        return false;
    }

    return true;
}

void FillWinnerPositions(BoardPosition positions[3], Result *result) {
    for (int i = 0; i < 3; ++i) {
        result->positions[i] = positions[i];
    }
}


// Check if the game is over
Result isGameOver() {
    // Check rows
    Result result;
    result.game_over = false;

    for (int i = 0; i < BOARD_SIZE; i++) {
        if (board[i][0] != 0 && board[i][0] == board[i][1] && board[i][1] == board[i][2]) {
            result.game_over = true;

            BoardPosition winner_positions[3] = {
                    {i, 0},
                    {i, 1},
                    {i, 2}
            };

            FillWinnerPositions(winner_positions, &result);

            return result;
        }
    }

    // Check columns
    for (int i = 0; i < BOARD_SIZE; i++) {
        if (board[0][i] != 0 && board[0][i] == board[1][i] && board[1][i] == board[2][i]) {
            result.game_over = true;

            BoardPosition winner_positions[3] = {
                    {0, i},
                    {1, i},
                    {2, i}
            };

            FillWinnerPositions(winner_positions, &result);

            return result;
        }
    }

    // Check diagonals
    if (board[0][0] != 0 && board[0][0] == board[1][1] && board[1][1] == board[2][2]) {
        result.game_over = true;

        BoardPosition winner_positions[3] = {
                {0, 0},
                {1, 1},
                {2, 2}
        };

        FillWinnerPositions(winner_positions, &result);

        return result;
    }

    if (board[0][2] != 0 && board[0][2] == board[1][1] && board[1][1] == board[2][0]) {
        result.game_over = true;

        BoardPosition winner_positions[3] = {
                {0, 2},
                {1, 1},
                {2, 0}
        };

        FillWinnerPositions(winner_positions, &result);

        return result;
    }

    // Check if board is full
    for (int i = 0; i < BOARD_SIZE; i++) {
        for (int j = 0; j < BOARD_SIZE; j++) {
            if (board[i][j] == 0) {
                return result;
            }
        }
    }

    return result;
}

// Restart the game
void restartGame() {
    for (int i = 0; i < BOARD_SIZE; i++) {
        for (int j = 0; j < BOARD_SIZE; j++) {
            board[i][j] = 0;
        }
    }

    currentPlayer = 1;
}


// Handle events
void handleEvent(SDL_Event *event, Result *result) {
    switch (event->type) {
        case SDL_MOUSEBUTTONDOWN:

            if (result->game_over) {
                quit = true;
                break;
            } else if (event->button.button == SDL_BUTTON_LEFT) {
                int x = event->button.x;
                int y = event->button.y;

                // Convert mouse board_position to board coordinates
                int boardX = (x - BOARD_MARGIN) / CELL_SIZE;
                int boardY = (y - BOARD_MARGIN) / CELL_SIZE;

                if (isMoveValid(boardX, boardY)) {
                    board[boardX][boardY] = currentPlayer;
                    currentPlayer = (currentPlayer == 1) ? 2 : 1;
                }
            }
            break;
        case SDL_KEYDOWN:
            if (event->key.keysym.sym == SDLK_r) {
                restartGame();
            }
            break;
    }
}




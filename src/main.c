#include "game.h"

#include <stdio.h>
#ifdef _WIN32
#include <windows.h>
#endif

#define DEFAULT_ROWS 9
#define DEFAULT_COLS 9
#define DEFAULT_MINES 10

static int read_settings(int *rows, int *cols, int *mines);

int main(void)
{
    Game game;
    int rows = DEFAULT_ROWS;
    int cols = DEFAULT_COLS;
    int mines = DEFAULT_MINES;
    char line[8];

#ifdef _WIN32
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
#endif

    printf("Welcome to C Minesweeper.\n");
    printf("Default board: %d x %d, mines: %d.\n", DEFAULT_ROWS, DEFAULT_COLS, DEFAULT_MINES);

    if (!read_settings(&rows, &cols, &mines)) {
        printf("Invalid settings. Using default values.\n");
        rows = DEFAULT_ROWS;
        cols = DEFAULT_COLS;
        mines = DEFAULT_MINES;
    }

    if (!game_init(&game, rows, cols, mines)) {
        printf("Failed to initialize the game. Please check board size and mine count.\n");
        return 1;
    }

    game_run(&game);
    game_destroy(&game);

    printf("Press Enter to close...");
    fgets(line, sizeof(line), stdin);
    return 0;
}

static int read_settings(int *rows, int *cols, int *mines)
{
    char choice;
    char line[128];

    printf("Customize board? (y/N): ");
    if (fgets(line, sizeof(line), stdin) == NULL) {
        return 1;
    }

    if (sscanf(line, " %c", &choice) != 1 || (choice != 'y' && choice != 'Y')) {
        return 1;
    }

    printf("Enter rows columns mines, for example 9 9 10: ");
    if (fgets(line, sizeof(line), stdin) == NULL) {
        return 0;
    }

    if (sscanf(line, "%d %d %d", rows, cols, mines) != 3) {
        return 0;
    }

    return *rows > 0 && *cols > 0 && *mines > 0 && *mines < (*rows * *cols);
}

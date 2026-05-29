#include "game.h"

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

static void print_commands(void);
static int read_command(char *command, int *row, int *col);

int game_init(Game *game, int rows, int cols, int mines)
{
    if (game == NULL) {
        return 0;
    }

    if (!board_create(&game->board, rows, cols, mines)) {
        return 0;
    }

    game->status = GAME_RUNNING;
    game->firstMove = 1;
    srand((unsigned int)time(NULL));
    return 1;
}

void game_destroy(Game *game)
{
    if (game == NULL) {
        return;
    }

    board_destroy(&game->board);
    game->status = GAME_QUIT;
    game->firstMove = 0;
}

void game_run(Game *game)
{
    if (game == NULL) {
        return;
    }

    print_commands();

    while (game->status == GAME_RUNNING) {
        char command;
        int row;
        int col;
        int result;

        board_print(&game->board, 0);
        printf("Enter command: ");

        if (!read_command(&command, &row, &col)) {
            printf("Invalid input. Use r row col, f row col, or q.\n");
            continue;
        }

        if (command == 'q' || command == 'Q') {
            game->status = GAME_QUIT;
            break;
        }

        row--;
        col--;
        if (!board_is_inside(&game->board, row, col)) {
            printf("Position is outside the board.\n");
            continue;
        }

        if (command == 'f' || command == 'F') {
            board_toggle_flag(&game->board, row, col);
            continue;
        }

        if (command != 'r' && command != 'R') {
            printf("Unknown command. Use r, f, or q.\n");
            continue;
        }

        if (game->firstMove) {
            board_place_mines(&game->board, row, col);
            board_calculate_adjacent_counts(&game->board);
            game->firstMove = 0;
        }

        result = board_reveal(&game->board, row, col);
        if (result == -1) {
            game->status = GAME_LOST;
        } else if (board_all_safe_cells_revealed(&game->board)) {
            game->status = GAME_WON;
        }
    }

    board_print(&game->board, 1);
    if (game->status == GAME_WON) {
        printf("You win! All safe cells were revealed.\n");
    } else if (game->status == GAME_LOST) {
        printf("Boom! You stepped on a mine.\n");
    } else {
        printf("Game exited.\n");
    }
}

static void print_commands(void)
{
    printf("=== C Minesweeper ===\n");
    printf("Commands:\n");
    printf("  r row col  reveal a cell, for example: r 2 3\n");
    printf("  f row col  toggle a flag, for example: f 4 1\n");
    printf("  q          quit the game\n");
}

static int read_command(char *command, int *row, int *col)
{
    char line[128];
    int count;

    if (fgets(line, sizeof(line), stdin) == NULL) {
        return 0;
    }

    count = sscanf(line, " %c %d %d", command, row, col);
    if (*command == 'q' || *command == 'Q') {
        return count >= 1;
    }

    return count == 3;
}

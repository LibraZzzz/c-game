#ifndef BOARD_H
#define BOARD_H

#include <stddef.h>

typedef enum {
    CELL_HIDDEN = 0,
    CELL_REVEALED,
    CELL_FLAGGED
} CellState;

typedef struct {
    int hasMine;
    int adjacentMines;
    CellState state;
} Cell;

typedef struct {
    int rows;
    int cols;
    int mineCount;
    int revealedCount;
    Cell **cells;
} Board;

int board_create(Board *board, int rows, int cols, int mineCount);
void board_destroy(Board *board);
int board_is_inside(const Board *board, int row, int col);
void board_place_mines(Board *board, int safeRow, int safeCol);
void board_calculate_adjacent_counts(Board *board);
int board_reveal(Board *board, int row, int col);
int board_toggle_flag(Board *board, int row, int col);
int board_all_safe_cells_revealed(const Board *board);
void board_print(const Board *board, int revealAll);

#endif

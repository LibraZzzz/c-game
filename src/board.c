#include "board.h"

#include <stdio.h>
#include <stdlib.h>

static void reveal_empty_area(Board *board, int row, int col);

int board_create(Board *board, int rows, int cols, int mineCount)
{
    int i;

    if (board == NULL || rows <= 0 || cols <= 0 || mineCount <= 0 || mineCount >= rows * cols) {
        return 0;
    }

    board->rows = rows;
    board->cols = cols;
    board->mineCount = mineCount;
    board->revealedCount = 0;
    board->cells = (Cell **)malloc((size_t)rows * sizeof(Cell *));

    if (board->cells == NULL) {
        return 0;
    }

    for (i = 0; i < rows; i++) {
        int j;

        board->cells[i] = (Cell *)malloc((size_t)cols * sizeof(Cell));
        if (board->cells[i] == NULL) {
            int allocatedRow;

            for (allocatedRow = 0; allocatedRow < i; allocatedRow++) {
                free(board->cells[allocatedRow]);
            }
            free(board->cells);
            board->cells = NULL;
            return 0;
        }

        for (j = 0; j < cols; j++) {
            board->cells[i][j].hasMine = 0;
            board->cells[i][j].adjacentMines = 0;
            board->cells[i][j].state = CELL_HIDDEN;
        }
    }

    return 1;
}

void board_destroy(Board *board)
{
    int i;

    if (board == NULL || board->cells == NULL) {
        return;
    }

    for (i = 0; i < board->rows; i++) {
        free(board->cells[i]);
    }
    free(board->cells);

    board->cells = NULL;
    board->rows = 0;
    board->cols = 0;
    board->mineCount = 0;
    board->revealedCount = 0;
}

int board_is_inside(const Board *board, int row, int col)
{
    return board != NULL && row >= 0 && row < board->rows && col >= 0 && col < board->cols;
}

void board_place_mines(Board *board, int safeRow, int safeCol)
{
    int placed = 0;

    if (board == NULL) {
        return;
    }

    while (placed < board->mineCount) {
        int row = rand() % board->rows;
        int col = rand() % board->cols;
        Cell *cell = &board->cells[row][col];

        if (cell->hasMine || (row == safeRow && col == safeCol)) {
            continue;
        }

        cell->hasMine = 1;
        placed++;
    }
}

void board_calculate_adjacent_counts(Board *board)
{
    int row;

    if (board == NULL) {
        return;
    }

    for (row = 0; row < board->rows; row++) {
        int col;

        for (col = 0; col < board->cols; col++) {
            int dr;
            int count = 0;

            if (board->cells[row][col].hasMine) {
                continue;
            }

            for (dr = -1; dr <= 1; dr++) {
                int dc;

                for (dc = -1; dc <= 1; dc++) {
                    int nr = row + dr;
                    int nc = col + dc;

                    if ((dr != 0 || dc != 0) && board_is_inside(board, nr, nc) && board->cells[nr][nc].hasMine) {
                        count++;
                    }
                }
            }

            board->cells[row][col].adjacentMines = count;
        }
    }
}

int board_reveal(Board *board, int row, int col)
{
    Cell *cell;

    if (!board_is_inside(board, row, col)) {
        return 0;
    }

    cell = &board->cells[row][col];
    if (cell->state == CELL_REVEALED || cell->state == CELL_FLAGGED) {
        return 1;
    }

    if (cell->hasMine) {
        cell->state = CELL_REVEALED;
        return -1;
    }

    reveal_empty_area(board, row, col);
    return 1;
}

int board_toggle_flag(Board *board, int row, int col)
{
    Cell *cell;

    if (!board_is_inside(board, row, col)) {
        return 0;
    }

    cell = &board->cells[row][col];
    if (cell->state == CELL_REVEALED) {
        return 1;
    }

    cell->state = (cell->state == CELL_FLAGGED) ? CELL_HIDDEN : CELL_FLAGGED;
    return 1;
}

int board_all_safe_cells_revealed(const Board *board)
{
    int safeCellCount;

    if (board == NULL) {
        return 0;
    }

    safeCellCount = board->rows * board->cols - board->mineCount;
    return board->revealedCount == safeCellCount;
}

void board_print(const Board *board, int revealAll)
{
    int row;

    if (board == NULL) {
        return;
    }

    printf("\n    ");
    for (row = 0; row < board->cols; row++) {
        printf("%2d ", row + 1);
    }
    printf("\n");

    printf("   +");
    for (row = 0; row < board->cols; row++) {
        printf("---");
    }
    printf("+\n");

    for (row = 0; row < board->rows; row++) {
        int col;

        printf("%2d |", row + 1);
        for (col = 0; col < board->cols; col++) {
            const Cell *cell = &board->cells[row][col];
            char symbol = '#';

            if (cell->state == CELL_FLAGGED && !revealAll) {
                symbol = 'F';
            } else if ((cell->state == CELL_REVEALED || revealAll) && cell->hasMine) {
                symbol = '*';
            } else if (cell->state == CELL_REVEALED || revealAll) {
                symbol = (cell->adjacentMines == 0) ? ' ' : (char)('0' + cell->adjacentMines);
            }

            printf(" %c ", symbol);
        }
        printf("|\n");
    }

    printf("   +");
    for (row = 0; row < board->cols; row++) {
        printf("---");
    }
    printf("+\n\n");
}

static void reveal_empty_area(Board *board, int row, int col)
{
    int dr;
    Cell *cell;

    if (!board_is_inside(board, row, col)) {
        return;
    }

    cell = &board->cells[row][col];
    if (cell->state == CELL_REVEALED || cell->state == CELL_FLAGGED || cell->hasMine) {
        return;
    }

    cell->state = CELL_REVEALED;
    board->revealedCount++;

    if (cell->adjacentMines != 0) {
        return;
    }

    for (dr = -1; dr <= 1; dr++) {
        int dc;

        for (dc = -1; dc <= 1; dc++) {
            if (dr != 0 || dc != 0) {
                reveal_empty_area(board, row + dr, col + dc);
            }
        }
    }
}

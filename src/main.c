#include "game.h"

#include <stdio.h>

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

    printf("歡迎來到 C 語言資料結構專案：踩地雷\n");
    printf("預設棋盤為 %d x %d，地雷數 %d。\n", DEFAULT_ROWS, DEFAULT_COLS, DEFAULT_MINES);

    if (!read_settings(&rows, &cols, &mines)) {
        printf("設定不合法，改用預設值。\n");
        rows = DEFAULT_ROWS;
        cols = DEFAULT_COLS;
        mines = DEFAULT_MINES;
    }

    if (!game_init(&game, rows, cols, mines)) {
        printf("遊戲初始化失敗，請檢查棋盤大小與地雷數。\n");
        return 1;
    }

    game_run(&game);
    game_destroy(&game);
    return 0;
}

static int read_settings(int *rows, int *cols, int *mines)
{
    char choice;
    char line[128];

    printf("是否自訂棋盤？(y/N): ");
    if (fgets(line, sizeof(line), stdin) == NULL) {
        return 1;
    }

    if (sscanf(line, " %c", &choice) != 1 || (choice != 'y' && choice != 'Y')) {
        return 1;
    }

    printf("請輸入列數 行數 地雷數，例如 9 9 10: ");
    if (fgets(line, sizeof(line), stdin) == NULL) {
        return 0;
    }

    if (sscanf(line, "%d %d %d", rows, cols, mines) != 3) {
        return 0;
    }

    return *rows > 0 && *cols > 0 && *mines > 0 && *mines < (*rows * *cols);
}

# C 語言資料結構專案：踩地雷 Minesweeper

這是一個以 C 語言實作的命令列版踩地雷遊戲，專案重點放在資料結構設計、二維動態記憶體配置、模組化分層與完整釋放記憶體。棋盤使用「列指標陣列 + 每一列獨立配置」的方式建立，符合 C 語言中常見的動態二維陣列設計。

## 功能介紹

- 可使用預設 9 x 9、10 顆地雷棋盤，也可自訂列數、行數與地雷數。
- 支援翻開格子、插旗與取消旗標。
- 第一個翻開的格子保證不是地雷。
- 自動計算周圍 8 格地雷數。
- 翻開空白格時會遞迴展開相鄰安全區域。
- 遊戲會判斷勝利、失敗與離開狀態。
- 結束時顯示完整棋盤，方便檢查地雷配置。

## 使用技術

- C99
- MinGW GCC / GCC
- Makefile
- 模組化標頭檔與原始碼分離
- 動態記憶體配置：`malloc` / `free`
- 指標陣列形式二維棋盤：`Cell **cells`

## 系統架構

```text
.
├── include/
│   ├── board.h      # 棋盤資料結構與操作介面
│   └── game.h       # 遊戲狀態與流程介面
├── src/
│   ├── board.c      # 棋盤配置、釋放、佈雷、翻格、列印
│   ├── game.c       # 遊戲主流程、指令讀取、勝敗判定
│   └── main.c       # 程式入口與初始設定
├── Makefile         # 編譯與執行指令
└── README.md
```

### 資料結構設計

`Cell` 代表棋盤中的單一格子：

- `hasMine`：是否有地雷
- `adjacentMines`：周圍地雷數
- `state`：目前狀態，包含隱藏、已翻開、已插旗

`Board` 管理整張棋盤：

- `rows` / `cols`：棋盤大小
- `mineCount`：地雷數
- `revealedCount`：已翻開安全格數
- `Cell **cells`：二維棋盤，採列指標配置

### 記憶體配置與釋放

本專案明確使用列指標建立棋盤：

```c
board->cells = (Cell **)malloc((size_t)rows * sizeof(Cell *));
for (i = 0; i < rows; i++) {
    board->cells[i] = (Cell *)malloc((size_t)cols * sizeof(Cell));
}
```

釋放時逐列釋放，再釋放外層指標：

```c
for (i = 0; i < board->rows; i++) {
    free(board->cells[i]);
}
free(board->cells);
```

因此配置數量與釋放數量對應如下：

- 外層列指標陣列：1 次 `malloc`，1 次 `free`
- 每一列資料：`rows` 次 `malloc`，`rows` 次 `free`

若任一列配置失敗，程式會立即釋放前面已配置成功的列，再釋放外層指標，避免記憶體洩漏。

## 執行方式

### 使用 Makefile

```bash
make
make run
```

### Windows PowerShell 直接編譯

```powershell
gcc -std=c99 -Wall -Wextra -pedantic -Iinclude src/main.c src/board.c src/game.c -o minesweeper.exe
.\minesweeper.exe
```

### 遊戲指令

```text
r 行 列  翻開指定格，例如 r 2 3
f 行 列  標記或取消旗標，例如 f 4 1
q       離開遊戲
```

## Prompt 使用紀錄與除錯

### Prompt 目標整理

本專案依照需求將任務拆解為以下工程目標：

1. 建立可執行的 C 語言踩地雷遊戲。
2. 使用合理的資料結構表示棋盤、格子與遊戲狀態。
3. 將系統拆分為棋盤層、遊戲流程層與程式入口層。
4. 二維棋盤必須使用列指標與每一列獨立記憶體配置。
5. 所有 `malloc` 都必須有對應的 `free`，並處理配置失敗時的清理。
6. README 需包含專題名稱、功能介紹、使用技術、架構、執行方式、Prompt 紀錄與除錯說明。

### 除錯與修正紀錄

- 初始化階段加入參數驗證，避免地雷數大於或等於總格數造成無法完成佈雷。
- 佈雷時排除第一次翻開的格子，提升遊戲公平性。
- 翻開格子時先檢查座標是否在棋盤內，避免非法索引造成未定義行為。
- 插旗格不可被直接翻開，降低操作誤觸風險。
- 空白區域展開使用遞迴函式，並以 `CELL_REVEALED` 作為停止條件，避免重複翻開造成無限遞迴。
- `board_create` 在任一列配置失敗時會釋放已配置列與外層指標，確保錯誤路徑也不會洩漏。
- `game_destroy` 統一呼叫 `board_destroy`，確保遊戲結束、勝利、失敗或中途離開都會釋放棋盤記憶體。

## 自查清單

- [x] 專案可使用 GCC 編譯。
- [x] 棋盤使用 `Cell **` 與逐列 `malloc`。
- [x] 每一個成功配置的記憶體區塊都有對應 `free`。
- [x] 配置失敗路徑會清理已配置記憶體。
- [x] 系統分為 `main`、`game`、`board` 三層。
- [x] 遊戲具備翻格、插旗、勝敗判定與離開功能。
- [x] README 包含專題名稱、功能介紹、使用技術、系統架構、執行方式、Prompt 使用紀錄與除錯。

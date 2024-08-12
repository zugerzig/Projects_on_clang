#include <locale.h>
#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>

#define HEIGHT 25
#define WIDTH 80
#define VOID_SYMBOL " "
#define CELL_SYMBOL "█"

void makeStep(int arr[][WIDTH]);
int getNextCellState(int arr[][WIDTH], int n, int m);
void fullfillWindow(int arr[][WIDTH], char *argv[]);
void outputWindow(int arr[][WIDTH]);
void outputSpeed(int num);
int checkBordersExceptofCorners(int arr[][WIDTH], int n, int m);
int checkFullBorders(int arr[][WIDTH], int n, int m);
int check_ending(int arr[][WIDTH]);
void charHandler(char ch, int arr[][WIDTH], int *slowGame, int *flag, int *showParamsViewFlag,
                 int *pauseFlag);
void printHelp();
void printEndGame();
void printGen(int gen);
void stdInput(int arr[][WIDTH]);
void loopOutput(int arr[][WIDTH], int pauseFlag, int showParamsViewFlag, int slowGame, int genCounter);

int main(int argc, char *argv[]) {
    setlocale(LC_ALL, "");
    int flag = 1;
    int window_matrix[HEIGHT][WIDTH];
    int slowGame = 100;
    int pauseFlag = 0;
    int showParamsViewFlag = 0;
    int gameWasRunned = 1;
    int genCounter = 1;
    if (argc != 26 && argc != 1) {
        flag = 0;
        gameWasRunned = 0;
    }
    if (argc == 1) {
        stdInput(window_matrix);
        if (freopen("/dev/tty", "r", stdin)) {
        }
    } else if (argc == 2) {
        if (argv[1][0] == 'h' && argv[1][1] == 'e' && argv[1][2] == 'l' && argv[1][3] == 'p') {
            printHelp();
        }
    } else if (argc == 26) {
        fullfillWindow(window_matrix, argv);
    }
    if (flag) {
        initscr();
        curs_set(0);
        noecho();
    }
    while (flag) {
        nodelay(stdscr, true);
        loopOutput(window_matrix, pauseFlag, showParamsViewFlag, slowGame, genCounter);
        int ch = getch();
        charHandler(ch, window_matrix, &slowGame, &flag, &showParamsViewFlag, &pauseFlag);
        napms(slowGame);
        genCounter++;
    }
    if (gameWasRunned) {
        printGen(genCounter);
        printEndGame();
        getchar();
        endwin();
    }
    return 0;
}

void loopOutput(int arr[][WIDTH], int pauseFlag, int showParamsViewFlag, int slowGame, int genCounter) {
    if (!pauseFlag) {
        outputWindow(arr);
        if (showParamsViewFlag) {
            outputSpeed(slowGame);
            printGen(genCounter);
        }
        refresh();
        makeStep(arr);
    }
}

void printGen(int gen) {
    mvprintw(0, 60, "Поколение %d", gen);
    refresh();
}

void stdInput(int arr[][WIDTH]) {
    for (int i = 0; i < HEIGHT; i++) {
        for (int j = 0; j < WIDTH; j++) {
            char ch;
            scanf("%c", &ch);
            if (ch == '\n') {
                j--;
                continue;
            }
            if (ch == '1') {
                arr[i][j] = 1;
            } else {
                arr[i][j] = 0;
            }
        }
    }
}

void printEndGame() {
    mvprintw(0, 0, "Игра завершилась%s%s%s", VOID_SYMBOL, VOID_SYMBOL, VOID_SYMBOL);
    refresh();
}

void printHelp() {
    printf("________________________________The Game of Life________________________________\n");
    printf("Правила игры:\n");
    printf("    Если клетка живая и рядом есть 2 или 3 живых клетки, \n    то клетка продолжает жить;\n");
    printf("    В ином случае живая клетка умирает;\n");
    printf("    Если клетка мертвая и рядом находится 3 живых клетки, то клетка оживает;\n");
    printf("    В других случаях клетка остается мертвой.\n");
    printf("Как Запустить?\n");
    printf("    Для запуска игры требуется передать начальное состояние игры\n");
    printf("    через stdin или указав данные файла в параметрах запуска прогаммы,\n");
    printf("    сделать это можно так:\n");
    printf("    cat init/oscillators | ./a.out\n");
    printf("    ./a.out $(cat inits/oscillators.txt)\n");
    printf("Управление:\n");
    printf("    Ускорение игры - w\n");
    printf("    Замедление - s\n");
    printf("    Завершение игры - q\n");
    printf("    Пауза/продолжение игры - p\n");
    printf("    Смена вида - v\n");
}

void charHandler(char ch, int arr[][WIDTH], int *slowGame, int *flag, int *showParamsViewFlag,
                 int *pauseFlag) {
    if ((ch == 'w' || ch == 'W') && *slowGame > 1) {
        if (*slowGame <= 5) {
            *slowGame -= 1;
        } else if (*slowGame >= 200) {
            *slowGame -= 10;
        } else {
            *slowGame -= 5;
        }
    } else if (ch == 's' || ch == 'S') {
        if (*slowGame < 5) {
            *slowGame += 1;
        } else if (*slowGame >= 200) {
            *slowGame += 10;
        } else {
            *slowGame += 5;
        }
    } else if ((ch == 'q' || ch == 'Q') || (check_ending(arr))) {
        *flag = 0;
    } else if (ch == 'v' || ch == 'V') {
        if (*showParamsViewFlag == 0) {
            *showParamsViewFlag = 1;
        } else {
            *showParamsViewFlag = 0;
        }
    } else if (ch == 'p' || ch == 'P') {
        if (*pauseFlag == 0) {
            *pauseFlag = 1;
            if (*showParamsViewFlag) {
                mvprintw(0, 0, "Игра приостановлена");
            }
        } else {
            *pauseFlag = 0;
        }
    }
}

void outputSpeed(int num) { mvprintw(0, 0, "SLOW:%d", num); }

void outputWindow(int arr[][WIDTH]) {
    move(0, 0);
    for (int i = 0; i < HEIGHT; i++) {
        for (int j = 0; j < WIDTH; j++) {
            if (arr[i][j] == 1) {
                mvprintw(i, j, CELL_SYMBOL);
            } else {
                mvprintw(i, j, VOID_SYMBOL);
            }
        }
    }
}

void fullfillWindow(int arr[][WIDTH], char *argv[]) {
    int argvShiftHeight = 1;
    for (int i = 0; i < HEIGHT; i++) {
        for (int j = 0; j < WIDTH; j++) {
            if (argv[i + argvShiftHeight][j] == '1') {
                arr[i][j] = 1;
            } else {
                arr[i][j] = 0;
            }
        }
    }
}

void makeStep(int arr[][WIDTH]) {
    int buf[HEIGHT][WIDTH];
    for (int i = 0; i < HEIGHT; i++) {
        for (int j = 0; j < WIDTH; j++) {
            buf[i][j] = getNextCellState(arr, i, j);
        }
    }
    for (int i = 0; i < HEIGHT; i++) {
        for (int j = 0; j < WIDTH; j++) {
            arr[i][j] = buf[i][j];
        }
    }
}

int getNextCellState(int arr[][WIDTH], int n, int m) {
    int flag = 0;
    int counterNeighbours = 0;
    counterNeighbours = checkFullBorders(arr, n, m);
    if (arr[n][m] == 1) {
        if (counterNeighbours >= 2 && counterNeighbours <= 3) {
            flag = 1;
        } else {
            flag = 0;
        }
    } else if (arr[n][m] == 0) {
        if (counterNeighbours == 3) {
            flag = 1;
        } else {
            flag = 0;
        }
    }
    return flag;
}

int checkFullBorders(int arr[][WIDTH], int n, int m) {
    int counterNeighbours = 0;
    if (arr[(n + HEIGHT) % HEIGHT][(m - 1 + WIDTH) % WIDTH] == 1) {  // обработка клетки слева
        counterNeighbours++;
    }
    if (arr[(n - 1 + HEIGHT) % HEIGHT][(m - 1 + WIDTH) % WIDTH] ==
        1) {  // обработка клетки по диагонали влево вверх
        counterNeighbours++;
    }
    if (arr[(n - 1 + HEIGHT) % HEIGHT][(m + WIDTH) % WIDTH] == 1) {  // обработка клетки сверху
        counterNeighbours++;
    }
    if (arr[(n - 1 + HEIGHT) % HEIGHT][(m + 1 + WIDTH) % WIDTH] ==
        1) {  // обработка клетки по диагонали вправо вверх
        counterNeighbours++;
    }
    if (arr[(n + 1 + HEIGHT) % HEIGHT][(m - 1 + WIDTH) % WIDTH] ==
        1) {  // обработка клетки по диагонали влево вниз
        counterNeighbours++;
    }
    if (arr[(n + 1 + HEIGHT) % HEIGHT][(m + WIDTH) % WIDTH] == 1) {  // обработка клетки снизу
        counterNeighbours++;
    }
    if (arr[(n + 1 + HEIGHT) % HEIGHT][(m + 1 + WIDTH) % WIDTH] ==
        1) {  // обработка клетки по диагонали вправо вниз
        counterNeighbours++;
    }
    if (arr[(n + HEIGHT) % HEIGHT][(m + 1 + WIDTH) % WIDTH] == 1) {  // обработка клетки справа
        counterNeighbours++;
    }
    return counterNeighbours;
}

int check_ending(int arr[][WIDTH]) {
    int flag = 0;
    int check_empty_cells = 0;
    int check_last_frame = 1;
    int buf[HEIGHT][WIDTH];

    for (int i = 0; i < HEIGHT; i++) {
        for (int j = 0; j < WIDTH; j++) {
            buf[i][j] = getNextCellState(arr, i, j);
            if ((arr[i][j] == 1)) check_empty_cells = 1;
            if (arr[i][j] != buf[i][j]) check_last_frame = 0;
        }
    }
    if ((check_empty_cells != 1) || (check_last_frame == 1)) flag = 1;
    return flag;
}
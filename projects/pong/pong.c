#include <stdio.h>

int clear();
int play_zone(int score_l, int score_r, int ballsp_y, int ballsp_x);
int racket_left(int y, int mv);
int racket_right(int y, int mv);
int ball(int mv_bx, int mv_by, int mv_l, int mv_r);

int main() {
    int score_l = 0;
    int score_r = 0;
    int ballsp_y = 0;
    int ballsp_x = 1;
    int flag;
    while ((score_l != 21) && (score_r != 21)) {
        if (flag == 1) {
            score_l++;
            ballsp_x = 1;
        }
        if (flag == 2) {
            score_r++;
            ballsp_x = -1;
        }
        flag = play_zone(score_l, score_r, ballsp_y, ballsp_x);
    }
    (score_l == 21) ? printf("Первый игрок победил!!!") : printf("Второй игрок победил!!!");
    return 0;
}

int play_zone(int score_l, int score_r, int ballsp_y, int ballsp_x) {
    int mv_l = 11, mv_r = 11, mv_by = 12, mv_bx = 39, flag = 0;
    char next = '\n';
    while (1) {
        clear();
        int res = ball(mv_bx, mv_by, mv_l, mv_r);
        if (res == 1) ballsp_y *= -1;
        if (res == 2) {
            ballsp_x *= -1;
            ballsp_y = -1;
        }
        if (res == 3) {
            ballsp_y = 0;
            flag = 1;
        }
        if (res == 4) {
            ballsp_y = 0;
            flag = 2;
        }
        if (res == 5) {
            ballsp_x *= -1;
            ballsp_y = 1;
        }
        if (res == 6) {
            ballsp_x *= -1;
            ballsp_y *= 1;
        }
        if ((flag == 1) || (flag == 2)) break;
        mv_by += ballsp_y;
        mv_bx += ballsp_x;
        for (int y = 0; y < 25; y++) {
            for (int x = 0; x < 80; x++) {
                if (y == 0 || y == 24)
                    printf("-");
                else {
                    if (x == 0 || x == 79)
                        printf("*");
                    else {
                        if (x == 1) racket_left(y, mv_l);
                        if ((x == 40) && ((x != mv_bx) || (y != mv_by))) printf("|");
                        if (x == 78) racket_right(y, mv_r);
                        if (((x > 1 && x < 40) || (x > 40 && x < 78)) && ((x != mv_bx) || (y != mv_by)))
                            printf(" ");
                        if ((x == mv_bx) && (y == mv_by)) printf("o");
                    }
                }
                if (x == 79) printf("\n");
            }
        }
        printf("                                       %d %d\n", score_r, score_l);
        next = getchar();
        if ((next == 'a') && (mv_l != 1)) mv_l--;
        if ((next == 'z') && (mv_l != 21)) mv_l++;
        if ((next == 'k') && (mv_r != 1)) mv_r--;
        if ((next == 'm') && (mv_r != 21)) mv_r++;
    }
    return flag;
}

int clear() {
    for (int i = 0; i < 30; i++) {
        printf("\n");
    }
    return 0;
}

int racket_left(int y, int mv_l) {
    if ((y == mv_l) || (y == mv_l + 1) || (y == mv_l + 2)) {
        printf("|");
    } else {
        printf(" ");
    }
    return 0;
}

int racket_right(int y, int mv_r) {
    if ((y == mv_r) || (y == mv_r + 1) || (y == mv_r + 2)) {
        printf("|");
    } else {
        printf(" ");
    }
    return 0;
}

int ball(int mv_bx, int mv_by, int mv_l, int mv_r) {
    int res;
    if ((mv_by == 23) || (mv_by == 1)) res = 1;
    if (mv_bx == 77) {
        if (mv_by == (mv_r))
            res = 2;
        else if (mv_by == (mv_r + 1))
            res = 6;
        else if (mv_by == (mv_r + 2))
            res = 5;
        else {
            res = 3;
        }
    }
    if (mv_bx == 2) {
        if (mv_by == (mv_l))
            res = 2;
        else if (mv_by == (mv_l + 1))
            res = 6;
        else if (mv_by == (mv_l + 2))
            res = 5;
        else {
            res = 4;
        }
    }
    return res;
}

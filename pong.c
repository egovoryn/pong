// Copyright 2021 <Egor Oleynik>

#include "pong.h"

int main() {
    Rules();
    char key = getchar();
    if (key == 'f' || key == 'F') {
        struct pong pong = {12, 12, 30, 10, -1, 0, 0};
        initscr();
        noecho();
        timeout(0);
        struct timespec tim1, tim2;
        tim1.tv_sec = 0;
        tim1.tv_nsec = SPEED;
        Draw(pong);
        while ((pong.rightwin != WIN) && (pong.leftwin != WIN)) {
            system("/bin/stty raw");
            key = getch();
            system("/bin/stty cooked");
            if (key) {
                if (key == 'q' || key == 'Q') break;  // exit
                PressKey(key, &pong);
                printf("\e[1;1H\e[2J\e[3J");  // clear stdin
            }
            ChangeDirection(&pong);
            nanosleep(&tim1, &tim2);
            endwin();
        }
        Final(&pong);
    } else {
        printf("\33c\e[3J");
        printf("\tYou completed the game. Good luck!\n");
    }
    return 0;
}

void PressKey(int key, struct pong *pong) {
    if ((key == LEFT_UP || key == LEFT_UP + REG) && pong->rack_left != 2) {
        pong->rack_left--;
    } else if ((key == LEFT_DOWN || key == LEFT_DOWN + REG) && pong->rack_left != 22) {
        pong->rack_left++;
    } else if ((key == RIGHT_UP || key == RIGHT_UP + REG) && pong->rack_right != 2) {
        pong->rack_right--;
    } else if ((key == RIGHT_DOWN || key == RIGHT_DOWN + REG) && pong->rack_right != 22) {
        pong->rack_right++;
    }
}

void ChangeDirection(struct pong *pong) {
    if (pong->ball_y == 1) pong->direct_ball /= 3;
    if (pong->ball_y == 23) pong->direct_ball *= 3;

    if (pong->ball_x == 0 || pong->ball_x == 80) RandomSpawnBall(pong);

    if (pong->ball_x == 3) {
        if ((pong->ball_y == pong->rack_left + 2 || pong->ball_y == pong->rack_left - 2)
        || IsLeftRacket(pong->ball_y, pong->rack_left)) {
            if (pong->direct_ball == -1) pong->direct_ball = -3;
            else if (pong->direct_ball == -3) pong->direct_ball = -1;
        }
    } else if (pong->ball_x == 76) {
        if ((pong->ball_y == pong->rack_right + 2 || pong->ball_y == pong->rack_right - 2)
        || IsRightRacket(pong->ball_y, pong->rack_right)) {
            if (pong->direct_ball == 1) pong->direct_ball = 3;
            else if (pong->direct_ball == 3) pong->direct_ball = 1;
        }
    } else if (pong->ball_x == 4) {
        if (pong->ball_y == pong->rack_left + 2 || pong->ball_y == pong->rack_left - 2) {
            if (pong->direct_ball == -1) pong->direct_ball = 1;
            else if (pong->direct_ball == -3) pong->direct_ball = 3;
        } else if (IsLeftRacket(pong->ball_y, pong->rack_left)) {
            if (pong->direct_ball == -1) pong->direct_ball = 1;
            if (pong->direct_ball == -3) pong->direct_ball = 3;
        }
    } else if (pong->ball_x == 75) {
        if (pong->ball_y == pong->rack_right + 2 || pong->ball_y == pong->rack_right - 2) {
            if (pong->direct_ball == 3) pong->direct_ball = -3;
            else if (pong->direct_ball == 1) pong->direct_ball = -1;
        } else if (IsRightRacket(pong->ball_y, pong->rack_right)) {
            if (pong->direct_ball == 1) pong->direct_ball = -1;
            else if (pong->direct_ball == 3) pong->direct_ball = -3;
        }
    }
    DirectionMove(pong);
}

void DirectionMove(struct pong *pong) {
    switch (pong->direct_ball) {
        case 1:
            pong->ball_x++;
            pong->ball_y++;
            break;
        case 2:
            pong->ball_x++;
            break;
        case 3:
            pong->ball_x++;
            pong->ball_y--;
            break;
        case -1:
            pong->ball_x--;
            pong->ball_y++;
            break;
        case -2:
            pong->ball_x--;
            break;
        case -3:
            pong->ball_x--;
            pong->ball_y--;
            break;
        default:
            break;
    }
    Draw(*pong);
    Score(pong);
}

void Draw(struct pong pong) {
    printf("\33c\e[3J");
    for (int y = 0; y < 25; y++) {
        for (int x = 0; x < 80; x++) {
            if (x == pong.ball_x && y == pong.ball_y) {
                printf("%c", 'o');
            } else if (y == 0 || y == 24) {
                printf("%c", '-');
            } else if (x == 0 || x == 79 || x == 40) {
                printf("%c", '|');
            } else if (x == 3 && IsLeftRacket(y, pong.rack_left)) {
                printf("%c", 'H');
            } else if (x == 76 && IsRightRacket(y, pong.rack_right)) {
                printf("%c", 'H');
            } else {
                printf("%c", ' ');
            }
        }
        printf("\n");
    }
}

void Score(struct pong *pong) {
    if (pong->ball_x == 0) pong->rightwin++;
    else if (pong->ball_x == 80) pong->leftwin++;
    printf("\t\t\t\t      %d - %d\n", pong->leftwin, pong->rightwin);
    if (pong->leftwin == WIN) printf("\t\t\tPlayer 1 - WIN. Congratulations!\n");
    if (pong->rightwin == WIN) printf("\t\t\tPlayer 2 - WIN. Congratulations!\n");
}

void Final(struct pong *pong) {
    if (pong->rightwin == WIN) pong->rightwin--;
    else if (pong->leftwin == WIN) pong->leftwin--;

    Draw(*pong);
    Score(pong);
}

void Rules() {
    printf("\33c\e[3J");
    printf("\n\t\tWelcome, stranger!\n");
    printf("This is a game for two players. I hope you're not alone.\n\n");
    printf("  Press Q to exit | A & Z – Player 1 | M & K – Player 2\n\n");
    printf("\t   Press F to start the game.\n");
}

int IsLeftRacket(int y, int rack_left) {
    return (y == rack_left || y == rack_left + 1 || y == rack_left - 1) ? 1 : 0;
}

int IsRightRacket(int y, int rack_right) {
    return (y == rack_right || y == rack_right + 1 || y == rack_right - 1) ? 1 : 0;
}

void RandomSpawnBall(struct pong *pong) {
    int lower_x = 40, upper_x = 65;
    int lower_y = 3, upper_y = 22;
    if (pong->ball_x == 80) lower_x = 15, upper_x = 40;
    srand(time(0));

    pong->ball_x = (rand() % (upper_x - lower_x + 1)) + lower_x;
    pong->ball_y = (rand() % (upper_y - lower_y + 1)) + lower_y;
    pong->direct_ball *= -1;
}

#ifndef SRC_PONG_H_
#define SRC_PONG_H_

#include <curses.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

#define WIN 5                // win score
#define SPEED 100 * 1000000  // microsec

#define LEFT_UP 'a'
#define LEFT_DOWN 'z'
#define RIGHT_UP 'k'
#define RIGHT_DOWN 'm'
#define REG ('A' - 'a')

typedef struct pong {
    int rack_left;    // left racket position
    int rack_right;   // right racket position
    int ball_x;       // X position of the ball
    int ball_y;       // Y position of the ball
    int direct_ball;  // direction of the ball
    int leftwin;      // score counter for the left player
    int rightwin;     // score counter for the right player
} pong;

void PressKey(int key, struct pong *pong);  // control
void Draw(struct pong pong);                // rendering graphics
void ChangeDirection(struct pong *pong);    // change of direction of the ball
void DirectionMove(struct pong *pong);      // ball movement
void Score(struct pong *pong);              // ball movement after determining direction
int IsLeftRacket(int y, int rack_left);     // find out the position of the left racket
int IsRightRacket(int y, int rack_right);   // find out the position of the right racket
void Final(struct pong *pong);              // final score and field
void Rules();                               // rules of the game
void RandomSpawnBall(struct pong *pong);    // randow spawn for ball after goal

#endif  // SRC_PONG_H_

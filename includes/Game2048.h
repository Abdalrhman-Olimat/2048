#ifndef GAME2048_H
#define GAME2048_H


#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#define COLOR_EMPTY 1
#define COLOR_2     2
#define COLOR_4     3
#define COLOR_8     4
#define COLOR_16    5
#define COLOR_32    6
#define COLOR_64    7
#define COLOR_128   8
#define COLOR_256   9
#define COLOR_512   10
#define COLOR_1024  11
#define COLOR_2048  12
#define COLOR_OTHER 13
#define COLOR_BORDER 14
#define COLOR_TEXT  15

#define MENU_PLAY 0
#define MENU_EXIT 1
#define MENU_OPTIONS 2

typedef struct s_grid {
   int grid[4][4];
} t_grid;

#endif
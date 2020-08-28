#ifndef CONFIG_H
#define CONFIG_H

#include <QPair>
#include <QVector>
#include <QPainter>

typedef QVector<QPair<unsigned, unsigned>> myVec;

#define CELL_LENGTH 10

/* shape */
#define CUR_SHAPE_MOVING_SPEED_SLOW 1;  // how many ROWS moves per timeInterval. Attention, not pixels
#define CUR_SHAPE_MOVING_SPEED_FAST 3;

/* mainBoard */
#define MAIN_BOARD_X 10  // mainWindow's (x,y)
#define MAIN_BOARD_Y 10
#define MAIN_BOARD_WIDTH 400
#define MAIN_BOARD_HEIGHT 560
/* nextBoard */
#define NEXT_BOARD_X 420  // nextWindow's (x,y)
#define NEXT_BOARD_Y 30
#define NEXT_BOARD_WIDTH 120
#define NEXT_BOARD_HEIGHT 120

/* mainWindow */
/* timer */
#define TIMER_INTERVAL_FAST 50
#define TIMER_INTERVAL_SLOW 200

#define NUMS_OF_COLOR 7
#define NUMS_OF_SHAPETYPE 7


extern QPen pen;
extern QBrush brush;

#endif  // CONFIG_H

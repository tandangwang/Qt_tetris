#ifndef BOARD_H
#define BOARD_H

#include <QPainter>

#include "cell.h"
#include "config.h"

// forward declaration
class cell;

class board
{
public:
    board(unsigned _X, unsigned _Y, unsigned _w, unsigned _h);
    ~board();
    /* member function */
    unsigned eliminateFullRows();  // eliminate all full rows and count the rows
    void setCellWithColor(unsigned _row, unsigned _col, unsigned _color);
    void clearBoard();  // set all visible cells' color to white and invisble

    inline unsigned getTopLeftX() const { return m_X; }
    inline unsigned getTopLeftY() const { return m_Y; }
    inline unsigned getWidth() const { return m_width; }
    inline unsigned getHeight() const { return m_height; }
    inline unsigned getNumOfRows() const { return m_numOfRows; }
    inline unsigned getNumOfCols() const { return m_numOfCols; }
    inline bool isCellVisible(unsigned _row, unsigned _col) const { return m_cells[_row][_col].isVisible(); }

    void drawBoard(QPainter& _painter) const;

private:
    /* data member */
    unsigned m_X;
    unsigned m_Y;
    unsigned m_width;
    unsigned m_height;

    //    static const unsigned m_numOfRows = MAIN_BOARD_HEIGHT / CELL_LENGTH;
    //    static const unsigned m_numOfCols = MAIN_BOARD_WIDTH / CELL_LENGTH;
    unsigned m_numOfRows;
    unsigned m_numOfCols;

    cell** m_cells;                     // from right to left, m_cells is a pointer to cell*
    QVector<cell*>* m_vecVisibleCells;  // need to be done later

    /* encapsulation */
    unsigned m_lowestFullRow = m_numOfRows;  // default invalid row index ( [0,m_numOfRows-1] is valid range)
    bool isFullRowExistAndGetLowestRow();
    void remarkLowestFullRow() { m_lowestFullRow = m_numOfRows; }
    void moveDownRowsAboveLowestFullRow();
    unsigned findLowestBlankRow() const;
};

#endif  // BOARD_H

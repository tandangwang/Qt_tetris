#ifndef SHAPE_H
#define SHAPE_H

#include <QPair>
#include <QVector>

#include "board.h"
#include "cell.h"

enum MOVE_DIRECTION
{
    MOVE_DOWN,
    MOVE_LEFT,
    MOVE_RIGHT
};

class shape
{
public:
    shape();
    shape(unsigned _randValue, board* _board);  // place at board center
    ~shape();

    /* <1> set */
    void initCells(unsigned _randValue, board* _board);  // set property of cells,color_type_Locations
    void setLocation(unsigned _row, unsigned _col, board* _board);

    inline unsigned getRow() const
    {
        return m_row;
    }
    inline unsigned getCol() const
    {
        return m_col;
    }
    inline unsigned getColor() const
    {
        return m_color;
    }
    inline unsigned getShapeType() const
    {
        return m_shapeType;
    }
    myVec& getCellsLocation() const;
    unsigned getLowestCellRow() const;

    /* <2> move shape */
    void rotateShape(unsigned _centerCellIndex, board* _board);
    void autoMoveDown(unsigned _moveSpeed, board* _board);
    void moveLeft(unsigned _moveSpeed, board* _board);
    void moveRight(unsigned _moveSpeed, board* _board);

    void checkBorder_moveCells(MOVE_DIRECTION value, unsigned _speed, board* _board);

    void drawShape(QPainter& _painter) const;

private:
    /* data member */
    cell* m_cells;
    unsigned m_shapeType;
    unsigned m_color;
    unsigned m_X;
    unsigned m_Y;
    unsigned m_row;
    unsigned m_col;
    myVec* m_locations;
    bool bNeedRestoreLocation = false;

    /* encapsulation */
    void calPosition(board* _board);
    void calFirstCellLocation(board* _board);  // (x,y) to (row,col), m_row and m_col only have value on board
    void setCellsColor(unsigned _color);       // set 4 cells' color to same
    void initCellsLocations(board* _board);
    void borderCheck(unsigned& aimRow, unsigned& aimCol, board* _board, myVec& vOrigLoc, cell& cell);
};

#endif  // SHAPE_H

#ifndef CELL_H
#define CELL_H

#include <QPainter>
#include <QRect>

#include "config.h"

class cell
{
public:
    cell();
    ~cell() {}

    /* member function */
    inline void setColor(unsigned _color)
    {
        m_color = _color;
    }
    inline void setVisibleState(bool _bValue)
    {
        m_bVisible = _bValue;
    }
    inline unsigned getColor() const
    {
        return m_color;
    }
    inline bool isVisible() const
    {
        return m_bVisible;
    }

    //    inline QPair<unsigned, unsigned> getCellLocation() const { return qMakePair(m_row, m_col);}

    /* change where to put the cell */
    void setLocation(unsigned _row, unsigned _col, unsigned _boardOriginX, unsigned _boardOriginY);
    QPair<unsigned, unsigned> getCellLocation() const
    {
        return qMakePair(m_row, m_col);
    }
    QPair<unsigned, unsigned> getCellPosition() const
    {
        return qMakePair(m_X, m_Y);
    }


    /* draw cell */
    void drawCell(QPainter& _painter) const;

private:
    /* data member */
    unsigned m_sideLength;
    unsigned m_X;
    unsigned m_Y;
    unsigned m_row;
    unsigned m_col;
    unsigned m_color;
    bool m_bVisible;

    /* encapsulation */
};

#endif  // CELL_H

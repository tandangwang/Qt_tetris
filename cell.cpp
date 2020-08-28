#include "cell.h"

cell::cell()
{
    m_sideLength = CELL_LENGTH;
    m_X = 0;
    m_Y = 0;
    m_row = 0;
    m_col = 0;
    m_color = 0;
    m_bVisible = false;
}

void cell::setLocation(unsigned _row, unsigned _col, unsigned _boardOriginX, unsigned _boardOriginY)
{
    m_row = _row;
    m_col = _col;
    m_X = _boardOriginX + m_col * CELL_LENGTH;
    m_Y = _boardOriginY + m_row * CELL_LENGTH;
}

void cell::drawCell(QPainter& _painter) const
{
    // use blow if, avoid board's invisible cell involk blow drawRect(), CPU consuming
    if(!m_bVisible)
        return;
    switch(m_color)
    {
        case 0:  // L
            brush.setColor(Qt::red);
            break;
        case 1:  // J
            brush.setColor(Qt::green);
            break;
        case 2:  // T
            brush.setColor(Qt::blue);
            break;
        case 3:  // O
            brush.setColor(Qt::cyan);
            break;
        case 4:  // Z
            brush.setColor(Qt::magenta);
            break;
        case 5:  // S
            brush.setColor(Qt::yellow);
            break;
        case 6:  // I
            brush.setColor(Qt::gray);
            break;
        default:
            break;
    }
    brush.setStyle(Qt::SolidPattern);
    _painter.setBrush(brush);
    // draw rect (cell frame)
    _painter.drawRect(m_X, m_Y, m_sideLength, m_sideLength);
}

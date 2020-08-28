#include "board.h"

board::board(unsigned _X, unsigned _Y, unsigned _w, unsigned _h) : m_X(_X), m_Y(_Y), m_width(_w), m_height(_h)
{
    m_numOfRows = _h / CELL_LENGTH;
    m_numOfCols = _w / CELL_LENGTH;
    /* cells */
    m_cells = new cell*[m_numOfRows];  // an array of type 'cell*'
    for(unsigned i = 0; i < m_numOfRows; i++)
        m_cells[i] = new cell[m_numOfCols];
    // cal location of every cells on board
    for(unsigned i = 0; i < m_numOfRows; i++)
        for(unsigned j = 0; j < m_numOfCols; j++)
            m_cells[i][j].setLocation(i, j, m_X, m_Y);
    /* record visible cells */
    m_vecVisibleCells = new QVector<cell*>;
}

board::~board()
{
    for(unsigned i = 0; i < m_numOfRows; i++)
        delete[] m_cells[i];  // first delete level1 pointer
    delete[] m_cells;  // then delete level2 pointer
    delete m_vecVisibleCells;  // elements in vec alrealy been release by above steps
}

unsigned board::eliminateFullRows()
{
    /* -------------------------------------------------
     * 1. from bottom to top, find a full row;
     * 2. eleminate the full row;
     * 3. move down above rows;
     * 4. repeat the procedure until no full rows found;
     *--------------------------------------------------*/
    unsigned count = 0;  // count how many full rows are eliminated
    while(isFullRowExistAndGetLowestRow())   // m_FirstFullRow is assigned value in isFullRowsExist()
    {
        moveDownRowsAboveLowestFullRow();
        remarkLowestFullRow();  // in case next time use the value left here
        count++;
    }
    return count;
}

void board::setCellWithColor(unsigned _row, unsigned _col, unsigned _color)
{
    m_cells[_row][_col].setVisibleState(true);
    m_cells[_row][_col].setColor(_color);
}

void board::clearBoard()
{
    for(unsigned row = 0; row < m_numOfRows; row++)
        for(unsigned col = 0; col < m_numOfCols; col++)
        {
            // only handle current visible cells
            if(m_cells[row][col].isVisible())
            {
                m_cells[row][col].setColor(Qt::white);
                m_cells[row][col].setVisibleState(false);
            }
        }
}

void board::drawBoard(QPainter& _painter) const
{
    // draw outside frame
    pen.setWidth(3);
    _painter.setPen(pen);
    brush.setColor(Qt::white);
    _painter.setBrush(brush);
    _painter.drawRect(m_X, m_Y, m_width, m_height);
    // draw inside cells
    pen.setWidth(1);
    _painter.setPen(pen);
    for(unsigned row = 0; row < m_numOfRows; row++)
    {
        for(unsigned col = 0; col < m_numOfCols; col++)
        {
            if(m_cells[row][col].isVisible())
                m_cells[row][col].drawCell(_painter);
        }
    }
}

bool board::isFullRowExistAndGetLowestRow()
{
    // check from board's bottom to top
    for(int i = m_numOfRows - 1; i >= 0; i--)   // use 'int' to avoid i>=0 always true
    {
        unsigned countVisibleCells = 0;
        // go through from column's left to right
        for(unsigned j = 0; j < m_numOfCols; j++)
        {
            if(!m_cells[(unsigned)i][j].isVisible())   // if exist invisible cell, skip and move on to next row
                break;
            else
            {
                // no need to juedge all rows, only if one full row exist, return true
                if(++countVisibleCells == m_numOfCols)
                {
                    m_lowestFullRow = (unsigned)i;
                    return true;
                }
            }
        }
    }
    // after above traverse all rows of the board, still no full row exist, return false
    return false;
}

void board::moveDownRowsAboveLowestFullRow()
{
    /* ------------------------------------
     * start from the row _aFullRow,
     * copy its above row cells' visible state and color,
     * attention: don't deal with same in
     * -----------------------------------*/
    unsigned lowestBlankRow = findLowestBlankRow();
    unsigned curRow = m_lowestFullRow;
    while(curRow > lowestBlankRow)
    {
        for(unsigned i = 0; i < m_numOfCols; i++)
        {
            // cell below and cell above are both invisible, no need to copy
            if(!m_cells[curRow][i].isVisible() && !m_cells[curRow - 1][i].isVisible())
                continue;
            // run to here, visibleState of two cells must be all visible or only one is visible
            bool aboveCellVisibleState = m_cells[curRow - 1][i].isVisible();
            m_cells[curRow][i].setVisibleState(aboveCellVisibleState);
            unsigned aboveCellColor = m_cells[curRow - 1][i].getColor();
            m_cells[curRow][i].setColor(aboveCellColor);
        }
        curRow--;  // deal with next row
    }
}

unsigned board::findLowestBlankRow() const
{
    for(unsigned i = m_numOfRows - 1; i >= 0; i--)
    {
        unsigned countInvisibleCells = 0;
        for(unsigned j = 0; j < m_numOfCols; j++)
        {
            // if exist a visible cell, means the row is not blank, judge next row
            if(m_cells[i][j].isVisible())
                break;
            else
            {
                // no need to juedge all rows, only if one full row exist, return true
                if(++countInvisibleCells == m_numOfCols)
                    return i;
            }
        }
    }
    return m_numOfRows;  // return an invalid row index
}

#include "shape.h"

#include <QColor>

#include "config.h"

shape::shape()
{
    m_cells = nullptr;
    m_locations = nullptr;
}

shape::shape(unsigned _randValue, board* _board)
{
    m_cells = new cell[4];
    initCells(_randValue, _board);
    m_color = m_cells[0].getColor();  // important,otherwise leaveshadow can't copy this m_color
    m_locations = new myVec;
}

shape::~shape()
{
    if(m_cells != nullptr)
        delete[] m_cells;
    delete m_locations;
}

myVec& shape::getCellsLocation() const
{
    m_locations->clear();
    for(unsigned i = 0; i < 4; i++)
        m_locations->push_back(m_cells[i].getCellLocation());
    return *m_locations;
}

unsigned shape::getLowestCellRow() const
{
    QVector<unsigned> vShapeCellsRows;
    for(unsigned i = 0; i < 4; i++)
        vShapeCellsRows.push_back(m_cells[i].getCellLocation().first);
    auto it = std::max_element(vShapeCellsRows.begin(), vShapeCellsRows.end());
    return *it;
}

void shape::rotateShape(unsigned _centerCellIndex, board* _board)
{
    /* -------------------------------------------------------------
     * rotate orientation is counter-clockwise;
     * will not change (row,col) of entered cell (center of circle);
     * ------------------------------------------------------------*/
    // backup original location (row,col)
    myVec vOrigLoc = getCellsLocation();
    unsigned centerCellRow = m_cells[_centerCellIndex].getCellLocation().first;
    unsigned centerCellCol = m_cells[_centerCellIndex].getCellLocation().second;
    enum RelativePosition
    {
        top,
        left,
        bottom,
        right,
        top_right,
        top_left,
        bottom_left,
        bottom_right
    } enumPosition;
    for(unsigned i = 0; i < 4; i++)
    {
        int rowDifference = m_cells[i].getCellLocation().first - centerCellRow;
        int colDifference = m_cells[i].getCellLocation().second - centerCellCol;
        // get relative position
        if(rowDifference < 0)
        {
            if(colDifference > 0)
                enumPosition = RelativePosition::top_right;
            else if(colDifference == 0)
                enumPosition = RelativePosition::top;
            else if(colDifference < 0)
                enumPosition = RelativePosition::top_left;
        }
        else if(rowDifference == 0)
        {
            if(colDifference > 0)
                enumPosition = RelativePosition::right;
            else if(colDifference < 0)
                enumPosition = RelativePosition::left;
        }
        else if(rowDifference > 0)
        {
            if(colDifference > 0)
                enumPosition = RelativePosition::bottom_right;
            else if(colDifference == 0)
                enumPosition = RelativePosition::bottom;
            else if(colDifference < 0)
                enumPosition = RelativePosition::bottom_left;
        }
        // rotate a cell
        switch(enumPosition)
        {
            case top:
                {
                    unsigned aimRow = m_cells[i].getCellLocation().first + (-rowDifference);
                    unsigned aimCol = m_cells[i].getCellLocation().second - (-rowDifference);
                    borderCheck(aimRow, aimCol, _board, vOrigLoc, m_cells[i]);
                }
                break;
            case left:
                {
                    unsigned aimRow = m_cells[i].getCellLocation().first + (-colDifference);
                    unsigned aimCol = m_cells[i].getCellLocation().second + (-colDifference);
                    borderCheck(aimRow, aimCol, _board, vOrigLoc, m_cells[i]);
                }
                break;
            case bottom:
                {
                    unsigned aimRow = m_cells[i].getCellLocation().first - rowDifference;
                    unsigned aimCol = m_cells[i].getCellLocation().second + rowDifference;
                    borderCheck(aimRow, aimCol, _board, vOrigLoc, m_cells[i]);
                }
                break;
            case right:
                {
                    unsigned aimRow = m_cells[i].getCellLocation().first - colDifference;
                    unsigned aimCol = m_cells[i].getCellLocation().second - colDifference;
                    borderCheck(aimRow, aimCol, _board, vOrigLoc, m_cells[i]);
                }
                break;
            case top_right:
                {
                    unsigned aimRow = m_cells[i].getCellLocation().first;
                    unsigned aimCol = m_cells[i].getCellLocation().second - 2;
                    borderCheck(aimRow, aimCol, _board, vOrigLoc, m_cells[i]);
                }
                break;
            case top_left:
                {
                    unsigned aimRow = m_cells[i].getCellLocation().first + 2;
                    unsigned aimCol = m_cells[i].getCellLocation().second;
                    borderCheck(aimRow, aimCol, _board, vOrigLoc, m_cells[i]);
                }
                break;
            case bottom_left:
                {
                    unsigned aimRow = m_cells[i].getCellLocation().first;
                    unsigned aimCol = m_cells[i].getCellLocation().second + 2;
                    borderCheck(aimRow, aimCol, _board, vOrigLoc, m_cells[i]);
                }
                break;
            case bottom_right:
                {
                    unsigned aimRow = m_cells[i].getCellLocation().first - 2;
                    unsigned aimCol = m_cells[i].getCellLocation().second;
                    borderCheck(aimRow, aimCol, _board, vOrigLoc, m_cells[i]);
                }
                break;
            default:
                break;
        }
        if(bNeedRestoreLocation)
        {
            bNeedRestoreLocation = false; // reset to default
            return;
        }
    }
}

void shape::autoMoveDown(unsigned _moveSpeed, board* _board)
{
    checkBorder_moveCells(MOVE_DIRECTION::MOVE_DOWN, _moveSpeed, _board);
}

void shape::moveLeft(unsigned _moveSpeed, board* _board)
{
    //    for (unsigned i = 0; i < 4; i++)
    //    {
    //        unsigned oldRow = m_cells[i].getCellLocation().first;
    //        unsigned newCol = m_cells[i].getCellLocation().second - _moveSpeed;
    //        unsigned boardOriginX = _board->getTopLeftX();
    //        unsigned boardOriginY = _board->getTopLeftY();
    //        m_cells[i].setLocation(oldRow, newCol, boardOriginX, boardOriginY);
    //    }
    checkBorder_moveCells(MOVE_DIRECTION::MOVE_LEFT, _moveSpeed, _board);
}

void shape::moveRight(unsigned _moveSpeed, board* _board)
{
    //    for (unsigned i = 0; i < 4; i++)
    //    {
    //        unsigned oldRow = m_cells[i].getCellLocation().first;
    //        unsigned newCol = m_cells[i].getCellLocation().second + _moveSpeed;
    //        unsigned boardOriginX = _board->getTopLeftX();
    //        unsigned boardOriginY = _board->getTopLeftY();
    //        m_cells[i].setLocation(oldRow, newCol, boardOriginX, boardOriginY);
    //    }
    checkBorder_moveCells(MOVE_DIRECTION::MOVE_RIGHT, _moveSpeed, _board);
}

void shape::checkBorder_moveCells(MOVE_DIRECTION _enumrator, unsigned _offset, board* _board)
{
    /* ----------------------------------------------------------
     * # all cells in shape move same distance always;
     * # when move to a direction, distance Max is the
     *   direction-most cell to the border;
     * # there are 3 types of distance:
     *   distanceToBorder,distanceToBlockCell,distanceTry;
     * (below use 'len' represents 'distance', 'obstacle' represents
     * 'visible cell that blocked way to move'
     * ---------------------------------------------------------*/
    // get the left-most cell's col
    myVec& vShapeCellsLocations = getCellsLocation();
    QVector<unsigned> vShapeCellsRows, vShapeCellsCols;
    for(unsigned i = 0; i < 4; i++)
    {
        vShapeCellsRows.push_back(vShapeCellsLocations[i].first);
        vShapeCellsCols.push_back(vShapeCellsLocations[i].second);
    }
    // sort to ascending order
    std::sort(vShapeCellsRows.begin(), vShapeCellsRows.end());
    std::sort(vShapeCellsCols.begin(), vShapeCellsCols.end());
    switch(_enumrator)
    {
        case MOVE_DIRECTION::MOVE_DOWN:
            {
                /* -------------------------------------------------------------
                 * Situation1: when shape reached border,can't move anymore
                 * Situation2: when automove or long-press to move a len
                 * Situation3: when shape was blocked by a visible cell on board
                 * -------------------------------------------------------------*/
                unsigned lenToBorder = (_board->getNumOfRows() - 1) - vShapeCellsRows[3];  // get the bottom-most
                if(lenToBorder == 0)
                    return;
                unsigned lenTry = _offset;
                /* -------------------------------------------------------------
                 * Situation3:
                 * # why traverse all 4 cells of shape?
                 *   Because each cell in shape may be blocked while other 3 not;
                 * # when more than one cell is blocked, cal each cell's able
                 *   moving len, choose the smallest one for shape;
                 * -------------------------------------------------------------*/
                QVector<unsigned> vLensToObstacle;
                for(unsigned i = 0; i < 4; i++)
                {
                    // start from the cell below m_cells[i], end at cell above tempNewRow
                    unsigned startRow = vShapeCellsLocations[i].first + 1;
                    unsigned endRow = vShapeCellsLocations[i].first + (lenToBorder <= lenTry ? lenToBorder : lenTry);
                    unsigned fixedCol = vShapeCellsLocations[i].second;
                    // curObstacleCellRow represents the blockedCell's row
                    for(unsigned curObstacleCellRow = startRow; curObstacleCellRow <= endRow; curObstacleCellRow++)
                    {
                        if(_board->isCellVisible(curObstacleCellRow, fixedCol))
                        {
                            // store the len from the blocked cell to m_cells[i]
                            unsigned ableLen = curObstacleCellRow - vShapeCellsLocations[i].first - 1;
                            vLensToObstacle.push_back(ableLen);
                            break;
                        }
                    }
                }
                unsigned lenToObstacle = 0;
                unsigned finalLen = 0;
                if(vLensToObstacle.isEmpty())
                    finalLen = (lenToBorder <= lenTry ? lenToBorder : lenTry);
                else
                {
                    auto itObstacle = std::min_element(vLensToObstacle.begin(), vLensToObstacle.end());
                    lenToObstacle = *itObstacle;
                    QVector<unsigned> vLens{lenToBorder, lenTry, lenToObstacle};
                    auto itFinal = std::min_element(vLens.begin(), vLens.end());
                    finalLen = *itFinal;
                }
                unsigned newRow = m_row + finalLen;
                setLocation(newRow, m_col, _board);
            }
            break;
        case MOVE_DIRECTION::MOVE_LEFT:
            {
                unsigned lenToBorder = vShapeCellsCols[0];  // get the left-most
                if(lenToBorder == 0)
                    return;
                unsigned lenTry = _offset;
                QVector<unsigned> vLensToObstacle;
                for(unsigned i = 0; i < 4; i++)
                {
                    unsigned startCol = vShapeCellsLocations[i].second - 1;
                    unsigned endCol = vShapeCellsLocations[i].second - (lenToBorder <= lenTry ? lenToBorder : lenTry);
                    unsigned fixedRow = vShapeCellsLocations[i].first;
                    for(unsigned curObstacleCellCol = startCol; curObstacleCellCol >= endCol; curObstacleCellCol--)
                    {
                        if(_board->isCellVisible(fixedRow, curObstacleCellCol))
                        {
                            unsigned ableLen = vShapeCellsLocations[i].second - curObstacleCellCol - 1;
                            if(ableLen == 0)
                                return;
                            vLensToObstacle.push_back(ableLen);
                            break;
                        }
                        /* -----------------------------------------------
                         * 1. when curObstacleCellCol == 0, this must be first round loop,above already
                         *    considered the situation;
                         * 2. due to curObstacleCellCol type is unsigned, can't curObstacleCellCol--,as it
                         *    result will be a huge large number,certainly will > endCol,then will step into
                         *    second round of loop, pass a wrong col value to _board->isCellVisible(),error
                         * ----------------------------------------------*/
                        if(curObstacleCellCol == 0)
                            break;
                    }
                }
                unsigned lenToObstacle = 0;
                unsigned finalLen = 0;
                if(vLensToObstacle.isEmpty())
                    finalLen = (lenToBorder <= lenTry ? lenToBorder : lenTry);
                else
                {
                    auto itObstacle = std::min_element(vLensToObstacle.begin(), vLensToObstacle.end());
                    lenToObstacle = *itObstacle;
                    QVector<unsigned> vLens{lenToBorder, lenTry, lenToObstacle};
                    auto itFinal = std::min_element(vLens.begin(), vLens.end());
                    finalLen = *itFinal;
                }
                unsigned newCol = m_col - finalLen;
                setLocation(m_row, newCol, _board);
            }
            break;
        case MOVE_DIRECTION::MOVE_RIGHT:
            {
                unsigned lenToBorder = (_board->getNumOfCols() - 1) - vShapeCellsCols[3];  // get the left-most
                if(lenToBorder == 0)
                    return;
                unsigned lenTry = _offset;
                QVector<unsigned> vLensToObstacle;
                for(unsigned i = 0; i < 4; i++)
                {
                    unsigned startCol = vShapeCellsLocations[i].second + 1;
                    unsigned endCol = vShapeCellsLocations[i].second + (lenToBorder <= lenTry ? lenToBorder : lenTry);
                    unsigned fixedRow = vShapeCellsLocations[i].first;
                    for(unsigned curObstacleCellCol = startCol; curObstacleCellCol <= endCol; curObstacleCellCol++)
                    {
                        if(_board->isCellVisible(fixedRow, curObstacleCellCol))
                        {
                            unsigned ableLen = curObstacleCellCol - vShapeCellsLocations[i].second - 1;
                            if(ableLen == 0)
                                return;
                            vLensToObstacle.push_back(ableLen);
                            break;
                        }
                    }
                }
                unsigned lenToObstacle = 0;
                unsigned finalLen = 0;
                if(vLensToObstacle.isEmpty())
                    finalLen = (lenToBorder <= lenTry ? lenToBorder : lenTry);
                else
                {
                    auto itObstacle = std::min_element(vLensToObstacle.begin(), vLensToObstacle.end());
                    lenToObstacle = *itObstacle;
                    QVector<unsigned> vLens{lenToBorder, lenTry, lenToObstacle};
                    auto itFinal = std::min_element(vLens.begin(), vLens.end());
                    finalLen = *itFinal;
                }
                unsigned newCol = m_col + finalLen;
                setLocation(m_row, newCol, _board);
            }
            break;
        default:
            break;
    }
}

void shape::drawShape(QPainter& _painter) const
{
    pen.setWidth(1);  // apply to all cells
    for(unsigned i = 0; i < 4; i++)
        m_cells[i].drawCell(_painter);
//    // FOR TEST, TO BE DELETED --- draw cell index
//    QFont font = _painter.font();
//    auto size = font.pointSize();
//    font.setPointSize(size * 1);
//    _painter.setFont(font);
//    for(unsigned i = 0; i < 4; i++)
//    {
//        QPoint point(m_cells[i].getCellPosition().first + CELL_LENGTH / 3, m_cells[i].getCellPosition().second + CELL_LENGTH * 1);
//        _painter.drawText(point, QString::number(i));
//    }
//    font.setPointSize(size);
//    // TEST END
}

void shape::calPosition(board* _board)
{
    m_X = m_col * CELL_LENGTH + _board->getTopLeftX();
    m_Y = m_row * CELL_LENGTH + _board->getTopLeftY();
}

void shape::calFirstCellLocation(board* _board)
{
    m_row = (m_Y - _board->getTopLeftY()) / CELL_LENGTH;
    m_col = (m_X - _board->getTopLeftX()) / CELL_LENGTH;
}

void shape::setCellsColor(unsigned _color)
{
    for(unsigned i = 0; i < 4; i++)
        m_cells[i].setColor(_color);
}

void shape::initCells(unsigned _randValue, board* _board)
{
    // set visible state
    for(unsigned i = 0; i < 4; i++)
        m_cells[i].setVisibleState(true);
    // set color and shape
    switch(_randValue)
    {
        case 0:  // L
            {
                m_color = 0;
                m_shapeType = 0;
            }
            break;
        case 1:  // J
            {
                m_color = 1;
                m_shapeType = 1;
            }
            break;
        case 2:  // T
            {
                m_color = 2;
                m_shapeType = 2;
            }
            break;
        case 3:  // O
            {
                m_color = 3;
                m_shapeType = 3;
            }
            break;
        case 4:  // Z
            {
                m_color = 4;
                m_shapeType = 4;
            }
            break;
        case 5:  // S
            {
                m_color = 5;
                m_shapeType = 5;
            }
            break;
        case 6:  // I
            {
                m_color = 6;
                m_shapeType = 6;
            }
            break;
        default:
            break;
    }
    setCellsColor(m_color);
    // set locations
    initCellsLocations(_board);
}

void shape::setLocation(unsigned _row, unsigned _col, board* _board)
{
    int rowDifference = m_row - _row;
    int colDifference = m_col - _col;
    // move all cells in shape
    for(unsigned i = 0; i < 4; i++)
    {
        unsigned newRow = m_cells[i].getCellLocation().first - rowDifference;
        unsigned newCol = m_cells[i].getCellLocation().second - colDifference;
        m_cells[i].setLocation(newRow, newCol, _board->getTopLeftX(), _board->getTopLeftY());
    }
    m_row = _row;
    m_col = _col;
    calPosition(_board);
}

void shape::initCellsLocations(board* _board)
{
    unsigned _boardOriginX = _board->getTopLeftX();
    unsigned _boardOriginY = _board->getTopLeftY();
    // by default place shape at the top-middle of board
    m_row = 0;
    m_col = (_board->getWidth() / CELL_LENGTH) / 2;
    m_X = _boardOriginX + m_col * CELL_LENGTH;
    m_Y = _boardOriginY;
    switch(m_shapeType)
    {
        case 0:  // L
            {
                m_cells[0].setLocation(m_row, m_col, _boardOriginX, _boardOriginY);
                m_cells[1].setLocation(m_row + 1, m_col, _boardOriginX, _boardOriginY);
                m_cells[2].setLocation(m_row + 2, m_col, _boardOriginX, _boardOriginY);
                m_cells[3].setLocation(m_row + 2, m_col + 1, _boardOriginX, _boardOriginY);
            }
            break;
        case 1:  // J
            {
                m_cells[0].setLocation(m_row, m_col, _boardOriginX, _boardOriginY);
                m_cells[1].setLocation(m_row + 1, m_col, _boardOriginX, _boardOriginY);
                m_cells[2].setLocation(m_row + 2, m_col, _boardOriginX, _boardOriginY);
                m_cells[3].setLocation(m_row + 2, m_col - 1, _boardOriginX, _boardOriginY);
            }
            break;
        case 2:  // T
            {
                m_cells[0].setLocation(m_row, m_col, _boardOriginX, _boardOriginY);
                m_cells[1].setLocation(m_row + 1, m_col, _boardOriginX, _boardOriginY);
                m_cells[2].setLocation(m_row + 1, m_col - 1, _boardOriginX, _boardOriginY);
                m_cells[3].setLocation(m_row + 1, m_col + 1, _boardOriginX, _boardOriginY);
            }
            break;
        case 3:  // O
            {
                m_cells[0].setLocation(m_row, m_col, _boardOriginX, _boardOriginY);
                m_cells[1].setLocation(m_row, m_col + 1, _boardOriginX, _boardOriginY);
                m_cells[2].setLocation(m_row + 1, m_col + 1, _boardOriginX, _boardOriginY);
                m_cells[3].setLocation(m_row + 1, m_col, _boardOriginX, _boardOriginY);
            }
            break;
        case 4:  // Z
            {
                m_cells[0].setLocation(m_row, m_col, _boardOriginX, _boardOriginY);
                m_cells[1].setLocation(m_row, m_col + 1, _boardOriginX, _boardOriginY);
                m_cells[2].setLocation(m_row + 1, m_col + 1, _boardOriginX, _boardOriginY);
                m_cells[3].setLocation(m_row + 1, m_col + 2, _boardOriginX, _boardOriginY);
            }
            break;
        case 5:  // S
            {
                m_cells[0].setLocation(m_row, m_col, _boardOriginX, _boardOriginY);
                m_cells[1].setLocation(m_row, m_col - 1, _boardOriginX, _boardOriginY);
                m_cells[2].setLocation(m_row + 1, m_col - 1, _boardOriginX, _boardOriginY);
                m_cells[3].setLocation(m_row + 1, m_col - 2, _boardOriginX, _boardOriginY);
            }
            break;
        case 6:  // I
            {
                m_cells[0].setLocation(m_row, m_col, _boardOriginX, _boardOriginY);
                m_cells[1].setLocation(m_row + 1, m_col, _boardOriginX, _boardOriginY);
                m_cells[2].setLocation(m_row + 2, m_col, _boardOriginX, _boardOriginY);
                m_cells[3].setLocation(m_row + 3, m_col, _boardOriginX, _boardOriginY);
            }
            break;
        default:
            break;
    }
}

void shape::borderCheck(unsigned& aimRow, unsigned& aimCol, board* _board, myVec& vOrigLoc, cell& cell)
{
    unsigned boardOriginX = _board->getTopLeftX();
    unsigned boardOriginY = _board->getTopLeftY();
    if(aimCol < 0 || aimCol > _board->getNumOfCols() - 1 || aimRow < 0 || aimRow > _board->getNumOfRows() - 1)
    {
        for(unsigned j = 0; j < 4; j++)
            m_cells[j].setLocation(vOrigLoc[j].first, vOrigLoc[j].second, boardOriginX, boardOriginY);
        bNeedRestoreLocation = true;
        return;
    }
    cell.setLocation(aimRow, aimCol, boardOriginX, boardOriginY);
}

#include "gameEngine.h"

#include <config.h>

#include <QtGlobal>  // qsrand(),qrand()
#include <ctime>     // time()

gameEngine::gameEngine()
{
    qsrand(time(NULL));  // start send seed
    /* 2 boards, has to be initialized before shapes,as will be used in shapes */
    m_mainBoard = new board(MAIN_BOARD_X, MAIN_BOARD_Y, MAIN_BOARD_WIDTH, MAIN_BOARD_HEIGHT);
    m_nextBoard = new board(NEXT_BOARD_X, NEXT_BOARD_Y, NEXT_BOARD_WIDTH, NEXT_BOARD_HEIGHT);
    /* 3 shapes */
    // generate 2 random shapes
    unsigned randValue = rand() % 7;
    m_curShape = new shape(randValue, m_mainBoard);
    m_curShape->setLocation(0, (MAIN_BOARD_WIDTH / CELL_LENGTH) / 2,
                            m_mainBoard);  // place at top-middle
    randValue = rand() % 7;
    m_nextShape = new shape(randValue, m_nextBoard);
    m_nextShape->setLocation(m_nextBoard->getNumOfRows() / 2, m_nextBoard->getNumOfCols() / 2, m_nextBoard);
    m_score = 0;
    m_moveSpeed = CUR_SHAPE_MOVING_SPEED_SLOW;
    m_bOkToMoveDown = true;
    m_bGameSuspend = false;
    m_bFirstTimeClickBtnPlay = true;
}

gameEngine::~gameEngine()
{
    delete m_mainBoard;
    delete m_nextBoard;
    if(m_curShape != nullptr)
        delete m_curShape;
    if(m_nextShape != nullptr)
        delete m_nextShape;
}

void gameEngine::keyUp_rotate()
{
    /* ---------------------------------------------------
     * counter-clockwise;
     * no reponse to long press;
     * calculate 4 cells every time rotate() is involked,
     * --------------------------------------------------*/
    switch(m_curShape->getShapeType())
    {
        case 0:  // L
            m_curShape->rotateShape(2, m_mainBoard);
            break;
        case 1:  // J
            m_curShape->rotateShape(2, m_mainBoard);
            break;
        case 2:  // T
            m_curShape->rotateShape(1, m_mainBoard);
            break;
        //        case 3:  // O, no rotate for shape 'O'
        //            break;
        case 4:  // Z
            m_curShape->rotateShape(2, m_mainBoard);
            break;
        case 5:  // S
            m_curShape->rotateShape(2, m_mainBoard);
            break;
        case 6:  // I
            m_curShape->rotateShape(3, m_mainBoard);
            break;
        default:
            break;
    }
}

void gameEngine::keyDown_moveDown()
{
    //    m_curShape->autoMoveDown(m_moveSpeed, m_mainBoard); // abandon this funcion due to no logic judge when to stop
    //    and no moving distance regulation
    curShapeAutoMoveDown();
}

void gameEngine::keyLeft_moveLeft()
{
    m_curShape->moveLeft(m_moveSpeed, m_mainBoard);
}

void gameEngine::keyRight_moveRight()
{
    m_curShape->moveRight(m_moveSpeed, m_mainBoard);
}

void gameEngine::initGame()
{
    m_score = 0;
    m_mainBoard->clearBoard();
    m_nextBoard->clearBoard();
    m_curShape->setLocation(0, (MAIN_BOARD_WIDTH / CELL_LENGTH) / 2,
                            m_mainBoard);  // place at top-middle
    // change type and color,then place at m_nextBoard center
    unsigned randValue = rand() % 7;
    m_nextShape->initCells(randValue, m_mainBoard);
    m_nextShape->setLocation(m_nextBoard->getNumOfRows() / 2, m_nextBoard->getNumOfCols() / 2, m_nextBoard);
    //    m_nextShape->setLocation((NEXT_BOARD_HEIGHT / CELL_LENGTH) / 2, (NEXT_BOARD_WIDTH / CELL_LENGTH) / 2,
    //    m_nextBoard);
}

bool gameEngine::isGameSuspend()
{
    return m_bGameSuspend;
}

int gameEngine::getScore() const
{
    return m_score;
}

void gameEngine::drawGame(QPainter& _painter) const
{
    m_mainBoard->drawBoard(_painter);
    m_nextBoard->drawBoard(_painter);
    m_curShape->drawShape(_painter);
    m_nextShape->drawShape(_painter);
}

bool gameEngine::isReachedBottom() const
{
    if(m_curShape->getLowestCellRow() == m_mainBoard->getNumOfRows() - 1)
        return true;
    else
        return false;
}

bool gameEngine::isReachedVisibleCell() const
{
    myVec& vec = m_curShape->getCellsLocation();
    for(unsigned i = 0; i < 4; i++)
    {
        unsigned rowOfCellBelow = vec[i].first + 1;
        if(rowOfCellBelow > m_mainBoard->getNumOfRows() - 1)
            return false;
        if(m_mainBoard->isCellVisible(rowOfCellBelow, vec[i].second))
            return true;
    }
    return false;
}

unsigned gameEngine::eliminateFullRows()
{
    // only eliminate rows when shape stopped
    if(m_bOkToMoveDown)
        return 0;
    return m_mainBoard->eliminateFullRows();
}

void gameEngine::putShapeOnTwoBoards()
{
    // if still moving down,can't place shape
    if(m_bOkToMoveDown)
        return;
    // change m_curShape's location to m_mainBoard's middle-top,copy type and color from m_nextShape
    m_curShape->initCells(m_nextShape->getShapeType(), m_mainBoard);  // contains set: place at board'stop-middle
    // change m_nextShape's color and type
    unsigned randValue = rand() % 7;
    m_nextShape->initCells(randValue, m_nextBoard);  // by default put to top-middle
    m_nextShape->setLocation(m_nextBoard->getNumOfRows() / 2, m_nextBoard->getNumOfCols() / 2,
                             m_nextBoard);  // change to board center
    // after put shape on m_mainBoard, change the state to true so that shape can move down
    m_bOkToMoveDown = true;
}

void gameEngine::curShapeAutoMoveDown()
{
    /* -------------------------------------------------------
     * Two situations will stop move down:
     * S1: any one of shape's cells reach the bottom
     * S2: any one of shape's cells reach the mainBoard shadow
     * -------------------------------------------------------*/
    /* S1 || S2 */
    if(isReachedBottom() || isReachedVisibleCell())
    {
        m_bOkToMoveDown = false;
        return;
    }
    // move shape
    m_curShape->autoMoveDown(m_moveSpeed, m_mainBoard);
    // check S1 and S2 again
    if(isReachedBottom() || isReachedVisibleCell())
        m_bOkToMoveDown = false;
}

void gameEngine::copyShapeColorToMainboard()
{
    // if still moving, don't leave shadow
    if(m_bOkToMoveDown)
        return;
    myVec& vec = m_curShape->getCellsLocation();
    for(unsigned i = 0; i < 4; i++)
        m_mainBoard->setCellWithColor(vec[i].first, vec[i].second, m_curShape->getColor());
}

bool gameEngine::isGameOver()
{
    // get m_curShape's all cells location
    myVec& vecShape = m_curShape->getCellsLocation();
    // check if any location is occupied by visible cell on m_mainBoard
    for(unsigned i = 0; i < 4; i++)
    {
        bool isCurCellVisible = m_mainBoard->isCellVisible(vecShape[i].first, vecShape[i].second);
        if(isCurCellVisible)
        {
            // action taken to notify player
            suspend();
            return true;
        }
    }
    return false;
}

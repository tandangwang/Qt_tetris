#include "mainwindow.h"

#include <QMessageBox>

#include "config.h"
#include "ui_mainwindow.h"

QPen pen;
QBrush brush;

MainWindow::MainWindow(QWidget* parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    m_gameEngine = new gameEngine;
    m_timer = new QTimer(this);
    m_interval_fast = TIMER_INTERVAL_FAST;
    m_interval_slow = TIMER_INTERVAL_SLOW;
    m_timer->setInterval(m_interval_slow);
    m_elapsedTimer = new QElapsedTimer;
    m_elapsedTimer->start();
    m_bKeyLongPressed = false;
    /* all connections with slots */
    connect(m_timer, SIGNAL(timeout()), this, SLOT(timerMission()));
    connect(ui->btn_play, SIGNAL(clicked(bool)), this, SLOT(play()));
    connect(ui->btn_suspend, SIGNAL(clicked(bool)), this, SLOT(suspend()));
    connect(ui->btn_replay, SIGNAL(clicked(bool)), this, SLOT(replay()));
    connect(m_gameEngine, SIGNAL(scoreValueChanged(unsigned)), this, SLOT(displayScore()));
    setFocusPolicy(Qt::StrongFocus);
}

MainWindow::~MainWindow()
{
    delete ui;
    delete m_timer;
    delete m_gameEngine;
    delete m_elapsedTimer;
}

void MainWindow::keyReleaseEvent(QKeyEvent* event)
{
    // when release key, it won't be long press anymore
    m_bKeyLongPressed = false;
    m_gameEngine->setSpeedToSlow();
}

void MainWindow::keyPressEvent(QKeyEvent* event)
{
    m_elapsedTimer->restart();
    switch(event->key())
    {
        case Qt::Key_Up:
            m_gameEngine->keyUp_rotate();
            break;
        case Qt::Key_Down:
            //            m_gameEngine->keyDown_moveDown();
            //            m_timer->setInterval(m_interval_fast);
            m_gameEngine->setSpeedToFast();
            break;
        case Qt::Key_Left:
            m_gameEngine->keyLeft_moveLeft();
            break;
        case Qt::Key_Right:
            m_gameEngine->keyRight_moveRight();
            break;
        default:
            break;
    }
}

void MainWindow::paintEvent(QPaintEvent* event)
{
    QPainter painter(this);
    pen.setColor(Qt::black);
    pen.setStyle(Qt::SolidLine);
    pen.setCapStyle(Qt::RoundCap);
    pen.setJoinStyle(Qt::RoundJoin);
    m_gameEngine->drawGame(painter);
}

void MainWindow::timerMission()
{
    /* [Part1] response to button 'Suspend' */
    if(m_gameEngine->isGameSuspend())
        return;
    /* [Part2] m_gameEngine routine */
    m_gameEngine->putShapeOnTwoBoards();
    update();
    if(m_gameEngine->isGameOver())   // check right after new shape try to appear
    {
        QMessageBox::information(this, tr("Attention"), tr("Game over!"));
        // only button 'Replay' is supposed to be available
        ui->btn_play->setEnabled(false);
        ui->btn_suspend->setEnabled(false);
    }
    m_gameEngine->curShapeAutoMoveDown();
    update();
    m_gameEngine->copyShapeColorToMainboard();
    update();
    unsigned countRows = m_gameEngine->eliminateFullRows();
    m_gameEngine->updateScore(countRows);
    /*----------- m_gameEngine gameRoutine ------------------------*/
}

void MainWindow::play()
{
    if(m_gameEngine->isBtnPlayFirstTimeClicked())
    {
        m_gameEngine->setFirstTimeClickBtnPlay(false);  // set to 'not the first click'
        m_timer->start();
        // change button 'Play' to grey
        ui->btn_play->setEnabled(false);
    }
    else
    {
        // unsuspend game
        m_gameEngine->unSuspend();
        ui->btn_play->setEnabled(false);
        ui->btn_suspend->setEnabled(true);  // by far, btn_suspend must have been clicked
    }
}

void MainWindow::suspend()
{
    m_gameEngine->suspend();
    // after btn_suspend is clicked, it shall become grey
    ui->btn_suspend->setEnabled(false);
    // enable btn_play, so player can click it to resume game
    ui->btn_play->setEnabled(true);
    ui->btn_play->setText("Continue");
}

void MainWindow::replay()
{
    m_timer->stop();
    m_gameEngine->initGame();
    m_timer->start();
    // change button 'Play' to grey
    ui->btn_play->setEnabled(false);
    ui->btn_play->setText("Play");
    // evenif game is suspended, still allow replay. here need to unsuspend to eable timerMission()
    m_gameEngine->unSuspend();
    // change button 'Suspend' to available, so that even if game is suspended, still can replay
    if(!ui->btn_suspend->isEnabled())
        ui->btn_suspend->setEnabled(true);
}

void MainWindow::displayScore()
{
    ui->lcdScore->display(m_gameEngine->getScore());
}

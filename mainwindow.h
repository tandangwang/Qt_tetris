#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QElapsedTimer>
#include <QKeyEvent>
#include <QLCDNumber>
#include <QMainWindow>
#include <QPaintEvent>
#include <QTimer>
#include <QPainter>

#include "gameEngine.h"

QT_BEGIN_NAMESPACE
namespace Ui
{
    class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget* parent = nullptr);
    ~MainWindow();

    /* <1> moniter key events */
    void keyReleaseEvent(QKeyEvent* event) override;
    void keyPressEvent(QKeyEvent* event) override;

    /* <2> draw events */
    void paintEvent(QPaintEvent* event) override;

private:
    Ui::MainWindow* ui;
    gameEngine* m_gameEngine;
    QTimer* m_timer;

    QElapsedTimer* m_elapsedTimer;
    bool m_bKeyLongPressed;
    unsigned m_interval_fast;
    unsigned m_interval_slow;

public slots:
    void timerMission();
    void play();
    void suspend();
    void replay();
    void displayScore();
};
#endif  // MAINWINDOW_H

#include <QApplication>
#include <QFont>
#include <QGridLayout>
#include <QHBoxLayout>
#include <QLCDNumber>
#include <QLabel>
#include <QPushButton>
#include <QShortcut>
#include <QVBoxLayout>
#include <QWidget>
#include <iostream>
#include <QKeyEvent>

#include "pacmanarena.h"
#include "gameboard.h"

GameBoard::GameBoard(QWidget *parent)
    : QWidget(parent)
{
    // quit button
    QPushButton *quit = new QPushButton(tr("&Quit"));
    quit->setFont(QFont("Times", 18, QFont::Bold));
    quit->setFocusPolicy(Qt::NoFocus);

    connect(quit, SIGNAL(clicked()), qApp, SLOT(quit()));

    QFrame *pacmanBox = new QFrame;
    pacmanBox->setFrameStyle(QFrame::WinPanel | QFrame::Sunken);

    //arena itself
    pacmanArena = new PacmanArena;
    pacmanArena->setFocus();
    connect(pacmanArena, SIGNAL(foodEaten(int)),
            this, SLOT(increasePoints(int)));

    //restart button
    QPushButton *restart = new QPushButton(tr("&New Game"));
    restart->setFocusPolicy(Qt::NoFocus);
    connect(restart, SIGNAL(clicked()), this, SLOT(newGame()));

    //show score
    points = new QLCDNumber(3);
    points->setSegmentStyle(QLCDNumber::Filled);

    (void) new QShortcut(Qt::CTRL + Qt::Key_Q, this, SLOT(close()));

    // showing top widgets
    QHBoxLayout *topLayout = new QHBoxLayout;
    topLayout->addWidget(quit);
    topLayout->addWidget(points);
    topLayout->addWidget(restart);

    //pacman box
    QVBoxLayout *cannonLayout = new QVBoxLayout;
    cannonLayout->addWidget(pacmanArena);
    pacmanBox->setLayout(cannonLayout);

    //pacman arena
    QGridLayout *gridLayout = new QGridLayout;
    gridLayout->addLayout(topLayout, 0, 0);
    gridLayout->addWidget(pacmanBox, 1, 0);
    gridLayout->setRowStretch(1, 0);
    setLayout(gridLayout);

    
    newGame();
}


void GameBoard::keyPressEvent(QKeyEvent *event)
{
    QApplication::sendEvent(pacmanArena, event);
}


void GameBoard::increasePoints(int howMany)
{
    points->display(points->value() + howMany);
}


void GameBoard::newGame()
{
    points->display(0);
    pacmanArena->restartGame(); //load map and restart game
    this->setFocus();
}
#include <QApplication>
#include <QPainter>
#include <iostream>
#include <vector>
#include <QGraphicsView>
#include <QtCore>

#include "pacmanarena.h"
#include "player.h"
#include "ghost.h"

PacmanArena::PacmanArena(QWidget *parent)
    : QWidget(parent)
{
    setPalette(QPalette(QColor(0, 0, 55)));
    setAutoFillBackground(true);
    player = new Player(this);
    player->setStartCoordinates();
    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(tick()));

    frameDelay = 150;

    connect(player, SIGNAL(foodEaten(int)),
            this, SLOT(transferPoint(int)));

    ghost = new Ghost(this, player);

    connect(player, SIGNAL(superFoodEaten()),
            ghost, SLOT(superFoodEaten()));
}

void PacmanArena::paintEvent(QPaintEvent * /* event */)
{
    QPainter painter(this);

    if (state == GAME){
        drawBoard(painter);
        drawFood(painter);
        player->drawPlayer(painter);
        ghost->drawGhost(painter);
    }
    else if (state == GAME_OVER){
        painter.drawText(100, 100, "Game over!");
    }
    else if (state == YOU_WON){
        painter.drawText(100, 100, "YOU WON!");
    }
}

void PacmanArena::keyPressEvent(QKeyEvent *event)
{
    QApplication::sendEvent(player, event);
}

void PacmanArena::drawBoard(QPainter &painter){
  
    painter.setPen(QColor(qRgb(0, 0, 55)));
    painter.setBrush(QColor(qRgb(4, 35, 160)));

    for (int i = 0; i < 31; ++i){
        for (int j = 0; j < 28; ++j){
            if (gameMap[i][j] == 1)
                painter.drawRect(j / 28.0 * rect().width(), i / 31.0 * rect().height(), rect().width() / 28.0, rect().height() / 31.0);
        }
    }


    painter.setBrush(QColor(qRgb(117, 4, 201)));

    //draw door
    for (int i = 0; i < 31; ++i){
        for (int j = 0; j < 28; ++j){
            if (gameMap[i][j] == 3)
                painter.drawRect(j / 28.0 * rect().width(), i / 31.0 * rect().height(), rect().width() / 28.0, rect().height() / 31.0);
        }
    }
}

void PacmanArena::drawFood(QPainter &painter){
    
    painter.setPen(Qt::NoPen);
    painter.setBrush(QColor(qRgb(244, 244, 66)));

    for (int i = 0; i < 31; ++i){
        for (int j = 0; j < 28; ++j){
            if (gameMap[i][j] == 2){ //draw regular food
                painter.setBrush(QColor(qRgb(244, 244, 66)));
                painter.drawEllipse(j * 1.0 / gameColumns * rect().width() + 1.0 * rect().width() / gameColumns / 3, i * 1.0 / gameRows * rect().height() + 1.0 * rect().height() / gameRows / 3, 1.0 * rect().width() / gameColumns / 3, 1.0 * rect().height() / gameRows / 3);
            } 
            else if(gameMap[i][j] == 4){ //draw superfood
                painter.setBrush(QColor(qRgb(48, 255, 103)));
                painter.drawEllipse(j * 1.0 / gameColumns * rect().width() + 1.0 * rect().width() / gameColumns / 4, i * 1.0 / gameRows * rect().height() + 1.0 * rect().height() / gameRows / 4, 1.0 * rect().width() / gameColumns / 2, 1.0 * rect().height() / gameRows / 2);
            }
        }
    }
}

void PacmanArena::tick(){
    player->tick();
    checkCollision();
    ghost->tick();
    checkCollision();
    update();

    if (!isFoodLeft())
        youWon();
}

void PacmanArena::transferPoint(int howMany){
    emit foodEaten(howMany);
}

void PacmanArena::checkCollision(){
    // check ghost - player collisions
    if((player->getColumn() == ghost->getColumn()) && (player->getRow() == ghost->getRow())){
        if (!ghost->canBeEaten) {
            timer->stop();
            state = GAME_OVER;
        }
        else {
            ghost->setStartCoordinates();
            ghost->canBeEaten = false;
            ghost->atSpawn = true;
            emit(foodEaten(100));
        }
    }
}

void PacmanArena::restartGame(){
    //restart to copy map and start game
    std::copy(&gameStartMap[0][0], &gameStartMap[0][0]+gameRows*gameColumns,&gameMap[0][0]);
    player->setStartCoordinates();
    player->setPlayerDir(0);
    ghost->setStartCoordinates();
    ghost->atSpawn = true;
    timer->start(frameDelay);
    state = GAME;
    update();
}

void PacmanArena::youWon(){
    timer->stop();
    state = YOU_WON;
}

bool PacmanArena::isFoodLeft(){
    for (int i = 0; i < gameRows; ++i){
        for (int j = 0; j < gameColumns; ++j){
            if (gameMap[i][j] == 2)
                return true;
        }
    }
    return false;
}
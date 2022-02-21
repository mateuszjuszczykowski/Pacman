#ifndef GHOST_H
#define GHOST_H

#include <QWidget>
#include <QPainter>

#include "pacmanarena.h"
#include "player.h"

class Ghost : public QWidget
{
    Q_OBJECT
public:
    Ghost(PacmanArena *a, Player *p, QWidget *parent = 0);
    PacmanArena *arena;
    Player *player;
    void drawGhost(QPainter &painter);
    void setStartCoordinates();
    void tick();
    int getRow();
    int getColumn();
    bool atSpawn;
    bool canBeEaten;
private slots:
    void superFoodEaten();
private:
    int currentRow, currentColumn;
    int ghostDir;
    bool movementMade;
    void move();
    int canBeEatenTimer;
    int canBeEatenTimerMax;
    };


#endif
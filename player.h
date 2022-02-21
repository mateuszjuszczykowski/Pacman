#ifndef PLAYER_H
#define PLAYER_H

#include <QWidget>
#include <QKeyEvent>

class PacmanArena;

enum Directions
{
    LEFT = 3,
    RIGHT = 1,
    UP = 2,
    DOWN = 4
};

class Player : public QWidget
{
    Q_OBJECT
public:
    Player(PacmanArena *a, QWidget *parent = 0);
    void tick();
    void drawPlayer(QPainter &painter);
    void setStartCoordinates();
    int getRow();
    int getColumn();
    void setPlayerDir(int);
    int getPlayerDir();
signals:
    void foodEaten(int);
    void superFoodEaten();
protected:
    void keyPressEvent(QKeyEvent *event);
private:
    PacmanArena *arena;
    int animationTimer;
    void move();
    int currentRow, currentColumn;
    int playerDir;
    int rotation;
    Directions dirs;
};


#endif
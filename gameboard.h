#ifndef GAMEBOARD_H
#define GAMEBOARD_H

#include <QWidget>

class QLCDNumber;
class PacmanArena;

class GameBoard : public QWidget
{
    Q_OBJECT

public:
    GameBoard(QWidget *parent = 0);
protected slots:
    void increasePoints(int);
    void newGame();
protected:
    void keyPressEvent(QKeyEvent *event);
private:
    QLCDNumber *points;
    PacmanArena *pacmanArena;
};

#endif

#include <iostream>

#include "ghost.h"
#include "gameboard.h"


Ghost::Ghost(PacmanArena *a, Player *p, QWidget *parent)
    :QWidget(parent)
{
    arena = a;
    player = p;
    canBeEatenTimerMax = 40;
    canBeEaten = false;
    setStartCoordinates();
    atSpawn = true;
    ghostDir = 0;
}


void Ghost::drawGhost(QPainter &painter)
{
    painter.setPen(Qt::NoPen);
    if(canBeEaten) //change color if eaten
    {
        painter.setBrush(QColor(qRgb(173, 204, 255)));
    }
    else
    {
        painter.setBrush(QColor(qRgb(252, 76, 12)));
    }

    painter.drawEllipse(currentColumn / 28.0 * arena->rect().width(), currentRow / 31.0 * arena->rect().height(), arena->rect().width() / 28.0,  arena->rect().height() / 31.0);

    painter.setBrush(QColor(qRgb(255, 76, 0)));

    if (canBeEaten) { //timer
        painter.drawRect(0, arena->rect().height() / arena->gameRows / 3, arena->rect().width() * canBeEatenTimer / canBeEatenTimerMax, arena->rect().height() / arena->gameRows / 3);
    }
}

void Ghost::setStartCoordinates(){
    currentRow = 14;
    currentColumn = 13;
}

void Ghost::tick()
{
    move();

    if (canBeEaten) {
        canBeEatenTimer--;
        if (canBeEatenTimer == 0)
            canBeEaten = false;
    }
}

void Ghost::move()
{
    if(atSpawn){
        currentRow -= 1;

        if (currentRow == 11)
            atSpawn = false;
    }
    else{
        movementMade = false;
        int newGhostDir;

        // make better movements - somehow done
        int rowDiff = player->getRow() - getRow();
        int colDiff = player->getColumn() - getColumn();

        if(abs(rowDiff) >= abs(colDiff)) {
            if(player->getRow()>getRow())
                newGhostDir = DOWN;
            else
                newGhostDir = UP;
        }
        else {
            if(player->getColumn() > getColumn())
                newGhostDir = RIGHT;
            else
                newGhostDir = LEFT;
        }



        do{

            switch (ghostDir) //minimizes trunback rate
            {
            case UP:
                if(newGhostDir == DOWN)
                    ghostDir = UP;
                else
                    ghostDir = newGhostDir;
                break;
            case DOWN:
                if(newGhostDir == UP)
                    ghostDir = DOWN;
                else
                    ghostDir = newGhostDir;
                break;
            case RIGHT:
                if(newGhostDir == LEFT)
                    ghostDir = RIGHT;
                else
                    ghostDir = newGhostDir;
                break;
            case LEFT:
                if(newGhostDir == RIGHT)
                    ghostDir = LEFT;
                else
                    ghostDir = newGhostDir;
                break;
            
            default:
                ghostDir = newGhostDir;
                break;
            }

            
            //ghostDir = rand() % 4 + 1;

            switch(ghostDir){
                case UP:
                    if(arena->gameMap[currentRow - 1][currentColumn] != 1){
                        currentRow -= 1;
                        movementMade = true;
                    }
                    break;
                case RIGHT:
                    if(currentColumn == arena->gameColumns - 1)
                        currentColumn = -1;
                    if(arena->gameMap[currentRow][currentColumn + 1] != 1){
                        currentColumn += 1;
                        movementMade = true;
                    }
                    break;
                case DOWN:
                    if((arena->gameMap[currentRow + 1][currentColumn] != 1) && (arena->gameMap[currentRow + 1][currentColumn] != 3)){
                        currentRow += 1;
                        movementMade = true;
                    }
                    break;
                case LEFT:
                    if(currentColumn == 0)
                        currentColumn = arena->gameColumns;
                    if(arena->gameMap[currentRow][currentColumn - 1] != 1){
                        currentColumn -= 1;
                        movementMade = true;
                    }
                    break;
            }
            
            newGhostDir = rand() % 4 + 1; //in case it cannot follow
        }while (!movementMade);
    }
}

int Ghost::getRow(){ return currentRow; }

int Ghost::getColumn(){ return currentColumn; }

void Ghost::superFoodEaten(){
    canBeEaten = true;
    canBeEatenTimer = canBeEatenTimerMax;
}
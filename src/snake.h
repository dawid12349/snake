#ifndef __SNAKE_H__
#define __SNAKE_H__

#include "winsys.h"
#include <stdlib.h>
#include <vector>
#include <string>
#include <cstring>
#include <stdlib.h>
#include <unistd.h>
#include <ctime>
#include <iostream>
#include "screen.h"
class CSnake:public CFramedWindow
{
    enum Mode { RUN = 0, PAUSE, DEATH};
    Mode gameMode;
    typedef CPoint Vector;
    unsigned long speed = 150000, score = 0;

    void accelerate(){
        if(score % 30 == 0 && score > 50000)
            speed -= 25000;
    }

    static Vector getDirection(const int& direction) {
        switch(direction){
            case KEY_UP:
                return {0,-1};
            case KEY_DOWN:
                return {0, 1};
            case KEY_LEFT:
                return {-1,0};
            case KEY_RIGHT:
                return {1, 0};
            default:
                return {0, 0};
        }
    }

private:
    std::vector<CPoint> body;   //snake segments
    Vector c_dir;               //current direction of head
    CPoint food;                //food pos
public:
   explicit CSnake(CRect r, char _c = ' ');
   void paint() override;
   bool handleEvent(int key) override;
   void init();
   void update(const int& key);
   void Move();
   void changeDirection(const int& key);
   void keepInBounds();
   bool checkLose();
   void generateFood();
   CPoint& Head(){return body[0];}
   void grow(){body.emplace_back(CPoint(0, 0));}
};

#endif

#include "snake.h"

CSnake::CSnake(CRect r, char _c /*=' '*/):
  CFramedWindow(r, _c){
    init();
}

void CSnake::paint() {
    CFramedWindow::paint();
    gotoyx(geom.topleft.y - 1, geom.topleft.x + (geom.size.x / 2) - 3);
    printl("SCORE : %d", score);
    if(gameMode != DEATH) {
        gotoyx(geom.topleft.y + food.y, geom.topleft.x + food.x);
        printl("%c", 'F');
        gotoyx(geom.topleft.y + Head().y, geom.topleft.x + Head().x);
        printl("%c", '@');
        for (unsigned int i = 1; i < body.size(); i++) {
            gotoyx(geom.topleft.y + body[i].y, geom.topleft.x + body[i].x);
            printl("%c", '*');
        }
    }
    if(gameMode == DEATH){
        gotoyx(geom.topleft.y + 2,  geom.topleft.x + 5);
        printl("GAME OVER!");
        gotoyx(geom.topleft.y + 3,  geom.topleft.x + 5);
        printl("Press r/R to restart the game!");
    }
    if(gameMode == PAUSE) {
        gotoyx(geom.topleft.y + 2, geom.topleft.x + 5);
        printl("P/p - start/pause");
        gotoyx(geom.topleft.y + 3, geom.topleft.x + 5);
        printl("R/r - restart game");
        gotoyx(geom.topleft.y + 4, geom.topleft.x + 5);
        printl("Arrows - change direction");
    }
}

bool CSnake::handleEvent(int key) {
    fflush(stdin);
    if((key == 'p' || key == 'P')){
        if(gameMode != DEATH){
            if(gameMode == PAUSE)
                gameMode = RUN;
            else if (gameMode == RUN)
                gameMode = PAUSE;
        }
        return true;
    }
    if(key == 'r' || key == 'R'){
        init();
        sleep(1);
        return true;
    }
    if(gameMode==RUN){
        update(key);
        if(gameMode == DEATH){
            paint();
            return false;
        }
        return true;
    }
    if(gameMode != RUN && CFramedWindow::handleEvent(key)){
        return true;
    }
    return false;
}

void CSnake::init() {
    fflush(stdin);
    body.clear();
    body.emplace_back(CPoint{geom.size.x / 2, geom.size.y / 2});
    body.emplace_back(CPoint{geom.size.x / 2, (geom.size.y / 2) + 1});
    body.emplace_back(CPoint{geom.size.x / 2, (geom.size.y / 2) + 2});
    c_dir = getDirection(KEY_UP);
    generateFood();
    gameMode = PAUSE;
    score = 0;
    speed = 150000;
}

void CSnake::update(const int& key){
    cin.clear();
    changeDirection(key);
    if(Head() == food) {
        generateFood();
        grow();
        score += 10;
        accelerate();
    }
    Move();
    keepInBounds();
    if(checkLose())
        gameMode = DEATH;
    usleep(speed);
}

void CSnake::Move() {
    for(unsigned int i = body.size() - 1; i > 0; i--)
        body[i].moveTo(body[i-1]);
    Head() += c_dir;
}


void CSnake::changeDirection(const int& key) {
    Vector v = getDirection(key);
    if(v == c_dir) return; //the same dir
    else if((key == KEY_UP && c_dir.y != 1) || //check opposite
    (key == KEY_DOWN && c_dir.y != -1) ||
    (key == KEY_LEFT && c_dir.x != 1) ||
    (key == KEY_RIGHT && c_dir.x != -1))
        c_dir = v;
}


void CSnake::keepInBounds() {
    if(c_dir.y == -1 &&  Head().y + geom.topleft.y  == geom.topleft.y )
        Head().y = geom.size.y - 2;
    if(c_dir.y == 1 && Head().y + geom.topleft.y == geom.topleft.y + geom.size.y - 1)
        Head().y = 1;
    if(c_dir.x == -1 && Head().x + geom.topleft.x  == geom.topleft.x )
        Head().x = geom.size.x - 2;
    if(c_dir.x == 1 && Head().x + geom.topleft.x == geom.topleft.x + geom.size.x - 1 )
        Head().x = 1;
}

bool CSnake::checkLose()  {
    for(unsigned int i = 2; i<body.size(); i++)
        if(Head() == body[i])
            return true;
    return false;
}

void CSnake::generateFood() {
    for(const CPoint& segment : body) {
        srand((unsigned) time(nullptr));
        food.x = rand() % (geom.size.x - 2) + 1;
        food.y = rand() % (geom.size.y - 2) + 1;
        if(food == segment) continue;
        break;
    }
}

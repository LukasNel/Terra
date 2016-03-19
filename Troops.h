#ifndef TROOPS_H_INCLUDED
#define TROOPS_H_INCLUDED
#include "Bases.h"
enum moveType{
    MoveTranslate
};
struct Move{
    int x;
    int y;
    moveType mType;
    Move(){
        x = 0;
        y = 0;
        mType = MoveTranslate;
    }
    Move(int fx,int fy,moveType fMType){
        mType = fMType;
        x = fx;
        y = fy;
    }
};
struct Troop{
    int x,y;
    int tx,ty;
    WORD color;
    WORD moveAreaColour;
    Move moveQueue[200];
    int moveAmount;
    int TravelDistanceX;
    int cTravelDistanceX;
    int TravelDistanceY;
    int cTravelDistanceY;

    Base* SpawnBase = NULL;
    int bx = -1,by = -1;

    char forbiddenChar[2];
    WORD forbiddenAttr[2];
    int numForbidden = 2;
    char graphics;
    bool activation;
    inline bool checkIfForbidden(char checkChar,WORD checkAttr);
    virtual void InitTroop();

    void Selected(CHAR_INFO* levelmap,int maxLevelX,int maxLevelY,int deltaX,int deltaY,int movePoints);
    void Translate(int dx,int dy,CHAR_INFO destinationSquare);
    void Activate();
    void Deactivate();
    void ShowMoveQueue(CHAR_INFO* levelmap,int maxLX,int maxLY,int fdeltaX,int fdeltaY);
    void ExecuteMoveQueue();
    Troop();
    Troop(int fx,int fy);
};
#endif // TROOPS_H_INCLUDED

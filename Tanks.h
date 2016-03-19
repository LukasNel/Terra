#ifndef TANKS_H_INCLUDED
#define TANKS_H_INCLUDED
#include "Troops.h"

struct Tank : public Troop{
    int movePointReducer = 2;
    void InitTank(){
        color =  FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_INTENSITY;
        graphics = 'T';
        moveAreaColour = BACKGROUND_RED;
        forbiddenChar[0] = 'S';
        forbiddenAttr[0] = FOREGROUND_BLUE;

        forbiddenChar[1] = graphics;
        forbiddenAttr[1] = color;

        TravelDistanceX = 10;
        cTravelDistanceX = TravelDistanceX;
        TravelDistanceY = 10;
        cTravelDistanceY = TravelDistanceY;
        movePointReducer = 2;
        moveAmount = 0;
        x = y = tx = ty = 0;
        activation = false;
    }
    Tank(){
        InitTank();
    }
    Tank(int fx,int fy){
            InitTank();
            x = fx;
            y = fy;
    }


};
#endif // TANKS_H_INCLUDED

#include "WorldBasicFunctions.h"
void Troop::InitTroop(){
        color =  FOREGROUND_RED | FOREGROUND_INTENSITY;
        graphics = 'N';
        forbiddenChar[0] = 'S';
        forbiddenAttr[0] = FOREGROUND_BLUE;

        forbiddenChar[1] = graphics;
        forbiddenAttr[1] = color;

        TravelDistanceX = 5;
        cTravelDistanceX = TravelDistanceX;
        TravelDistanceY = 5;
        cTravelDistanceY = TravelDistanceY;

        moveAmount = 0;
        x = y = tx = ty = 0;
        activation = false;
    }
  Troop::Troop(){
        InitTroop();
    }
    Troop::Troop(int fx,int fy){
        InitTroop();
        x = fx;
        tx = fx;
        ty = fy;
        y = fy;


    }
     inline bool Troop::checkIfForbidden(char checkChar,WORD checkAttr){
        for(int i = 0;i<numForbidden;i++)if(checkChar == forbiddenChar[i] && checkAttr == forbiddenAttr[i])return 1;
        return 0;
     }
    void Troop::Translate(int dx,int dy,CHAR_INFO destinationSquare){
        if(checkIfForbidden(destinationSquare.Char.AsciiChar,destinationSquare.Attributes))return;
        moveQueue[moveAmount++] = Move(dx,dy,MoveTranslate);
    }
    void Troop::ExecuteMoveQueue(){
        for(int i = 0;i<moveAmount;i++){
            switch(moveQueue[i].mType){
                case MoveTranslate:
                    x += moveQueue[i].x;
                    y += moveQueue[i].y;
                break;

            }
        }
        moveAmount = 0;

    }
    void Troop::Selected(CHAR_INFO* levelmap,int maxLevelX,int maxLevelY,int deltaX,int deltaY,int movePoints){
        bool g_bool = false;
        int screenTroopX = 0;
        int screenTroopY = 0;
        for(int i = -TravelDistanceX;i <= TravelDistanceX;i++){
            for(int j = -TravelDistanceY; j <= TravelDistanceY; j++){
                    g_bool = false;
                    screenTroopX = tx-deltaX + i;
                    screenTroopY = ty-deltaY + j;

                    if(!OutOfBounds(screenTroopX,screenTroopY,maxLevelX,maxLevelY) && (abs(i)+abs(j)) <= movePoints){

                       if(i != 0 || j != 0){
                        if(checkIfForbidden(levelmap[screenTroopX + screenTroopY*maxLevelX].Char.AsciiChar,
                                            levelmap[screenTroopX + screenTroopY*maxLevelX].Attributes))
                                                                g_bool = true;
                       }
                        if(!g_bool) levelmap[screenTroopX  +  screenTroopY*maxLevelX].Attributes |= BACKGROUND_GREEN;
                    }
            }
        }



    }
    void Troop::ShowMoveQueue(CHAR_INFO* levelmap,int maxLX,int maxLY,int fdeltaX,int fdeltaY){
        tx = x;
        ty = y;
        for(int i = 0;i<moveAmount;i++){
            switch(moveQueue[i].mType){
                case MoveTranslate:
                    tx += moveQueue[i].x;
                    ty += moveQueue[i].y;
                   if(!OutOfBounds(tx-fdeltaX,ty-fdeltaY,maxLX,maxLY))
                    WriteCharA(levelmap,'X',FOREGROUND_RED | FOREGROUND_BLUE,tx-fdeltaX,ty-fdeltaY,maxLX);
                break;

            }
        }


    }
    void Troop::Activate(){

             color = color | BACKGROUND_RED ;
             activation = true;
    }
    void Troop::Deactivate(){
            color = color & ~BACKGROUND_RED;
            activation = false;
    }




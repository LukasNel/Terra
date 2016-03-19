#include "WorldBasicFunctions.h"
  Troop::Troop(){
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
    Troop::Troop(int fx,int fy){
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
        activation = false;
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


    void Troop::BuildBase(Base* bases,int& numBases,int& gOil,int& gIron,int& gSulphur,CHAR_INFO* worldmap,int maxX,int maxY){
       bool testRangeBool = false;

        for(int i = 0;i<numBases;i++)if(x - bases[i].x <= bases[i].resourceSearchSideLength &&
                                        y - bases[i].y <= bases[i].resourceSearchSideLength)testRangeBool = true;
        if(!testRangeBool){
            bases[numBases] = Base(x,y);
            bases[numBases].CountResources(worldmap,maxX,maxY);

            gOil += bases[numBases].oil;
            gSulphur += bases[numBases].sulphur;
            gIron += bases[numBases].iron;
            numBases++;
        }
    }

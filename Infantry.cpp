#include "Infantry.h"
void Infantry::InitTroop(){
        color =  FOREGROUND_RED | FOREGROUND_INTENSITY;
        graphics = 'N';
        moveAreaColour = BACKGROUND_GREEN;
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
void Infantry::BuildBase(Base* bases,int& numBases,int& gOil,int& gIron,int& gSulphur,CHAR_INFO* worldmap,int maxX,int maxY){
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

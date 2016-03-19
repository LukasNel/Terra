#include "WorldBasicFunctions.h"
Base::Base(){
       InitBase();

}
void Base::InitBase(){
    numBuildOrders = 0;
        buildQueue = NULL;
     oil = 0;
     oilGraphics = 'O';
     oilColour = BACKGROUND_INTENSITY;

     sulphur = 0;
     sulphurGraphics = 'S';
     sulphurColour = FOREGROUND_RED  | FOREGROUND_GREEN | FOREGROUND_INTENSITY;

     iron = 0;
     ironGraphics = 'I';
     ironColour = FOREGROUND_INTENSITY;

     infantryOilCost = 0;
     infantrySulphurCost = 0;
     infantryIronCost = 0;
     infantryUnitRepairGraphics = '+';
     infantryUnitRepairColour = BACKGROUND_RED;
     x = -1,y = -1;
     resourceSearchSideLength = 10;
     troopBuildingSideLength = 1;
     colour = BACKGROUND_BLUE | BACKGROUND_GREEN ;
     graphics = 'B';
     amountUnits = 0;
     maxBuildOrders = (troopBuildingSideLength+2)*(troopBuildingSideLength+2);
    buildQueue = new BuildOrder[maxBuildOrders];
    numForbiddens = 2;
    forbiddens = new CHAR_INFO[numForbiddens];
    forbiddens[0].Char.AsciiChar = seachar;
    forbiddens[0].Attributes = seaattr;
    forbiddens[1].Char.AsciiChar = Troop().graphics;
    forbiddens[1].Attributes = Troop().color;
     selected = false;

}
Base::Base(int fx,int fy,int fresourcesideLength,int funitSidelength){
    InitBase();

    if(buildQueue)delete[] buildQueue;

    x = fx;
    y = fy;

    resourceSearchSideLength = fresourcesideLength;
    troopBuildingSideLength = funitSidelength;

     maxBuildOrders = (troopBuildingSideLength+2)*(troopBuildingSideLength+2);
    buildQueue = new BuildOrder[maxBuildOrders];
}
Base::Base(int fx,int fy){
    InitBase();
    x = fx;
    y = fy;
}
void Base::CountResources(CHAR_INFO* worldmap,int maxMapX,int maxMapY){

    for(int i = -resourceSearchSideLength;i<=resourceSearchSideLength;i++){
        for(int j = -resourceSearchSideLength;j<=resourceSearchSideLength;j++){
            int index = x + i + (y + j)*maxMapX;
            if(OutOfBounds(x+i,y+j,maxMapX,maxMapY))continue;

            if(worldmap[index].Char.AsciiChar == oilGraphics && worldmap[index].Attributes == oilColour){
                oil++;
            }
            if(worldmap[index].Char.AsciiChar == sulphurGraphics && worldmap[index].Attributes == sulphurColour){
                sulphur++;
            }
            if(worldmap[index].Char.AsciiChar == ironGraphics && worldmap[index].Attributes == ironColour){
                iron++;
            }

        }
    }
    oldiron = iron;
    oldoil = oil;
    oldsulphur = sulphur;

}
void Base::AddTroopToBuildQueue(int dx,int dy){
    if(numBuildOrders >= maxBuildOrders)return;
    for(int i = 0;i<numBuildOrders;i++)if(buildQueue[i].x == dx && buildQueue[i].y == dy) return;
    buildQueue[numBuildOrders] = BuildOrder(dx,dy,TroopOrder);
    numBuildOrders++;
}
void Base::ExecuteBuildQueue(Troop* troops,int& numTroops,int maxMapX,int maxMapY,char seaChar,WORD seaAttr,CHAR_INFO* worldmap){
    for(int i = 0;i < numBuildOrders;i++){
        switch(buildQueue[i].orderType){
            case TroopOrder:
                CreateInfantry(buildQueue[i].x,buildQueue[i].y,troops,numTroops,maxMapX,maxMapY,worldmap,seaChar,seaAttr);
            break;
            default:break;

        }

    }
    numBuildOrders = 0;
}
void Base::CreateInfantry(int dx,int dy,Troop* troops,int& numTroops,int maxMapX,int maxMapY,CHAR_INFO* worldmap,char seaChar,WORD seaAttr){
    if(abs(dx) > troopBuildingSideLength || abs(dy) > troopBuildingSideLength)return;

    for(int i = 0;i<amountUnits;i++){
        if(baseUnits[i]->bx == x+dx && baseUnits[i]->by == y+dy)return;
    }
    if(dx == 0 && dy == 0)return;
    if(OutOfBounds(x+dx,y+dy,maxMapX,maxMapY))return;
    for(int i = 0;i<numForbiddens;i++)if(compareCharWorldmap(worldmap,x+dx,x+dy,maxMapX,forbiddens[i].Char.AsciiChar,forbiddens[i].Attributes))return;
    if(oil - infantryOilCost >= 0){
        if(sulphur - infantrySulphurCost >= 0){
            if(iron - infantryIronCost >= 0){
                oil -= infantryOilCost;
                sulphur -= infantrySulphurCost;
                iron -= infantryIronCost;

                troops[numTroops] = Troop(x+dx,y+dy);
                troops[numTroops].bx = x+dx;
                troops[numTroops].by = y+dy;
                troops[numTroops].SpawnBase = this;
                baseUnits[amountUnits++] = &troops[numTroops];
                numTroops++;
            }
        }
    }

}
void Base::Activate(){
    colour |= BACKGROUND_INTENSITY;

}
void Base::Deactivate(){
    colour &= ~BACKGROUND_INTENSITY;
}
void Base::Selected(CHAR_INFO* levelmap,int maxLX,int maxLY,int deltaX,int deltaY){
    for(int i = -resourceSearchSideLength;i<=resourceSearchSideLength;i++){
        for(int j = -resourceSearchSideLength;j<=resourceSearchSideLength;j++){
            int index = x + i - deltaX + (y + j-deltaY)*maxLX;
            if(!OutOfBounds(x+i-deltaX,y+j-deltaY,maxLX,maxLY))
            levelmap[index].Attributes |= BACKGROUND_GREEN;
        }
    }
    for(int i = -troopBuildingSideLength;i<=troopBuildingSideLength;i++){
        for(int j = -troopBuildingSideLength;j<=troopBuildingSideLength;j++){
            int index =  x + i - deltaX + (y + j-deltaY)*maxLX;
            if(OutOfBounds(x+i-deltaX,y+j-deltaY,maxLX,maxLY))continue;
            levelmap[index].Attributes &= ~BACKGROUND_GREEN;
            levelmap[index].Attributes |= BACKGROUND_RED;

        }
    }
     for(int i = 0;i<amountUnits;i++){
            int index = baseUnits[i]->bx - deltaX +  (baseUnits[i]->by - deltaY)*maxLX;

            levelmap[index].Attributes &= ~BACKGROUND_GREEN;
            levelmap[index].Attributes &= ~BACKGROUND_RED;

     }
            WriteNumA(levelmap,oldiron,FOREGROUND_INTENSITY,0,7,maxLX);
            WriteNumA(levelmap,oldoil,BACKGROUND_INTENSITY,0,8,maxLX);
            WriteNumA(levelmap,oldsulphur,FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_INTENSITY,0,9,maxLX);
}
void Base::ShowBuildQueue(CHAR_INFO* levelmap,int maxLX,int maxLY,int fdeltaX,int fdeltaY){
    for(int i = 0;i<numBuildOrders;i++){
            switch(buildQueue[i].orderType){
                case TroopOrder:
   WriteNumA(levelmap,oldiron,FOREGROUND_INTENSITY,0,7,maxLX);
            WriteNumA(levelmap,oldoil,BACKGROUND_INTENSITY,0,8,maxLX);
            WriteNumA(levelmap,oldsulphur,FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_INTENSITY,0,9,maxLX);
                    WriteCharA(levelmap,'Y',FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_GREEN
                               ,min(x + buildQueue[i].x-fdeltaX,maxLX),
                               min(y + buildQueue[i].y-fdeltaY,maxLY),maxLX);
                break;

            }
        }
}
void Base::Unselected(CHAR_INFO* levelmap,int maxMapX,int maxMapY,int deltaX,int deltaY){
    for(int i = -resourceSearchSideLength;i<=resourceSearchSideLength;i++){
        for(int j = -resourceSearchSideLength;j<=resourceSearchSideLength;j++){
            int index = x + i - deltaX + (y + j-deltaY)*maxMapX;
            if(OutOfBounds(x+i-deltaX,y+j-deltaY,maxMapX,maxMapY))continue;
            levelmap[index].Attributes &= ~BACKGROUND_GREEN;
        }
    }
    for(int i = -resourceSearchSideLength;i<=resourceSearchSideLength;i++){
        for(int j = -resourceSearchSideLength;j<=resourceSearchSideLength;j++){
            int index = x + i + (y + j)*maxMapX;
            if(OutOfBounds(x+i,y+j,maxMapX,maxMapY))continue;
            levelmap[index].Attributes &= ~BACKGROUND_RED;

        }
    }

}

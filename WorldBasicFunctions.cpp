#include "WorldBasicFunctions.h"
bool g_bool = false;
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
inline bool compareCharWorldmap(CHAR_INFO* worldmap,int fx,int fy,int maxMapX,char compChar,WORD charAttr){
    return (worldmap[fx+fy*maxMapX].Char.AsciiChar == compChar && worldmap[fx+fy*maxMapX].Attributes == charAttr) ? 1 : 0;
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
void WorldGen(CHAR_INFO* worldmap,int maxMapX,int maxMapY,char seaChar,WORD seaAttr,int amountResources,int resourceGroupSize){
    cout << "Loading Map..." << endl;
    HeightMap hm = SquareDiamondTerrainGeneration(10,100,40);
    cout << "Map Done Loading";

    for(int i = 0; i < maxMapX;i++)
        for(int j = 0; j<maxMapY;j++) {
            worldmap[i+j*maxMapX].Char.AsciiChar = (hm.heightmap[i][j] < 10) ? seaChar
                                                    : ((hm.heightmap[i][j] == '\n' || hm.heightmap[i][j] == seaChar) ? 'F'
                                                    : hm.heightmap[i][j]);

            worldmap[i+j*maxMapX].Attributes = (hm.heightmap[i][j] < 15) ? seaAttr
                                                : (hm.heightmap[i][j]<20) ? seaAttr | FOREGROUND_INTENSITY
                                                : (hm.heightmap[i][j]<35) ? FOREGROUND_GREEN
                                                : (hm.heightmap[i][j]<100)? FOREGROUND_GREEN | FOREGROUND_INTENSITY
                                                : FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_INTENSITY;
    };
    cout << endl << "Starting Resource Generation";
    char resource[3];
    WORD resourceAttr[3];
        resource[0] = 'O';
    resourceAttr[0] = BACKGROUND_INTENSITY ;
        resource[1] = 'I';
    resourceAttr[1] = FOREGROUND_INTENSITY;
        resource[2] = 'S';
    resourceAttr[2] = FOREGROUND_RED  | FOREGROUND_GREEN | FOREGROUND_INTENSITY;
    int curX = rand()%maxMapX;
    int curY = rand()%maxMapY;
    for(int j = 0;j < amountResources;j++){

        for(int i = 0;i < resourceGroupSize;i++){

            worldmap[curX + curY*maxMapX].Char.AsciiChar = resource[j%3];
            worldmap[curX + curY*maxMapX].Attributes = resourceAttr[j%3];
            curX += ((rand()%3)-1);
            curY += ((rand()%3)-1);

            curX = min(curX,maxMapX-1);
            curY = min(curY,maxMapY-1);

            curX = max(0,curX);
            curY = max(0,curY);

        }

        curX = rand()%maxMapX;
        curY = rand()%maxMapY;
    }

    cout << "Done" << endl;
}
void MovePlayer(int& px,int& py,int deltaPX,int deltaPY,int& screenX,int& screenY,CHAR_INFO* wr,int maxMapX,int maxMapY,char seaChar){
            if(px + deltaPX >= maxMapX) return;
            if(py + deltaPY >= maxMapY) return;
            if(px + deltaPX <  0) return;
            if(py + deltaPY <  0) return;
            if(wr[px+deltaPX+(py+deltaPY)*maxMapX].Char.AsciiChar != seaChar){
                  px += ((px+deltaPX) < maxMapX) ? deltaPX : 0;
                  py += ((py+deltaPY) < maxMapY) ? deltaPY : 0;
                  screenX+=deltaPX;
                  screenY+=deltaPY;
            }
}

void VisionCone(Direction fdirection,int px,int py,CHAR_INFO* wr,CHAR_INFO* worldmap,int maxX,int maxY,int visionReach){
    for(int i = 0;i<=visionReach;i++){
        for(int j = (-i/2);j<=i/2;j++){
            switch(fdirection){
                case Right:
                    if(px+i >= maxX) break;
                    if(py+j >= maxY) break;
                    if(px+i < 0) break;
                    if(py+j < 0) break;
                   wr[px+i+(py+j)*maxX].Char.AsciiChar =  worldmap[px+i+(py+j)*maxX].Char.AsciiChar;
                   wr[px+i+(py+j)*maxX].Attributes =  worldmap[px+i+(py+j)*maxX].Attributes;

                    break;
                case Left:
                    if(px-i >= maxX) break;
                    if(py+j >= maxY) break;
                    if(px-i < 0) break;
                    if(py+j < 0) break;
                    wr[px-i+(py+j)*maxX].Char.AsciiChar =  worldmap[px-i+(py+j)*maxX].Char.AsciiChar;
                    wr[px-i+(py+j)*maxX].Attributes =  worldmap[px-i+(py+j)*maxX].Attributes;
                    break;
                case Up:
                    if(px+j >= maxX) break;
                    if(py-i >= maxY) break;
                    if(px+j < 0) break;
                    if(py-i < 0) break;
                    wr[px+j+(py-i)*maxX].Char.AsciiChar =  worldmap[px+j+(py-i)*maxX].Char.AsciiChar;
                    wr[px+j+(py-i)*maxX].Attributes =  worldmap[px+j+(py-i)*maxX].Attributes;
                    break;
                case Down:
                    if(px+j >= maxX) break;
                    if(py+i >= maxY) break;
                    if(px+j < 0) break;
                    if(py+i < 0) break;
                    wr[px+j+(py+i)*maxX].Char.AsciiChar =  worldmap[px+j+(py+i)*maxX].Char.AsciiChar;
                    wr[px+j+(py+i)*maxX].Attributes =  worldmap[px+j+(py+i)*maxX].Attributes;
                    break;
            }
        }
    }

}
void SaveGame(int px,int py,CHAR_INFO* worldmap,int maxX,int maxY){
    static int numSaved  = 0;

    FILE* f = fopen("Rerra.txt","w");
    for(int i = 0;i<maxX;i++){
        for(int j = 0;j<maxY;j++){
            fputc(worldmap[i+j*maxX].Char.AsciiChar,f);
        }
        fputc('\n',f);
    }
    fclose(f);
    numSaved++;
}
void TroopUpdate(Troop* troops,int numTroops,int deltaX,int deltaY,int maxLevelX,int maxLevelY,int maxMapX,int maxMapY,int movePoints,CHAR_INFO* levelmap,CHAR_INFO* worldmap,char seachar,Troop* selectedTroop){
    int index = 0;
    int screenTroopX = 0;
    int screenTroopY = 0;
    for(int i = 0;i<numTroops;i++){
        screenTroopX = troops[i].x-deltaX;
        screenTroopY = troops[i].y-deltaY;
        if(!OutOfBounds(screenTroopX,screenTroopY,maxLevelX,maxLevelY)){
                WriteCharA(levelmap,troops[i].graphics,troops[i].color,screenTroopX,screenTroopY,maxLevelX);
                troops[i].TravelDistanceX = min(movePoints,troops[i].cTravelDistanceX);
                troops[i].TravelDistanceY = min(movePoints,troops[i].cTravelDistanceY);

        }
         if(!OutOfBounds(troops[i].ty-deltaY,troops[i].ty-deltaY,maxLevelX,maxLevelY))troops[i].ShowMoveQueue(levelmap,maxLevelX,maxLevelY,deltaX,deltaY);
    }

    if(selectedTroop ){
        g_bool = false;
        for(int i = -selectedTroop->TravelDistanceX;i <= selectedTroop->TravelDistanceX;i++){
                for(int j = -selectedTroop->TravelDistanceY; j <= selectedTroop->TravelDistanceY; j++){

                    screenTroopX = selectedTroop->tx-deltaX + i;
                    screenTroopY = selectedTroop->ty-deltaY + j;

                    if(!OutOfBounds(screenTroopX,screenTroopY,maxLevelX,maxLevelY) && (abs(i)+abs(j)) <= movePoints){

                       if(i != 0 || j != 0) for(int i = 0 ;i<selectedTroop->numForbidden;i++)if(compareCharWorldmap(levelmap,screenTroopX,screenTroopY,maxLevelX,selectedTroop->forbiddenChar[i],selectedTroop->forbiddenAttr[i]))g_bool = true;

                           if(!g_bool) levelmap[screenTroopX  +  screenTroopY*maxLevelX].Attributes |= BACKGROUND_GREEN;
                            g_bool = false;


                    }
            }
        }
        if(!OutOfBounds(selectedTroop->x - deltaX,selectedTroop->y - deltaY,maxLevelX,maxLevelY))levelmap[selectedTroop->x - deltaX+(selectedTroop->y - deltaY)*maxLevelX].Attributes = selectedTroop->color | BACKGROUND_RED;

    }
}
void BaseUpdate(Base* bases,int numBases,int deltaX,int deltaY,int maxLevelX,int maxLevelY,int maxMapX,int maxMapY,int movePoints,CHAR_INFO* levelmap,CHAR_INFO* worldmap,Base* selectedBase){
    int index = 0;
    int screenBaseX = 0;
    int screenBaseY = 0;
    for(int i = 0;i<numBases;i++){
        screenBaseX = bases[i].x-deltaX;
        screenBaseY = bases[i].y-deltaY;
        if(!OutOfBounds(screenBaseX,screenBaseY,maxLevelX,maxLevelY)){
                WriteCharA(levelmap,bases[i].graphics,bases[i].colour,screenBaseX,screenBaseY,maxLevelX);
                bases[i].ShowBuildQueue(levelmap,maxLevelX,maxLevelY,deltaX,deltaY);
                for(int j = 0;j<bases[i].amountUnits;j++){
                    if(!OutOfBounds(bases[i].baseUnits[j]->bx-deltaX,bases[i].baseUnits[j]->by-deltaY,maxLevelX,maxLevelY))
                         WriteCharA(levelmap,
                                    bases[i].infantryUnitRepairGraphics
                                    ,bases[i].infantryUnitRepairColour
                                    ,bases[i].baseUnits[j]->bx-deltaX
                                    ,bases[i].baseUnits[j]->by-deltaY,maxLevelX);
                }

        }
    }

    if(selectedBase){
            selectedBase->Selected(levelmap,maxLevelX,maxLevelY,deltaX,deltaY);

    }

    }




#include "WorldBasicFunctions.h"
bool g_bool = false;

inline bool compareCharWorldmap(CHAR_INFO* worldmap,int fx,int fy,int maxMapX,char compChar,WORD charAttr){
    return (worldmap[fx+fy*maxMapX].Char.AsciiChar == compChar && worldmap[fx+fy*maxMapX].Attributes == charAttr) ? 1 : 0;
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
void InfantryUpdate(Infantry* infantries,int numInfantry,Tank* tanks,int numTanks,int deltaX,int deltaY,int maxLevelX,int maxLevelY,int maxMapX,int maxMapY,int movePoints,CHAR_INFO* levelmap,CHAR_INFO* worldmap,char seachar,Infantry* selectedInfantry,Tank* selectedTank){
    int index = 0;
    int screenInfantryX = 0;
    int screenInfantryY = 0;
    for(int i = 0;i<numInfantry;i++){
        screenInfantryX = infantries[i].x-deltaX;
        screenInfantryY = infantries[i].y-deltaY;
        if(!OutOfBounds(screenInfantryX,screenInfantryY,maxLevelX,maxLevelY)){
                WriteCharA(levelmap,infantries[i].graphics,infantries[i].color,screenInfantryX,screenInfantryY,maxLevelX);
                infantries[i].TravelDistanceX = min(movePoints,infantries[i].cTravelDistanceX);
                infantries[i].TravelDistanceY = min(movePoints,infantries[i].cTravelDistanceY);

        }
        infantries[i].ShowMoveQueue(levelmap,maxLevelX,maxLevelY,deltaX,deltaY);
    }

    if(selectedInfantry ){
        selectedInfantry->Selected(levelmap,maxLevelX,maxLevelY,deltaX,deltaY,movePoints);
        if(!OutOfBounds(selectedInfantry->x - deltaX,selectedInfantry->y - deltaY,maxLevelX,maxLevelY))
            levelmap[selectedInfantry->x - deltaX+(selectedInfantry->y - deltaY)*maxLevelX].Attributes = selectedInfantry->color | BACKGROUND_RED;

    }
    for(int i = 0;i<numTanks;i++){
        screenInfantryX = tanks[i].x-deltaX;
        screenInfantryY = tanks[i].y-deltaY;
        if(!OutOfBounds(screenInfantryX,screenInfantryY,maxLevelX,maxLevelY)){
                WriteCharA(levelmap,tanks[i].graphics,tanks[i].color,screenInfantryX,screenInfantryY,maxLevelX);
                tanks[i].TravelDistanceX = min(movePoints,tanks[i].cTravelDistanceX);
                tanks[i].TravelDistanceY = min(movePoints,tanks[i].cTravelDistanceY);

        }
        tanks[i].ShowMoveQueue(levelmap,maxLevelX,maxLevelY,deltaX,deltaY);
    }

    if(selectedTank){
        selectedTank->Selected(levelmap,maxLevelX,maxLevelY,deltaX,deltaY,movePoints);
        WriteStrNum(levelmap,"Percent of Move Points removed: ",100/selectedTank->movePointReducer,0,0,maxLevelX);
        if(!OutOfBounds(selectedTank->x - deltaX,selectedTank->y - deltaY,maxLevelX,maxLevelY))
            levelmap[selectedTank->x - deltaX+(selectedTank->y - deltaY)*maxLevelX].Attributes = selectedTank->color | BACKGROUND_RED;

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




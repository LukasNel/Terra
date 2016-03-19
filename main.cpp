#include <iostream>
#include "ConsoleFunctions.h"
#include "TerrainGeneration.h"
#include "WorldBasicFunctions.h"

#include "Infantry.h"
#include "Tanks.h"

#include <iostream>
#include <windows.h>
#include <windowsx.h>
#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <dos.h>
#include <wincon.h>

#define MAX_UNITS_PER_TYPE 256

int maxMP = 100;
using namespace std;

int maxX = 1025,maxY = 1025;
int maxLevelX = 0,maxLevelY = 0;
int deltaX = 0,deltaY = 0;

CHAR_INFO* worldmap = NULL;
CHAR_INFO* levelmap = NULL;

void ErrorExit (LPSTR lpszMessage);

CONSOLE_SCREEN_BUFFER_INFO csbi;
HANDLE hStdin;
DWORD fdwSaveOldMode;
DWORD cNumRead;
INPUT_RECORD irInBuf[128];

int GameStart();
inline int CheckForResize();
int GameMain();
void GameMainKeyBoard(KEY_EVENT_RECORD kEvent);
bool OnceOff = true;
void GameMainMouse( MOUSE_EVENT_RECORD mEvent);
int Render();
int GameEnd();
int g_oil = 0;
int g_sulphur = 0;
int g_iron = 0;
void SpawnInfantry(int fnumInfantry,CHAR_INFO* worldmap,int maxMapX,int maxMapY,int maxLX,int maxLY,char seaChar);

int numInfantry = 0;
Infantry infantries[MAX_UNITS_PER_TYPE];
Infantry* selectedInfantry = NULL;

int numTanks = 0;
Tank tanks[MAX_UNITS_PER_TYPE];
Tank* selectedTank = NULL;

Base bases[MAX_UNITS_PER_TYPE];
Base* selectedBase = NULL;
int numBases = 0;
bool GameRunning = true;

int movePoints = 20;

enum BuildState{
    BuildingTanks,
    BuildingInfantry

};
BuildState curBuildState = BuildingInfantry;
int main()
{

    unsigned int seed = GetTickCount();
    srand(seed);
    movePoints = maxMP;
    if(!GameStart())return 0;

    while(GameRunning){
        seed = GetTickCount();
        GameMain();
        while(GetTickCount() - seed < 33);
    }

     if(!GameEnd())return 0;
    cout << "Hello world!" << endl;
    return 0;
}
void SpawnInfantry(int fnumInfantry,CHAR_INFO* worldmap,int maxMapX,int maxMapY,int maxLX,int maxLY,char seaChar){
    int curInfantryX = rand()%maxMapX;
    int curInfantryY = rand()%maxMapY;
    while(worldmap[curInfantryX+curInfantryY*maxMapX].Char.AsciiChar == seaChar){
        curInfantryX = rand()%maxMapX;
        curInfantryY = rand()%maxMapY;
    }

    for(int i = 0 ;i< fnumInfantry;i++){
        infantries[numInfantry++] = Infantry(curInfantryX,curInfantryY);
        curInfantryX += ((rand()%3)-1);
        curInfantryY += ((rand()%3)-1);
        curInfantryX = max(0,curInfantryX);
        curInfantryX = min(maxMapX,curInfantryX);
        curInfantryY = max(0,curInfantryY);
        curInfantryY = min(maxMapY,curInfantryY);
        while(worldmap[curInfantryX+curInfantryY*maxMapX].Char.AsciiChar == seaChar
              && worldmap[curInfantryX+curInfantryY*maxMapX].Char.AsciiChar != infantries[numInfantry-1].graphics){
            curInfantryX += ((rand()%3)-1)%curInfantryX;
            curInfantryY += ((rand()%3)-1)%curInfantryY;
            curInfantryX = max(0,curInfantryX);
            curInfantryY = max(0,curInfantryY);
        }
    }
    deltaX = curInfantryX - maxLX/2;
    deltaY = curInfantryY - maxLY/2;
}
int GameStart(){
    csbi = screenInfo();

    maxLevelX = csbi.srWindow.Right+1;
    maxLevelY = csbi.srWindow.Bottom+1;

    worldmap = new CHAR_INFO[maxX*maxY];
    levelmap = new CHAR_INFO[maxLevelX*maxLevelY];

    hStdin = GetStdHandle(STD_INPUT_HANDLE);
    if (hStdin == INVALID_HANDLE_VALUE)
        ErrorExit("GetStdHandle");

    // Save the current input mode, to be restored on exit.

    if (! GetConsoleMode(hStdin, &fdwSaveOldMode) )
        ErrorExit("GetConsoleMode");

    // Enable the window and mouse input events.
    if (! SetConsoleMode(hStdin, ENABLE_WINDOW_INPUT | ENABLE_MOUSE_INPUT) )
        ErrorExit("SetConsoleMode");

    ClearBuffer(worldmap,maxX,maxY);

    WorldGen(worldmap,maxX,maxY,seachar,seaattr,256,100);
   SpawnInfantry(4,worldmap,maxX,maxY,maxLevelX,maxLevelY,seachar);
    return 1;
}
COORD mousePos = {20,20};
int alternating_variable = 0;
int GameMain(){
        CheckForResize();
         if (! PeekConsoleInput(
                hStdin,      // input buffer handle
                irInBuf,     // buffer to read into
                128,         // size of read buffer
                &cNumRead) ) // number of records read
            ErrorExit("ReadConsoleInput");

        // Dispatch the events to the appropriate handler.

        for (int i = 0; i < cNumRead; i++)
        {
            switch(irInBuf[i].EventType)
            {
                case KEY_EVENT: // keyboard input
                    GameMainKeyBoard(irInBuf[i].Event.KeyEvent);
                    break;

                case MOUSE_EVENT: // mouse input
                    GameMainMouse(irInBuf[i].Event.MouseEvent);
                    break;
                case WINDOW_BUFFER_SIZE_EVENT: // scrn buf. resizing

                    break;

                case FOCUS_EVENT:  // disregard focus events

                case MENU_EVENT:   // disregard menu events
                    break;

                default:
                    ErrorExit("Unknown event type");
                    break;
            }
        }
        if(mousePos.X < 2)deltaX--;
        if(mousePos.Y < 2)deltaY--;
        if(mousePos.X > maxLevelX-2)deltaX++;
        if(mousePos.Y > maxLevelY-2)deltaY++;

        if(alternating_variable > 10){
        BaseUpdate(bases,numBases,deltaX,deltaY,maxLevelX,maxLevelY,maxX,maxY,movePoints,levelmap,worldmap,selectedBase);
        InfantryUpdate(infantries,numInfantry,tanks,numTanks,deltaX,deltaY,maxLevelX,maxLevelY,maxX,maxY,movePoints,levelmap,worldmap,seachar,selectedInfantry,selectedTank);
        }else{
        InfantryUpdate(infantries,numInfantry,tanks,numTanks,deltaX,deltaY,maxLevelX,maxLevelY,maxX,maxY,movePoints,levelmap,worldmap,seachar,selectedInfantry,selectedTank);
        BaseUpdate(bases,numBases,deltaX,deltaY,maxLevelX,maxLevelY,maxX,maxY,movePoints,levelmap,worldmap,selectedBase);
        }

        Render();
        FlushConsoleInputBuffer(hStdin);


        alternating_variable++;
        if(alternating_variable > 20) alternating_variable = 0;
        return 1;
}

int Render(){
    if(deltaX > maxX - maxLevelX)deltaX = maxX- maxLevelX;
    if(deltaX < 0)deltaX = 0;
    if(deltaY < 0)deltaY = 0;
    if(deltaY > maxY - maxLevelY)deltaY = maxY-maxLevelY;

        WriteStrNumA(levelmap,"Move Points: ",movePoints,FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY,FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY,0,1,maxLevelX);
        WriteNumA(levelmap,deltaX,    FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY,0,2,maxLevelX);
        WriteNumA(levelmap,deltaY,    FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY,0,3,maxLevelX);
        WriteNumA(levelmap,mousePos.X,FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY,0,4,maxLevelX);
        WriteNumA(levelmap,mousePos.Y,FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY,0,5,maxLevelX);


        WriteStrA(levelmap,"               ",FOREGROUND_INTENSITY,0,maxLevelY-1,maxLevelX);
        WriteStrA(levelmap,"               ",BACKGROUND_INTENSITY,0,maxLevelY-2,maxLevelX);
        WriteStrA(levelmap,"               ",FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_INTENSITY,0,maxLevelY-3,maxLevelX);

        WriteStrA(levelmap,"Total Iron: ",FOREGROUND_INTENSITY,0,maxLevelY-1,maxLevelX);
        WriteStrA(levelmap,"Total Oil: ",BACKGROUND_INTENSITY,0,maxLevelY-2,maxLevelX);
        WriteStrA(levelmap,"Total Sulphur: ",FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_INTENSITY,0,maxLevelY-3,maxLevelX);

        WriteNumA(levelmap,g_iron,FOREGROUND_INTENSITY,strlen("Total Sulphur: "),maxLevelY-1,maxLevelX);
        WriteNumA(levelmap,g_oil,BACKGROUND_INTENSITY,strlen("Total Sulphur: "),maxLevelY-2,maxLevelX);
        WriteNumA(levelmap,g_sulphur,FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_INTENSITY,strlen("Total Sulphur: "),maxLevelY-3,maxLevelX);

        SetConsoleBufferL((levelmap),maxLevelX,maxLevelY,deltaX,deltaY);
    ClearBuffer(levelmap,maxLevelX,maxLevelY);
     for(int i =0;i<maxLevelX;i++)
        for(int j =0;j<maxLevelY;j++)levelmap[i+j*maxLevelX] = worldmap[i+deltaX+(j+deltaY)*maxX];


}
int GameEnd(){
    delete[] levelmap;
    delete[] worldmap;
     return 1;
}
inline int CheckForResize(){
        csbi = screenInfo();

    if(maxLevelX - 1 != csbi.srWindow.Right || maxLevelY - 1 != csbi.srWindow.Bottom){
            maxLevelX = csbi.srWindow.Right+1;
            maxLevelY = csbi.srWindow.Bottom+1;
            delete[] levelmap;

            levelmap = new CHAR_INFO[(maxLevelX*maxLevelY)];
            ClearBuffer(levelmap,maxLevelX,maxLevelY);
    }


}
bool testRange = false;
void GameMainKeyBoard(KEY_EVENT_RECORD kEvent){
    if(kEvent.uChar.AsciiChar == 'g'){
        deltaX = infantries[rand()%numInfantry].x-maxLevelX/2;
        deltaY = infantries[rand()%numInfantry].y-maxLevelY/2;
    }
    if(kEvent.wVirtualKeyCode == VK_SPACE){
        for(int i = 0;i<numInfantry;i++){
            infantries[i].ExecuteMoveQueue();
        }
        for(int i = 0;i<numTanks;i++){
            tanks[i].ExecuteMoveQueue();
        }
         g_oil = 0;
         g_sulphur = 0;
         g_iron = 0;

        for(int i = 0;i<numBases;i++){
            g_oil+=bases[i].oldoil;
            g_sulphur+=bases[i].oldsulphur;
            g_iron+=bases[i].oldiron;
    }
        for(int i = 0;i<numBases;i++){
            bases[i].oil = g_oil;
            bases[i].sulphur = g_sulphur;
            bases[i].iron = g_iron;

            bases[i].ExecuteBuildQueue(infantries,numInfantry,tanks,numTanks,maxX,maxY,seachar,seaattr,worldmap);

            g_oil -= (g_oil - bases[i].oil);
            g_sulphur -=  (g_sulphur - bases[i].sulphur);
            g_iron -= (g_iron - bases[i].iron);

            bases[i].oil = bases[i].oldoil;
            bases[i].sulphur = bases[i].oldsulphur;
            bases[i].iron = bases[i].oldiron;
        }
        movePoints = maxMP;
    }
    if(kEvent.uChar.AsciiChar == 'b' && selectedInfantry){
        selectedInfantry->BuildBase(bases,numBases,g_oil,g_iron,g_sulphur,worldmap,maxX,maxY);
    }
    if(kEvent.uChar.AsciiChar == 't'){
        curBuildState = BuildingTanks;
    }

    if(kEvent.uChar.AsciiChar == 'i'){
        curBuildState = BuildingInfantry;
    }
}
int GMMScreenInfantryX = 0;
int GMMScreenInfantryY = 0;
bool GMMTraversable = true;
void GameMainMouse(MOUSE_EVENT_RECORD mEvent){
    GMMScreenInfantryX = 0;
    GMMScreenInfantryY = 0;
    GMMTraversable = true;
    switch(mEvent.dwEventFlags){
        case MOUSE_MOVED:
            mousePos.X = mEvent.dwMousePosition.X;
            mousePos.Y = mEvent.dwMousePosition.Y;
        break;
        case 0:
            switch(mEvent.dwButtonState){
                case FROM_LEFT_1ST_BUTTON_PRESSED:
                    for(int i = 0;i < numInfantry;i++){
                        Infantry& t_Infantry = infantries[i];
                        if(t_Infantry.x == mousePos.X+deltaX && t_Infantry.y == mousePos.Y+deltaY){
                            if(!t_Infantry.activation){
                                t_Infantry.Activate();
                                if(selectedInfantry)selectedInfantry->Deactivate();
                                selectedInfantry = &t_Infantry;
                                if(selectedBase)selectedBase->Deactivate();
                                if(selectedBase )selectedBase = NULL;
                                if(selectedTank)selectedTank->Deactivate();
                                if(selectedTank)selectedTank = NULL;

                            }else{
                                t_Infantry.Deactivate();
                                if(selectedInfantry && t_Infantry.x == selectedInfantry->x && t_Infantry.y == selectedInfantry->y)selectedInfantry = NULL;

                            }
                        }
                    }
                    for(int i = 0;i < numTanks;i++){
                        Tank& t_tank = tanks[i];
                        if(t_tank.x == mousePos.X+deltaX && t_tank.y == mousePos.Y+deltaY){
                            if(!t_tank.activation){
                                t_tank.Activate();
                                if(selectedTank)selectedTank->Deactivate();
                                selectedTank = &t_tank;
                                if(selectedBase)selectedBase->Deactivate();
                                if(selectedBase )selectedBase = NULL;
                                if(selectedInfantry)selectedInfantry->Deactivate();
                                if(selectedInfantry )selectedInfantry = NULL;

                            }else{

                                t_tank.Deactivate();
                                if(selectedTank && t_tank.x == selectedTank->x && t_tank.y == selectedTank->y)selectedTank = NULL;

                            }

                        }
                    }
                    for(int i = 0;i < numBases;i++){
                        Base& bBase = bases[i];
                        if(bBase.x == mousePos.X+deltaX && bBase.y == mousePos.Y+deltaY){
                            if(!bBase.selected){
                                bBase.selected = true;
                                if(selectedBase)selectedBase->Deactivate();
                                bBase.Activate();
                                selectedBase = &bBase;
                                if(selectedInfantry)selectedInfantry->Deactivate();
                                if(selectedInfantry )selectedInfantry = NULL;

                            }else{
                                bBase.selected = false;
                                if(selectedBase)selectedBase->Deactivate();
                                if(selectedBase && bBase.x == selectedBase->x && bBase.y == selectedBase->y)selectedBase = NULL;

                            }

                        }
                    }
                break;
                case RIGHTMOST_BUTTON_PRESSED:
                     if(selectedInfantry && movePoints > 0){

                        GMMScreenInfantryX = selectedInfantry->tx - deltaX;
                        GMMScreenInfantryY = selectedInfantry->ty - deltaY;

                        if(abs(GMMScreenInfantryX - mousePos.X) <= selectedInfantry->TravelDistanceX
                        && abs(GMMScreenInfantryY - mousePos.Y) <= selectedInfantry->TravelDistanceY){

                            if(worldmap[mousePos.X + deltaX + (mousePos.Y+deltaY)*maxX].Char.AsciiChar != seachar){
                            for(int i = 0;i<numInfantry;i++)if(infantries[i].tx - deltaX == mousePos.X && infantries[i].ty - deltaY == mousePos.Y)GMMTraversable = false;
                            if(GMMTraversable && movePoints - (abs(GMMScreenInfantryX - mousePos.X)+abs(GMMScreenInfantryY - mousePos.Y)) >= 0){
                                movePoints -= (abs(GMMScreenInfantryX - mousePos.X)+abs(GMMScreenInfantryY - mousePos.Y));


                                selectedInfantry->Translate(mousePos.X - GMMScreenInfantryX,mousePos.Y - GMMScreenInfantryY,worldmap[mousePos.X + deltaX + (mousePos.Y+deltaY)*maxX]);

                            }
                            }


                        }
                      }
                    if(selectedTank && movePoints > 0){

                        GMMScreenInfantryX = selectedTank->tx - deltaX;
                        GMMScreenInfantryY = selectedTank->ty - deltaY;

                        if(abs(GMMScreenInfantryX - mousePos.X) <= selectedTank->TravelDistanceX
                        && abs(GMMScreenInfantryY - mousePos.Y) <= selectedTank->TravelDistanceY){

                            if(!selectedTank->checkIfForbidden(worldmap[mousePos.X + deltaX + (mousePos.Y+deltaY)*maxX].Char.AsciiChar,worldmap[mousePos.X + deltaX + (mousePos.Y+deltaY)*maxX].Attributes)){

                            for(int i = 0;i<numTanks;i++)if(tanks[i].tx - deltaX == mousePos.X && tanks[i].ty - deltaY == mousePos.Y)GMMTraversable = false;

                            if(GMMTraversable && movePoints - (((abs(GMMScreenInfantryX - mousePos.X)+abs(GMMScreenInfantryY - mousePos.Y)))/selectedTank->movePointReducer) >= 0){
                                movePoints -= (((abs(GMMScreenInfantryX - mousePos.X)+abs(GMMScreenInfantryY - mousePos.Y)))/selectedTank->movePointReducer);


                                selectedTank->Translate(mousePos.X - GMMScreenInfantryX,mousePos.Y - GMMScreenInfantryY,worldmap[mousePos.X + deltaX + (mousePos.Y+deltaY)*maxX]);

                            }
                            }


                        }
                    }
                    if(selectedBase){
                        if(selectedBase->troopBuildingSideLength >= abs(mousePos.X + deltaX - selectedBase->x) ){
                        if(selectedBase->troopBuildingSideLength >= abs(mousePos.Y + deltaY - selectedBase->y)){
                       if(abs(mousePos.X + deltaX) != selectedBase->x || abs(mousePos.Y + deltaY) != selectedBase->y){
                            switch(curBuildState){
                                case BuildingInfantry:
                                selectedBase->AddTroopToBuildQueue(mousePos.X + deltaX - selectedBase->x,mousePos.Y + deltaY - selectedBase->y,InfantryOrder);
                                break;
                                case BuildingTanks:
                                selectedBase->AddTroopToBuildQueue(mousePos.X + deltaX - selectedBase->x,mousePos.Y + deltaY - selectedBase->y,TankOrder);
                                break;
                                default:break;
                            }
                       }
                       }
                       }

                    }


                    break;

            }

        break;
    }
}
void ErrorExit (LPSTR lpszMessage)
{
    fprintf(stderr, "%s\n", lpszMessage);

    // Restore input mode on exit.

    SetConsoleMode(hStdin, fdwSaveOldMode);

    ExitProcess(0);
}
/*
    for(int i = 0;i<numInfantry;i++)
        if(Infantry[i].x-deltaX >= 0 && Infantry[i].y-deltaY >= 0 &&
        Infantry[i].x-deltaX < maxLevelX && Infantrys[i].y-deltaY < maxLevelY)
                WriteCharA(levelmap,Infantrys[i].graphics,Infantrys[i].color,Infantrys[i].x-deltaX,Infantrys[i].y-deltaY,maxLevelX);

     for(int i = 0;i<numInfantrys;i++)Infantrys[i].TravelDistanceX = min(movePoints,Infantrys[i].cTravelDistanceX),Infantrys[i].TravelDistanceY = min(movePoints,Infantrys[i].cTravelDistanceY);

    if(selectedInfantry && selectedInfantry->tx-deltaX >= 0 && selectedInfantry->ty-deltaY >= 0 &&
        selectedInfantry->tx-deltaX < maxLevelX && selectedInfantry->ty-deltaY < maxLevelY){
        for(int i = -selectedInfantry->TravelDistanceX;i <= selectedInfantry->TravelDistanceX;i++){
                for(int j = -selectedInfantry->TravelDistanceY;j <= selectedInfantry->TravelDistanceY;j++){
                    if(selectedInfantry->tx-deltaX + i >= 0 && selectedInfantry->ty-deltaY+j >= 0 &&
                        selectedInfantry->tx-deltaX + i < maxLevelX && selectedInfantry->ty-deltaY + j < maxLevelY &&
                         (abs(i)+abs(j)) <= movePoints){
                            levelmap[selectedInfantry->tx - deltaX + i  +
                            ( selectedInfantry->ty - deltaY + j)*maxLevelX].Attributes |= BACKGROUND_GREEN;
                        }
            }
        }

           //levelmap[selectedInfantry->x - deltaX+(selectedInfantry->y - deltaY)*maxLevelX].Attributes = selectedInfantry->color | BACKGROUND_RED;
        }
        for(int i = 0;i<numInfantrys;i++)
            if(Infantrys[i].x-deltaX >= 0 && Infantrys[i].y-deltaY >= 0 &&
            Infantrys[i].x-deltaX < maxLevelX && Infantrys[i].y-deltaY < maxLevelY)
                    Infantrys[i].MoveQueueShow(levelmap,maxLevelX,maxLevelY,deltaX,deltaY);

*/


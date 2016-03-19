#include <iostream>
#include "ConsoleFunctions.h"
#include "TerrainGeneration.h"
#include "WorldBasicFunctions.h"
#include <iostream>
#include <windows.h>
#include <windowsx.h>
#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <dos.h>
#include <wincon.h>
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
void SpawnTroops(int fnumTroops,CHAR_INFO* worldmap,int maxMapX,int maxMapY,int maxLX,int maxLY,char seaChar);
int numTroops = 0;
Troop troops[256];
Troop* selectedTroop = NULL;
Base bases[256];
Base* selectedBase = NULL;
int numBases = 0;
bool GameRunning = true;

int movePoints = 20;

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
void SpawnTroops(int fnumTroops,CHAR_INFO* worldmap,int maxMapX,int maxMapY,int maxLX,int maxLY,char seaChar){
    int curTroopX = rand()%maxMapX;
    int curTroopY = rand()%maxMapY;
    while(worldmap[curTroopX+curTroopY*maxMapX].Char.AsciiChar == seaChar){
        curTroopX = rand()%maxMapX;
        curTroopY = rand()%maxMapY;
    }

    for(int i = 0 ;i< fnumTroops;i++){
        troops[numTroops++] = Troop(curTroopX,curTroopY);
        curTroopX += ((rand()%3)-1);
        curTroopY += ((rand()%3)-1);
        curTroopX = max(0,curTroopX);
        curTroopX = min(maxMapX,curTroopX);
        curTroopY = max(0,curTroopY);
        curTroopY = min(maxMapY,curTroopY);
        while(worldmap[curTroopX+curTroopY*maxMapX].Char.AsciiChar == seaChar
              && worldmap[curTroopX+curTroopY*maxMapX].Char.AsciiChar != troops[numTroops-1].graphics){
            curTroopX += ((rand()%3)-1)%curTroopX;
            curTroopY += ((rand()%3)-1)%curTroopY;
            curTroopX = max(0,curTroopX);
            curTroopY = max(0,curTroopY);
        }
    }
    deltaX = curTroopX - maxLX/2;
    deltaY = curTroopY - maxLY/2;
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
   SpawnTroops(4,worldmap,maxX,maxY,maxLevelX,maxLevelY,seachar);
    return 1;
}
COORD mousePos = {20,20};
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

        Render();
        FlushConsoleInputBuffer(hStdin);
        return 1;
}
int alternating_variable = 0;
int Render(){
    if(deltaX > maxX - maxLevelX)deltaX = maxX- maxLevelX;
    if(deltaX < 0)deltaX = 0;
    if(deltaY < 0)deltaY = 0;
    if(deltaY > maxY - maxLevelY)deltaY = maxY-maxLevelY;

    for(int i =0;i<maxLevelX;i++)
        for(int j =0;j<maxLevelY;j++)levelmap[i+j*maxLevelX] = worldmap[i+deltaX+(j+deltaY)*maxX];
/*
    for(int i = 0;i<numTroops;i++)
        if(troops[i].x-deltaX >= 0 && troops[i].y-deltaY >= 0 &&
        troops[i].x-deltaX < maxLevelX && troops[i].y-deltaY < maxLevelY)
                WriteCharA(levelmap,troops[i].graphics,troops[i].color,troops[i].x-deltaX,troops[i].y-deltaY,maxLevelX);

     for(int i = 0;i<numTroops;i++)troops[i].TravelDistanceX = min(movePoints,troops[i].cTravelDistanceX),troops[i].TravelDistanceY = min(movePoints,troops[i].cTravelDistanceY);

    if(selectedTroop && selectedTroop->tx-deltaX >= 0 && selectedTroop->ty-deltaY >= 0 &&
        selectedTroop->tx-deltaX < maxLevelX && selectedTroop->ty-deltaY < maxLevelY){
        for(int i = -selectedTroop->TravelDistanceX;i <= selectedTroop->TravelDistanceX;i++){
                for(int j = -selectedTroop->TravelDistanceY;j <= selectedTroop->TravelDistanceY;j++){
                    if(selectedTroop->tx-deltaX + i >= 0 && selectedTroop->ty-deltaY+j >= 0 &&
                        selectedTroop->tx-deltaX + i < maxLevelX && selectedTroop->ty-deltaY + j < maxLevelY &&
                         (abs(i)+abs(j)) <= movePoints){
                            levelmap[selectedTroop->tx - deltaX + i  +
                            ( selectedTroop->ty - deltaY + j)*maxLevelX].Attributes |= BACKGROUND_GREEN;
                        }
            }
        }

           //levelmap[selectedTroop->x - deltaX+(selectedTroop->y - deltaY)*maxLevelX].Attributes = selectedTroop->color | BACKGROUND_RED;
        }
        for(int i = 0;i<numTroops;i++)
            if(troops[i].x-deltaX >= 0 && troops[i].y-deltaY >= 0 &&
            troops[i].x-deltaX < maxLevelX && troops[i].y-deltaY < maxLevelY)
                    troops[i].MoveQueueShow(levelmap,maxLevelX,maxLevelY,deltaX,deltaY);

*/

        if(alternating_variable > 10){
        BaseUpdate(bases,numBases,deltaX,deltaY,maxLevelX,maxLevelY,maxX,maxY,movePoints,levelmap,worldmap,selectedBase);
        TroopUpdate(troops,numTroops,deltaX,deltaY,maxLevelX,maxLevelY,maxX,maxY,movePoints,levelmap,worldmap,seachar,selectedTroop);
        }else{
        TroopUpdate(troops,numTroops,deltaX,deltaY,maxLevelX,maxLevelY,maxX,maxY,movePoints,levelmap,worldmap,seachar,selectedTroop);
        BaseUpdate(bases,numBases,deltaX,deltaY,maxLevelX,maxLevelY,maxX,maxY,movePoints,levelmap,worldmap,selectedBase);
        }
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
     alternating_variable++;
    if(alternating_variable > 20) alternating_variable = 0;

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
        deltaX = troops[rand()%numTroops].x-maxLevelX/2;
        deltaY = troops[rand()%numTroops].y-maxLevelY/2;
    }
    if(kEvent.wVirtualKeyCode == VK_SPACE){
        for(int i = 0;i<numTroops;i++){
            troops[i].ExecuteMoveQueue();
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

            bases[i].ExecuteBuildQueue(troops,numTroops,maxX,maxY,seachar,seaattr,worldmap);

            g_oil -= (g_oil - bases[i].oil);
            g_sulphur -=  (g_sulphur - bases[i].sulphur);
            g_iron -= (g_iron - bases[i].iron);

            bases[i].oil = bases[i].oldoil;
            bases[i].sulphur = bases[i].oldsulphur;
            bases[i].iron = bases[i].oldiron;
        }
        movePoints = maxMP;
    }
    if(kEvent.uChar.AsciiChar == 't' && selectedTroop){
        selectedTroop->BuildBase(bases,numBases,g_oil,g_iron,g_sulphur,worldmap,maxX,maxY);
    }

}
int GMMScreenTroopX = 0;
int GMMScreenTroopY = 0;
bool GMMTraversable = true;
void GameMainMouse(MOUSE_EVENT_RECORD mEvent){
    GMMScreenTroopX = 0;
    GMMScreenTroopY = 0;
    GMMTraversable = true;
    switch(mEvent.dwEventFlags){
        case MOUSE_MOVED:
            mousePos.X = mEvent.dwMousePosition.X;
            mousePos.Y = mEvent.dwMousePosition.Y;
        break;
        case 0:
            switch(mEvent.dwButtonState){
                case FROM_LEFT_1ST_BUTTON_PRESSED:
                    for(int i = 0;i < numTroops;i++){
                        Troop& t_troop = troops[i];
                        if(t_troop.x == mousePos.X+deltaX && t_troop.y == mousePos.Y+deltaY){
                            if(!t_troop.activation){
                                t_troop.Activate();
                                if(selectedTroop)selectedTroop->Deactivate();
                                selectedTroop = &t_troop;
                                if(selectedBase)selectedBase->Deactivate();
                                if(selectedBase )selectedBase = NULL;

                            }else{
                                t_troop.Deactivate();
                                if(selectedTroop && t_troop.x == selectedTroop->x && t_troop.y == selectedTroop->y)selectedTroop = NULL;

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
                                if(selectedTroop)selectedTroop->Deactivate();
                                if(selectedTroop )selectedTroop = NULL;

                            }else{
                                bBase.selected = false;
                                if(selectedBase)selectedBase->Deactivate();
                                if(selectedBase && bBase.x == selectedBase->x && bBase.y == selectedBase->y)selectedBase = NULL;

                            }

                        }
                    }
                break;
                case RIGHTMOST_BUTTON_PRESSED:
                     if(selectedTroop && movePoints > 0){
                        GMMScreenTroopX = selectedTroop->tx - deltaX;
                        GMMScreenTroopY = selectedTroop->ty - deltaY;
                        if(abs(GMMScreenTroopX - mousePos.X) <= selectedTroop->TravelDistanceX
                        && abs(GMMScreenTroopY - mousePos.Y) <= selectedTroop->TravelDistanceY){
                            if(worldmap[mousePos.X + deltaX + (mousePos.Y+deltaY)*maxX].Char.AsciiChar != seachar){
                            for(int i = 0;i<numTroops;i++)if(troops[i].tx - deltaX == mousePos.X && troops[i].ty - deltaY == mousePos.Y)GMMTraversable = false;
                            if(GMMTraversable && movePoints - (abs(GMMScreenTroopX - mousePos.X)+abs(GMMScreenTroopY - mousePos.Y)) >= 0){
                                movePoints -= (abs(GMMScreenTroopX - mousePos.X)+abs(GMMScreenTroopY - mousePos.Y));


                                selectedTroop->Translate(mousePos.X - GMMScreenTroopX,mousePos.Y - GMMScreenTroopY,worldmap[mousePos.X + deltaX + (mousePos.Y+deltaY)*maxX]);

                            }
                            }


                        }
                      }
                    if(selectedBase){
                        if(selectedBase->troopBuildingSideLength >= abs(mousePos.X + deltaX - selectedBase->x) ){
                        if(selectedBase->troopBuildingSideLength >= abs(mousePos.Y + deltaY - selectedBase->y)){
                       // if(abs(mousePos.X + deltaX - selectedBase->x) || abs(mousePos.Y + deltaY - selectedBase->y)>=1){
                            selectedBase->AddTroopToBuildQueue(mousePos.X + deltaX - selectedBase->x,mousePos.Y + deltaY - selectedBase->y);
                       // }
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

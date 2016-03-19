#ifndef WORLDBASICFUNCTIONS_H_INCLUDED
#define WORLDBASICFUNCTIONS_H_INCLUDED
#include <windowsx.h>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#define OutOfBounds(fx,fy,mx,my) ((fx >= 0 && fy >= 0 && fx < mx && fy < my) ? 0 : 1)
#include "ConsoleFunctions.h"
using namespace std;

void SaveGame(int px,int py,CHAR_INFO* worldmap,int maxX,int maxY);
void VisionCone(Direction fdirection,int px,int py,CHAR_INFO* wr,CHAR_INFO* worldmap,int maxX,int maxY,int visionReach);
void MovePlayer(int& px,int& py,int deltaPX,int deltaPY,int& screenX,int& screenY,CHAR_INFO* wr,int maxMapX,int maxMapY,char seaChar);
void WorldGen(CHAR_INFO* worldmap,int maxMapX,int maxMapY,char seaChar,int amountResources,int resourceGroupSize);

char seachar = 'S';
WORD seaattr = FOREGROUND_BLUE;

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
struct Base;
struct Troop{
    int x,y;
    int tx,ty;
    WORD color;
    Move moveQueue[100];
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
    void BuildBase(Base* bases,int& numBases,int& gOil,int& gIron,int& gSulphur,CHAR_INFO* worldmap,int maxX,int maxY);

   void Translate(int dx,int dy,CHAR_INFO destinationSquare);
    void Activate();
    void Deactivate();
    void ShowMoveQueue(CHAR_INFO* levelmap,int maxLX,int maxLY,int fdeltaX,int fdeltaY);
    void ExecuteMoveQueue();
    Troop();
    Troop(int fx,int fy);
};
enum BuildOrderType{
TroopOrder,
NoneOrder
};
struct BuildOrder{
BuildOrderType orderType;
int x,y;
BuildOrder(){x = y = 0;orderType = NoneOrder;}
BuildOrder(int fx,int fy,BuildOrderType forderType){x = fx,y = fy;orderType = forderType;}
};
struct Base{

    int oil = 0;
    int oldoil = 0;
    char oilGraphics = 'O';
    WORD oilColour = BACKGROUND_INTENSITY;

    int sulphur;
    int oldsulphur;
    char sulphurGraphics = 'S';
    WORD sulphurColour = FOREGROUND_RED  | FOREGROUND_GREEN | FOREGROUND_INTENSITY;

    int iron;
    int oldiron;
    char ironGraphics = 'I';
    WORD ironColour = FOREGROUND_INTENSITY;

    int infantryOilCost = 1;
    int infantrySulphurCost = 5;
    int infantryIronCost = 2;
    char infantryUnitRepairGraphics = '+';
    WORD infantryUnitRepairColour = BACKGROUND_RED;
    int x = -1,y = -1;
    int resourceSearchSideLength = 10;
    int troopBuildingSideLength = 3;
    WORD colour = BACKGROUND_BLUE | BACKGROUND_GREEN ;
    char graphics = 'B';
    Troop* baseUnits[50];

    int numBuildOrders;
    int maxBuildOrders;
    BuildOrder* buildQueue;

    CHAR_INFO* forbiddens;
    int numForbiddens = 2;
    int amountUnits = 0;

    bool selected = false;
    Base();
    Base(int fx,int fy,int fresourcesideLength,int funitSidelength);
    Base(int fx,int fy);
    void Activate();
    void Deactivate();
    void InitBase();
    void CountResources(CHAR_INFO* worldmap,int maxMapX,int maxMapY);
   void CreateInfantry(int dx,int dy,Troop* troops,int& numTroops,int maxMapX,int maxMapY,CHAR_INFO* worldmap,char seaChar,WORD seaAttr);
    void AddTroopToBuildQueue(int dx,int dy);
   void ExecuteBuildQueue(Troop* troops,int& numTroops,int maxMapX,int maxMapY,char seaChar,WORD seaAttr,CHAR_INFO* worldmap);
    void ShowBuildQueue(CHAR_INFO* levelmap,int maxLX,int maxLY,int fdeltaX,int fdeltaY);
    void Unselected(CHAR_INFO* levelmap,int maxMapX,int maxMapY,int deltaX,int deltaY);
    void Selected(CHAR_INFO* levelmap,int maxLX,int maxLY,int deltaX,int deltaY);
};
void TroopUpdate(Troop* troops,int numTroops,int deltaX,int deltaY,int maxLevelX,int maxLevelY,int maxMapX,int maxMapY,int movePoints,CHAR_INFO* levelmap,CHAR_INFO* worldmap,Troop* selectedTroop);
void BaseUpdate(Base* bases,int numBases,int deltaX,int deltaY,int maxLevelX,int maxLevelY,int maxMapX,int maxMapY,int movePoints,CHAR_INFO* levelmap,CHAR_INFO* worldmap,Base* selectedBase);
#include "WorldBasicFunctions.cpp"
#include "Bases.cpp"
#include "Troops.cpp"

#endif // WORLDBASICFUNCTIONS_H_INCLUDED

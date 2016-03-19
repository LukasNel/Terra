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

#include "Troops.h"
#include "Infantry.h"
#include "Tanks.h"

void InfantryUpdate(Infantry* infantries,int numInfantry,Tank* tanks,int numTanks,int deltaX,int deltaY,int maxLevelX,int maxLevelY,int maxMapX,int maxMapY,int movePoints,CHAR_INFO* levelmap,CHAR_INFO* worldmap,char seachar,Infantry* selectedInfantry,Tank* selectedTank);
void BaseUpdate(Base* bases,int numBases,int deltaX,int deltaY,int maxLevelX,int maxLevelY,int maxMapX,int maxMapY,int movePoints,CHAR_INFO* levelmap,CHAR_INFO* worldmap,Base* selectedBase);
#include "WorldBasicFunctions.cpp"
#include "Bases.cpp"
#include "Troops.cpp"

#endif // WORLDBASICFUNCTIONS_H_INCLUDED

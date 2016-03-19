#ifndef INFANTRY_H_INCLUDED
#define INFANTRY_H_INCLUDED
#include "Bases.h"
struct Infantry : public Troop{
    void InitTroop();
    Infantry(){
        InitTroop();
        }
    Infantry(int fx,int fy){
        InitTroop();
        x = fx;
        y = fy;
    }
    void BuildBase(Base* bases,int& numBases,int& gOil,int& gIron,int& gSulphur,CHAR_INFO* worldmap,int maxX,int maxY);

};
#include "Infantry.cpp"
#endif // INFANTRY_H_INCLUDED

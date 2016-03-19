#ifndef TERRAINGENERATION_H_INCLUDED
#define TERRAINGENERATION_H_INCLUDED
#include "wincon.h"
enum Direction{
    Left,
    Right,
    Up,
    Down
};
struct HeightMap{
    char** heightmap;
    long height;
    long width;
    char* operator[](long faccess){
    return heightmap[faccess];
    }
};
HeightMap SquareDiamondTerrainGeneration(long num,unsigned char roughness,long initValue);
HeightMap SquareDiamondTerrainGeneration(long num,unsigned char roughness,long initValue,CHAR_INFO* worldmap,int maxX,int maxY,Direction dir,int ex, int ey,int sx,int sy);
inline char sample(long fx,long fy,char** fvalues,long fheight,long fwidth);
inline void setSample(long fx,long fy,char** fvalues,char fvalue,long fheight,long fwidth);
inline void Square(long fx,long fy,long fsize,char** fvalues,long fheight,long fwidth,unsigned char roughness);
inline void Diamond(long fx,long fy,long fsize,char** fvalues,long fheight,long fwidth,unsigned char roughness);
#include "TerrainGeneration.cpp"
#endif // TERRAINGENERATION_H_INCLUDED

#include "TerrainGeneration.h"
#define OffsetM(rough) ((rand()%rough)-(rough/2))
#define Offset(rough) ((rough > 0) ? OffsetM(rough) : 1)
inline long pow2(long n)
{
    long outOfpow2 = 1;
    for(long i = 0;i<n;i++)outOfpow2*=2;
    return outOfpow2;
}

inline char sample(long fx,long fy,char** fvalues,long fheight,long fwidth){
    long aA = (fx %fwidth) + (fy %fheight)*fwidth;
     int fcy = (fy >= 0) ? fy%fheight : abs(fheight+fy)%fheight;
    int fcx = (fx >= 0) ? fx%fwidth  : abs(fwidth+fx)%fwidth;
    return fvalues[fcx][fcy];
}
inline void setSample(long fx,long fy,char** fvalues,char fvalue,long fheight,long fwidth){
     int fcy = (fy >= 0) ? fy%fheight : abs(fheight+fy)%fheight;
    int fcx = (fx >= 0) ? fx%fwidth  : abs(fwidth+fx)%fwidth;
    fvalues[fcx][(fcy)] = fvalue;
}
inline void Diamond(long fx,long fy,long halfstep,char** fvalues,long fheight,long fwidth,unsigned char roughness){

    char a = sample(fx - halfstep,fy           ,fvalues,fheight,fwidth);
    char b = sample(fx + halfstep,fy           ,fvalues,fheight,fwidth);
    char c = sample(fx,           fy - halfstep,fvalues,fheight,fwidth);
    char d = sample(fx,           fy + halfstep,fvalues,fheight,fwidth);
    setSample(fx,fy,fvalues,(a+b+c+d)/4 + Offset(roughness),fheight,fwidth);

}
inline void Square(long fx,long fy,long halfstep,char** fvalues,long fheight,long fwidth,unsigned char roughness){

    char a = sample(fx - halfstep,fy - halfstep,fvalues,fheight,fwidth);
    char b = sample(fx + halfstep,fy + halfstep,fvalues,fheight,fwidth);
    char c = sample(fx + halfstep,fy - halfstep,fvalues,fheight,fwidth);
    char d = sample(fx - halfstep,fy + halfstep,fvalues,fheight,fwidth);

    setSample(fx,fy,fvalues,(a+b+c+d)/4 + Offset(roughness),fheight,fwidth);
}
HeightMap SquareDiamondTerrainGeneration(long num,unsigned char roughness,long initValue){

    long sideLength = pow2(num)+1;
    long width = sideLength;
    long height = sideLength;
    //Initialise squareArray
    char** mdiArray = new char*[width];
    for(int i = 0;i<width;i++)mdiArray[i] = new char[height];
    for(int i = 0;i<height;i++)
        for(int j = 0;j<width;j++)mdiArray[j][i] = 0;

    setSample(0,      0,       mdiArray,initValue,height,width);
    setSample(0,      height-1,mdiArray,initValue,height,width);
    setSample(width-1,0,       mdiArray,initValue,height,width);
    setSample(width-1,height-1,mdiArray,initValue,height,width);
    long halfstep = sideLength / 2;
    while(sideLength > 1){
        for (long y = halfstep; y < height + halfstep; y += sideLength)
        {
            for (long x = halfstep; x < width + halfstep; x += sideLength)
            {
                Square(x,y,halfstep,mdiArray,height,width,roughness);
               // setSample(x,y,mdiArray,(a+b+c+d)/4 + Offset(roughness),height,width);

            }
        }

        for (long y = 0; y < height; y += sideLength)
        {
            for (long x = 0; x < width; x += sideLength)
            {
                Diamond(x + halfstep, y, halfstep,mdiArray,height,width,roughness);
                Diamond(x, y + halfstep, halfstep,mdiArray,height,width,roughness);
            }
        }
        sideLength /= 2;
        halfstep    = sideLength/2;
        roughness  -= roughness/5;

     /*for(int i = 0;i<height;i++){
        for(int j = 0;j<width;j++){
              char outC = mdiArray[j][i] +127;
                if(outC != 0){
                    fprintf(f,"%c",(outC != '\n') ? outC : 'C');
                }else fputc((int)'X',f);
        }
            fprintf(f,"%c",'\n');
        }
    fprintf(f,"%c",'\n');*/


    }
    HeightMap outHM;
    outHM.heightmap = mdiArray;
    outHM.width = width;
    outHM.height = height;

    return outHM;
}
HeightMap SquareDiamondTerrainGeneration(long num,unsigned char roughness,long initValue,CHAR_INFO* worldmap,int maxX,int maxY,Direction dir,int ex, int ey,int sx,int sy){
    long sideLength = pow2(num)+1;
    long width = sideLength;
    long height = sideLength;
    //Initialise squareArray
    char** mdiArray = new char*[width];
    for(int i = 0;i<width;i++)mdiArray[i] = new char[height];
    for(int i = 0;i<height;i++)
        for(int j = 0;j<width;j++)mdiArray[j][i] = 0;

    setSample(0,      0,       mdiArray,initValue,height,width);
    setSample(0,      height-1,mdiArray,initValue,height,width);
    setSample(width-1,0,       mdiArray,initValue,height,width);
    setSample(width-1,height-1,mdiArray,initValue,height,width);
    switch(dir){
        case Right:
            for(int i = sy;i < ey;i++)
                setSample(0,0,mdiArray,worldmap[maxX-1+i*maxX].Char.AsciiChar,height,width);
            break;
        case Left:
            for(int i = sy;i < ey;i++)
                setSample(0,0,mdiArray,worldmap[i*maxX].Char.AsciiChar,height,width);
            break;
        case Up:
            for(int i = sy;i < ey;i++)
                setSample(0,0,mdiArray,worldmap[i].Char.AsciiChar,height,width);
            break;
        case Down:
            for(int i = sy;i < ey;i++)
                setSample(0,0,mdiArray,worldmap[i+maxY*maxX-maxX].Char.AsciiChar,height,width);
            break;

    }
    long halfstep = sideLength / 2;
    while(sideLength > 1){
        for (long y = halfstep; y < height + halfstep; y += sideLength)
        {
            for (long x = halfstep; x < width + halfstep; x += sideLength)
            {
                Square(x,y,halfstep,mdiArray,height,width,roughness);
               // setSample(x,y,mdiArray,(a+b+c+d)/4 + Offset(roughness),height,width);

            }
        }

        for (long y = 0; y < height; y += sideLength)
        {
            for (long x = 0; x < width; x += sideLength)
            {
                Diamond(x + halfstep, y, halfstep,mdiArray,height,width,roughness);
                Diamond(x, y + halfstep, halfstep,mdiArray,height,width,roughness);
            }
        }
        sideLength /= 2;
        halfstep    = sideLength/2;
        roughness  -= roughness/5;

     HeightMap outHM;
    outHM.heightmap = mdiArray;
    outHM.width = width;
    outHM.height = height;

    return outHM;


    }



}


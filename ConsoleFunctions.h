#ifndef CONSOLEFUNCTIONS_H_INCLUDED
#define CONSOLEFUNCTIONS_H_INCLUDED
#include <windows.h>
CONSOLE_SCREEN_BUFFER_INFO screenInfo();
void ClearScreen();
void SetConsoleBufferL(CHAR_INFO* fconsolebuffer,int fwidth,int fheight,int startX,int startY);
int Integer_Length(int num);
void WriteNum(CHAR_INFO* fwriteArea,int num,int fx,int fy,int lenOfXBuffer);
void WriteNumA(CHAR_INFO* fwriteArea,int num,WORD fattr,int fx,int fy,int lenOfXBuffer);

void WriteStr(CHAR_INFO* fwriteArea,char* fstr,int fx,int fy,int lenOfXBuffer);
void WriteStrA(CHAR_INFO* fwriteArea,char* fstr,WORD fattr,int fx,int fy,int lenOfXBuffer);

inline void WriteChar(CHAR_INFO* fwriteArea,char fchar,int fx,int fy,int lenOfXBuffer);
inline void WriteCharA(CHAR_INFO* fwriteArea,char fchar,WORD fattributes,int fx,int fy,int lenOfXBuffer);

void WriteStrNum(CHAR_INFO* fwriteArea,char* fstr,int num,int fx,int fy,int lenOfXBuffer);
void WriteStrNumA(CHAR_INFO* fwriteArea,char* fstr,int num,WORD fattr1,WORD fattr2,int fx,int fy,int lenOfXBuffer);

void ClearBuffer(CHAR_INFO* fwriteArea,int maxDrawAreaX,int maxDrawAreaY);

struct Key{
    int vkey = 0;
    Key(){}
    Key(int fvk);
    void SetKey(int fvk);
    bool CheckKey();
    private:
        bool isDown = false;
};
#include "ConsoleFunctions.cpp"
#endif // CONSOLEFUNCTIONS_H_INCLUDED

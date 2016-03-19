#include "ConsoleFunctions.h"
CONSOLE_SCREEN_BUFFER_INFO screenInfo(){
     HANDLE                     hStdOut;
     hStdOut = GetStdHandle( STD_OUTPUT_HANDLE );
CONSOLE_SCREEN_BUFFER_INFO csbi;
 if (!GetConsoleScreenBufferInfo( hStdOut, &csbi ));
 return csbi;

}

void ClearScreen(){
  HANDLE                     hStdOut;
  CONSOLE_SCREEN_BUFFER_INFO csbi;
  DWORD                      counter;
  DWORD                      cellCount;
  COORD                      homeCoords = { 0, 0 };

  hStdOut = GetStdHandle( STD_OUTPUT_HANDLE );
  if (hStdOut == INVALID_HANDLE_VALUE) return;

  /* Get the number of cells in the current buffer */
  if (!GetConsoleScreenBufferInfo( hStdOut, &csbi )) return;
  cellCount = csbi.dwSize.X * csbi.dwSize.Y;

  /* Fill the entire buffer with spaces */
  if (!FillConsoleOutputCharacter(
    hStdOut,
    (TCHAR) ' ',
    cellCount,
    homeCoords,
    &counter
    )) return;

  /* Fill the entire buffer with the current colors and attributes */
  if (!FillConsoleOutputAttribute(
    hStdOut,
    csbi.wAttributes,
    cellCount,
    homeCoords,
    &counter
    )) return;

  /* Move the cursor home */
  SetConsoleCursorPosition( hStdOut, homeCoords );
  }
void SetConsoleBufferL(CHAR_INFO* fconsolebuffer,int fwidth,int fheight,int startX,int startY){
  HANDLE                     hStdOut = GetStdHandle( STD_OUTPUT_HANDLE );
  CONSOLE_SCREEN_BUFFER_INFO csbi;
  DWORD                      counter;
  DWORD                      cellCount;
  COORD                      homeCoords = { 0, 0 };
  COORD                      dwBufferSize = {fwidth,fheight};

  SMALL_RECT                 WriteRegion ;

  if (hStdOut == INVALID_HANDLE_VALUE) return;

  /* Get the number of cells in the current buffer */
  if (!GetConsoleScreenBufferInfo( hStdOut, &csbi )) return;
    cellCount = csbi.dwSize.X * csbi.dwSize.Y;
    WriteRegion.Right = csbi.srWindow.Right ;
    WriteRegion.Bottom  = csbi.srWindow.Bottom;
    WriteRegion.Top  = 0;
    WriteRegion.Left  = 0;

  /* Fill the entire buffer with spaces */
  if (!WriteConsoleOutput(
            hStdOut,
            fconsolebuffer,
            dwBufferSize,
            homeCoords,
            &WriteRegion
    ))
    return;

  /* Move the cursor home */
  SetConsoleCursorPosition( hStdOut, homeCoords );
  }

int win_bool = 0;

 int Integer_Length(int num){
    int fintlen = 0;
    int y = num;
    do{
        fintlen++;
        y/=10;
    }while( y > 0);
    return (num != 0) ? fintlen : 1;
};
void WriteNum(CHAR_INFO* fwriteArea,int num,int fx,int fy,int lenOfXBuffer){
    if(num == 0){fwriteArea[fy*lenOfXBuffer + fx].Char.AsciiChar = '0';return;}
    int PosX = fx;
    if(num < 0) fwriteArea[fy*lenOfXBuffer + PosX++].Char.AsciiChar = '-';
    int y = abs(num);
    int intlenl = Integer_Length(num);
    int intlenf = intlenl;
    char* strOut = (char*)malloc(sizeof(int)*intlenf);

   // for(int i =0;i<intlenf;i++)strOut[i] = ' ';
    while(intlenl > 0){
        strOut[--intlenl] = (char)((y%10) + 48);
        y /= 10;
    };
    for(int i=0;i<intlenf;i++){
        fwriteArea[fy*lenOfXBuffer + PosX + i].Char.AsciiChar = strOut[i];
        fwriteArea[fy*lenOfXBuffer + PosX + i].Attributes = 0x000F;
    }
    free(strOut);

}
void WriteNumA(CHAR_INFO* fwriteArea,int num,WORD fattr,int fx,int fy,int lenOfXBuffer){
    if(num == 0){fwriteArea[fy*lenOfXBuffer + fx].Char.AsciiChar = '0';fwriteArea[fy*lenOfXBuffer + fx].Attributes = fattr;return;}
    int PosX = fx;
    if(num < 0) fwriteArea[fy*lenOfXBuffer + PosX++].Char.AsciiChar = '-';
    int y = abs(num);
    int intlenl = Integer_Length(num);
    int intlenf = intlenl;
    char* strOut = (char*)malloc(sizeof(int)*intlenf);

   // for(int i =0;i<intlenf;i++)strOut[i] = ' ';
    while(intlenl > 0){
        strOut[--intlenl] = (char)((y%10) + 48);
        y /= 10;
    };
    for(int i=0;i<intlenf;i++){
        fwriteArea[fy*lenOfXBuffer + PosX + i].Char.AsciiChar = strOut[i];
        fwriteArea[fy*lenOfXBuffer + PosX + i].Attributes = fattr;
    }
    free(strOut);

}
void WriteStr(CHAR_INFO* fwriteArea,char* fstr,int fx,int fy,int lenOfXBuffer){
    int fcounter = fx;
    while(fwriteArea[fy*lenOfXBuffer + fcounter++].Char.AsciiChar = *(fstr++)) fwriteArea[fy*lenOfXBuffer + fcounter-1].Attributes = 0x000F;
}
void WriteStrA(CHAR_INFO* fwriteArea,char* fstr,WORD fattr,int fx,int fy,int lenOfXBuffer){
    int fcounter = fx;
    while(fwriteArea[fy*lenOfXBuffer + fcounter++].Char.AsciiChar = *(fstr++)) fwriteArea[fy*lenOfXBuffer + fcounter-1].Attributes = fattr;
}
inline void WriteChar(CHAR_INFO* fwriteArea,char fchar,int fx,int fy,int lenOfXBuffer){
    fwriteArea[fy*lenOfXBuffer + fx].Char.AsciiChar = fchar;
    fwriteArea[fy*lenOfXBuffer + fx].Attributes = 0x000F;
}
inline void WriteCharA(CHAR_INFO* fwriteArea,char fchar,WORD fattributes,int fx,int fy,int lenOfXBuffer){
    fwriteArea[fy*lenOfXBuffer + fx].Char.AsciiChar = fchar;
    fwriteArea[fy*lenOfXBuffer + fx].Attributes = fattributes;

}
void WriteStrNum(CHAR_INFO* fwriteArea,char* fstr,int num,int fx,int fy,int lenOfXBuffer){
        WriteStr(fwriteArea,fstr,fx,fy,lenOfXBuffer);
        WriteNum(fwriteArea,num,fx + strlen(fstr) + 1,fy,lenOfXBuffer);

}
void WriteStrNumA(CHAR_INFO* fwriteArea,char* fstr,int num,WORD fattr1,WORD fattr2,int fx,int fy,int lenOfXBuffer){
        WriteStrA(fwriteArea,fstr,fattr1,fx,fy,lenOfXBuffer);
        WriteNumA(fwriteArea,num,fattr2,fx + strlen(fstr) + 1,fy,lenOfXBuffer);

}
void ClearBuffer(CHAR_INFO* fwriteArea,int maxDrawAreaX,int maxDrawAreaY){

    for(int i =0;i < maxDrawAreaY;i++)
            for(int j =0;j < maxDrawAreaX;j++){
                fwriteArea[i*maxDrawAreaX + j].Char.AsciiChar = ' ';fwriteArea[i*maxDrawAreaX + j].Attributes = 15;};
}


    Key::Key(int fvk){
        vkey = fvk;
    }
    void Key::SetKey(int fvk){
        vkey = fvk;
    }
    bool Key::CheckKey(){
        if((GetAsyncKeyState(vkey) & 0x8000) && !isDown){
            isDown = true;
            return 1;
        }
        isDown = (GetAsyncKeyState(vkey) & 0x8000) ? 1 : 0;
        return 0;

    }

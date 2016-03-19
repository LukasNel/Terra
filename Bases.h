#ifndef BASES_H_INCLUDED
#define BASES_H_INCLUDED
enum BuildOrderType{
    InfantryOrder,
    TankOrder,
    NoneOrder
};
struct BuildOrder{
    BuildOrderType orderType;
    int x,y;
    BuildOrder(){x = y = 0;orderType = NoneOrder;}
    BuildOrder(int fx,int fy,BuildOrderType forderType){x = fx,y = fy;orderType = forderType;}
};
struct Infantry;
struct Tank;
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

    int infantryOilCost = 8;
    int infantrySulphurCost = 10;
    int infantryIronCost = 4;
    char infantryUnitRepairGraphics = '+';
    WORD infantryUnitRepairColour = BACKGROUND_RED;

    int tankOilCost = 15;
    int tankSulphurCost = 10;
    int tankIronCost = 9;
    char tankUnitRepairGraphics = '-';
    WORD tankUnitRepairColour = BACKGROUND_RED | BACKGROUND_INTENSITY;

    int x = -1,y = -1;
    int resourceSearchSideLength = 10;
    int troopBuildingSideLength = 3;
    WORD colour = BACKGROUND_BLUE | BACKGROUND_GREEN ;
    char graphics = 'B';
    Infantry* baseUnits[50];
    Tank* baseTanks[50];

    int numBuildOrders;
    int maxBuildOrders;
    BuildOrder* buildQueue;

    CHAR_INFO* forbiddens;
    int numForbiddens = 2;
    int amountUnits = 0;
    int amountTanks = 0;

    bool selected = false;
    Base();
    Base(int fx,int fy,int fresourcesideLength,int funitSidelength);
    Base(int fx,int fy);
    void InitBase();

    void Activate();
    void Deactivate();


    void CountResources(CHAR_INFO* worldmap,int maxMapX,int maxMapY);

    void CreateInfantry(int dx,int dy,Infantry* infantries,int& numInfantry,int maxMapX,int maxMapY,CHAR_INFO* worldmap,char seaChar,WORD seaAttr);
    void CreateTank(int dx,int dy,Tank* tanks,int& numTanks,int maxMapX,int maxMapY,CHAR_INFO* worldmap,char seaChar,WORD seaAttr);

    void AddTroopToBuildQueue(int dx,int dy,BuildOrderType bType);
    void ExecuteBuildQueue(Infantry* troops,int& numInfantry,Tank* tanks,int& numTanks,int maxMapX,int maxMapY,char seaChar,WORD seaAttr,CHAR_INFO* worldmap);
    void ShowBuildQueue(CHAR_INFO* levelmap,int maxLX,int maxLY,int fdeltaX,int fdeltaY);

    void Unselected(CHAR_INFO* levelmap,int maxMapX,int maxMapY,int deltaX,int deltaY);
    void Selected(CHAR_INFO* levelmap,int maxLX,int maxLY,int deltaX,int deltaY);
};


#endif // BASES_H_INCLUDED

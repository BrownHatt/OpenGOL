#pragma once
#include <iostream>
#include <vector>
#include <math.h>
using namespace std;

class GameOfLifeClass
{

    

public:
    std::vector<int> cellsToUpdate;
    std::vector<int> cellsDied;
    std::vector<int> cellsBornSurvive;

    int golID;

    int sum;

    short* cellArea;


    int8_t* rules;

    int32_t* debuginfoarray;

    int cell=0;
    int width=0;
    int height=0;
    int areaSize=0;

    int cellNeighbours[8] = { 0,0,0,0,0,0,0,0 };
    int cellNeighbours2[8] = { 0,0,0,0,0,0,0,0 };


    int toUpdateCounter = 0;
    int nbCounter = 0;

    int db_cellsdied=0;
    int db_cellsbornsurv=0;
    int db_totalnbsum=0;

    bool running = true;

    GameOfLifeClass(int id, int pwidth, int pheight, int psize, short* area, int8_t* prules, int32_t* pdebugarray);

    GameOfLifeClass() = default;

    ~GameOfLifeClass();
    // The Heavy Work

    void operator()(int x);

    void updateDebugInfo(int nbTotalSum, int loopsDone);


    int getNeighboursAndSum(int* neighbours, short* cellArea, int cell, int width, int height);

    int checkCellDied(int cell, short* cellArea, int sumNb);
    void checkAndAddDied(vector<int>& diedPool, int cell);

    void checkAndAddActive(vector<int>& activePool, int cell);

    void updateCellArea(short* cellArea, vector<int>& cellsDied, vector<int>& cellsBornSurvive, vector<int>& cellsToUpdate);

    void runGeneration();

    int runGenerationStep();


    // Init

    bool addParameters(int pwidth, int pheight, int psize, short* area, int8_t* prules, int32_t* debugarray);
    bool Init();
    void addToUpdate(int cell);



    // Debug Output Stuff //
    bool alive(int nr);

    int populationCount();


    int8_t ruleIs(int rulenr);

    int getUpdateList();

    int sizeUpdateList();

    int getNeighboursAndSumPy(int cell);

    int getNeighbourPy();

};


#include "classes/GameOfLifeClass.h"
#include <chrono>
#include <thread>

GameOfLifeClass::GameOfLifeClass(int id, int pwidth, int pheight, int psize, short* area, int8_t* prules, int32_t* pdebugarray) {
    cell = 0;
    golID = id;
    width = pwidth;
    height = pheight;
    areaSize = psize;

    cellArea = area;
    rules = prules;

    db_cellsdied = 0;
    db_cellsbornsurv = 0;
    db_totalnbsum = 0;

    debuginfoarray = pdebugarray;

    Init();
}

GameOfLifeClass::~GameOfLifeClass() {
    cellsBornSurvive.~vector();
    cellsToUpdate.~vector();
    cellsDied.~vector();

}
// The Heavy Work

void GameOfLifeClass::operator()(int x) {
    cout << "running" << running << endl;
        auto start = chrono::steady_clock::now();
        runGeneration(); 
        auto end = chrono::steady_clock::now();
        std::chrono::duration<double> calctime = end - start;
        cout << golID << " - calc time:"  <<  calctime.count() << endl;

        this_thread::sleep_for(chrono::seconds(4));
     cout << "running" << running << endl;
        cout << golID << "stopped!" << endl;
    return;
}

void GameOfLifeClass::updateDebugInfo(int nbTotalSum, int loopsDone) {
    debuginfoarray[0] = cellsDied.size();
    debuginfoarray[1] = cellsBornSurvive.size();
    debuginfoarray[2] = nbTotalSum;
    debuginfoarray[3] = loopsDone;
}


int GameOfLifeClass::getNeighboursAndSum(int* neighbours, short* cellArea, int cell, int width, int height) {

    int posx = cell % width;
    int posy = floor(cell / width);

    int sum = 0;

    int east = (posx < width - 1) ? posx + 1 : 0;
    int west = posx > 0 ? posx - 1 : posx + width - 1;
    int north = posy > 0 ? posy - 1 : height - 1;
    int south = posy < height - 1 ? posy + 1 : 0;

    *neighbours = east + posy * width;
    sum += cellArea[*neighbours];
    neighbours++;

    *neighbours = west + posy * width;
    sum += cellArea[*neighbours];
    neighbours++;

    *neighbours = posx + north * width;
    sum += cellArea[*neighbours];
    neighbours++;

    *neighbours = posx + south * width;
    sum += cellArea[*neighbours];
    neighbours++;

    *neighbours = east + north * width;
    sum += cellArea[*neighbours];
    neighbours++;

    *neighbours = west + north * width;
    sum += cellArea[*neighbours];
    neighbours++;

    *neighbours = east + south * width;
    sum += cellArea[*neighbours];
    neighbours++;

    *neighbours = west + south * width;
    sum += cellArea[*neighbours];

    return sum;
}

int GameOfLifeClass::checkCellDied(int cell, short* cellArea, int sumNb) {

    int* cellAreaposNb;

    short cellValue = *(cellArea + cell);


    if (cellValue == 1)
        if (sumNb<rules[0] or sumNb>rules[1])
            return 1;
        else
            return 0;
    else
        if (sumNb<rules[2] or sumNb>rules[3])
            return 3;
        else
            return 2;
}

void GameOfLifeClass::checkAndAddDied(vector<int>& diedPool, int cell) {
    for (int i = diedPool.size() - 1; i >= 0; i--) {
        if (diedPool[i] == cell)
            return;
    }
    diedPool.push_back(cell);
}

void GameOfLifeClass::checkAndAddActive(vector<int>& activePool, int cell) {
    for (int i = activePool.size() - 1; i >= 0; i--) {
        if (activePool[i] == cell)
            return;
    }
    activePool.push_back(cell);
}

void GameOfLifeClass::updateCellArea(short* cellArea, vector<int>& cellsDied, vector<int>& cellsBornSurvive, vector<int>& cellsToUpdate) {
    int cbd;
    int ci = 0;
    while (ci < cellsDied.size()) {
        cbd = cellsDied[ci];
        cellArea[cbd] = 0;
        ci++;
    }
    int cbs;
    ci = 0;
    while (ci < cellsBornSurvive.size()) {
        cbs = cellsBornSurvive[ci];
        cellArea[cbs] = 1;
        cellsToUpdate.push_back(cbs);
        ci++;
    }
}

void GameOfLifeClass::runGeneration() {
    int cell;
    int nbSum = 0;
    int resultCheck;
    int nbTotalSum = 0;
    int loopsDone = 0;
    cellsBornSurvive.clear();
    cellsDied.clear();

    while (cellsToUpdate.size() != 0) {

        loopsDone++;
        cell = cellsToUpdate.back(); cellsToUpdate.pop_back();

        nbSum = getNeighboursAndSum(cellNeighbours, cellArea, cell, width, height);
        nbTotalSum += nbSum;

        resultCheck = checkCellDied(cell, cellArea, nbSum);

        if (resultCheck == 0 or resultCheck == 2)
            checkAndAddActive(cellsBornSurvive, cell);
        if (resultCheck == 1)
            checkAndAddDied(cellsDied, cell);

        for (int i = 0; i < 8; i++) {
            loopsDone++;
            nbSum = getNeighboursAndSum(cellNeighbours2, cellArea, cellNeighbours[i], width, height);
            nbTotalSum += nbSum;
            resultCheck = checkCellDied(cellNeighbours[i], cellArea, nbSum);
            if (resultCheck == 0 or resultCheck == 2)
                checkAndAddActive(cellsBornSurvive, cellNeighbours[i]);
            if (resultCheck == 1)
                checkAndAddDied(cellsDied, cellNeighbours[i]);
        }
    }

    updateDebugInfo(nbTotalSum, loopsDone);

    updateCellArea(cellArea, cellsDied, cellsBornSurvive, cellsToUpdate);
}

int GameOfLifeClass::runGenerationStep() {
    int cell;
    int nbSum = 0;
    int resultCheck;
    int nbTotalSum = 0;
    int loopsDone = 0;
    if (cellsToUpdate.size() == 0) {
        updateCellArea(cellArea, cellsDied, cellsBornSurvive, cellsToUpdate);
        return -1;
    }
    loopsDone++;

    cell = cellsToUpdate.back(); cellsToUpdate.pop_back();

    // if (cellArea[cell]==1 and cellsBornSurvive.size()>floor(areaSize/100)
    //                     and browseOverDone(cellsBornSurvive,cell,floor (cellsBornSurvive.size()/20)))
    //     return 0;

    nbSum = getNeighboursAndSum(cellNeighbours, cellArea, cell, width, height);
    nbTotalSum += nbSum;

    resultCheck = checkCellDied(cell, cellArea, nbSum);

    if (resultCheck == 0 or resultCheck == 2)
        checkAndAddActive(cellsBornSurvive, cell);
    if (resultCheck == 1)
        checkAndAddDied(cellsDied, cell);

    for (int i = 0; i < 8; i++) {
        loopsDone++;
        nbSum = getNeighboursAndSum(cellNeighbours2, cellArea, cellNeighbours[i], width, height);
        nbTotalSum += nbSum;
        resultCheck = checkCellDied(cellNeighbours[i], cellArea, nbSum);
        if (resultCheck == 0 or resultCheck == 2)
            checkAndAddActive(cellsBornSurvive, cellNeighbours[i]);
        if (resultCheck == 1)
            checkAndAddDied(cellsDied, cellNeighbours[i]);
    }
    updateDebugInfo(nbTotalSum, loopsDone);
    return 0;
}


// Init

bool GameOfLifeClass::addParameters(int pwidth, int pheight, int psize, short* area, int8_t* prules, int32_t* debugarray) {
    width = pwidth;
    height = pheight;
    areaSize = psize;

    cellArea = area;
    rules = prules;

    debuginfoarray = debugarray;

    return true;
}

bool GameOfLifeClass::Init() {
    int factor = areaSize > 1000 ? 2 : 1;

    cellsToUpdate.reserve(areaSize);
    cellsDied.reserve(areaSize);
    cellsBornSurvive.reserve(areaSize);

    return true;
}

void GameOfLifeClass::addToUpdate(int cell) {
    cellsToUpdate.push_back(cell);
    cout << "I received: " << cell << "size:" << cellsToUpdate.size() << endl;
}




// Debug Output Stuff //
bool GameOfLifeClass::alive(int nr) {
    return nr % 2 == 0;
}

int GameOfLifeClass::populationCount() {
    int popcount = 0;
    cout << "count ctu:" << cellsToUpdate.size() << endl;
    for (int i = 0; i < areaSize; i++) {
        popcount += cellArea[i];
    }

    return popcount;
}


int8_t GameOfLifeClass::ruleIs(int rulenr) {
    return rules[rulenr];
}

int GameOfLifeClass::getUpdateList() {
    if (toUpdateCounter >= cellsToUpdate.size()) {
        toUpdateCounter = 0;
        return -1;
    }
    cell = cellsToUpdate[toUpdateCounter];

    toUpdateCounter++;

    return cell;
}

int GameOfLifeClass::sizeUpdateList() {
    return cellsToUpdate.size();
}

int GameOfLifeClass::getNeighboursAndSumPy(int cell) {
    return getNeighboursAndSum(cellNeighbours, cellArea, cell, width, height);
}

int GameOfLifeClass::getNeighbourPy() {
    if (nbCounter >= 8) {
        nbCounter = 0;
        return -1;
    }
    cell = cellNeighbours[nbCounter];

    nbCounter++;

    return cell;
}
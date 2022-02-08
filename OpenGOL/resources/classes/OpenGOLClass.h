#pragma once
#include <vector>
#include <classes/GameOfLifeClass.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <math.h>
using namespace std;

struct verticeGOL {
    float posx;
    float posy;
    float cell;
    float territory;
};

class OpenGOLClass
{

public:
    GameOfLifeClass golObj;

	vector<float> positionsArea;
	vector<unsigned int> indicesArea;
	float* positionsArray;
	unsigned int* indicesArray;

	unsigned int vertexBuffer;
	unsigned int indexBuffer;

    int floatsPerVertice=4;

	int golID;

	short* cellArea;
    short* cellTerrain;

	int areaWidth;
	int areaSize;

    int8_t rules[8] = { 2,3,3,3,
                        2,3,2,3};
	int32_t debug[4]={0,0,0,0};

    OpenGOLClass(int id, int areaWidthCons) {
        
        golID = id;
        areaWidth = areaWidthCons;
        areaSize = areaWidth * areaWidth;
        cellArea = new short[areaSize];
        cellTerrain = new short[areaSize];
        for (int i = 0; i < areaSize; i++)
        {
            cellArea[i] = 0;
            if (i>floor(areaSize/2)) cellTerrain[i] = 1;
                else cellTerrain[i] = 0;

        }

        cout << "init cell area done - (size) " << areaSize << endl;

        golObj = GameOfLifeClass(golID, areaWidth, areaWidth, areaSize, cellArea, cellTerrain, rules, debug);
        cout << "gol Object created" << endl;
    }
   

    void addPattern(short* pattern, int startx, int starty) {
        cout << "start adding pattern..." << endl;
        int offset = starty * areaWidth + startx;

        int wctr = 0;

        for (int i = 0; i < 25; i++)
        {
            if (wctr == 5) { offset += areaWidth - 5; wctr = 0; }
            //cout << "adding:" << pattern[i];
            cellArea[offset] = pattern[i];
            

            if (cellArea[offset] == 1) golObj.addToUpdate(offset);

            offset += 1;
            wctr += 1;
        }
    }

    void makeCellSquare(float posx, float posy, float cellsizex, float cellsizey, vector<float>& positions, short Active, short terrain) {
        //cout << Active;
        float act = (float)Active;
        //float act = 0.0f;

        positions.push_back(posx);
        positions.push_back(posy - cellsizey);
        positions.push_back(act);
        positions.push_back(terrain);

        positions.push_back(posx + cellsizex);
        positions.push_back(posy - cellsizey);
        positions.push_back(act);
        positions.push_back(terrain);

        positions.push_back(posx + cellsizex);
        positions.push_back(posy);
        positions.push_back(act);
        positions.push_back(terrain);

        positions.push_back(posx);
        positions.push_back(posy);
        positions.push_back(act);
        positions.push_back(terrain);
    }

    void makeSquareIndices(int i, vector<unsigned int>& indices) {
        indices.push_back(i * (floatsPerVertice));
        indices.push_back(i * (floatsPerVertice) + 1);
        indices.push_back(i * (floatsPerVertice) + 2);
        indices.push_back(i * (floatsPerVertice) + 2);
        indices.push_back(i * (floatsPerVertice) + 3);
        indices.push_back(i * (floatsPerVertice));
    }

    void generateCellsRaster(float startx, float starty) {
        float origx = startx;
        float origy = starty;
        float gridwidth = 0.002f;
        //float cellsizex = (double)size * ((double)screenheight / (double)screenwidth);
        float cellsizex = (float)1.2 / (areaWidth * 2);
        float cellsizey = (float)1.2 / (areaWidth * 2);
        vector<float> cell;
        
        for (int i = 0; i < (areaSize); i++)
        {
            makeCellSquare(startx, starty, cellsizex, cellsizey, positionsArea, cellArea[i], cellTerrain[i]);
            makeSquareIndices(i, indicesArea);
            startx += (double)gridwidth + (double)cellsizex;
            if ((i + 1) % areaWidth == 0) {
                starty -= (double)gridwidth + (double)cellsizey;
                startx = origx;
            }
        }

        positionsArray = positionsArea.data();
        indicesArray = indicesArea.data();
       //showmePositions();
    }
    void showmePositions() {
        for (int i = 0; i < positionsArea.size(); i+=floatsPerVertice)
        {
            cout << "position:" << positionsArray[i] << "-" << positionsArray[i + 1];
        }
    }

    void showmeIndices() {
        for (int i = 0; i < indicesArea.size(); i++)
        {
            cout << "position:" << indicesArray[i];
        }
    }

    void setVertexAndIndexBuffer() {
        glGenBuffers(1, &vertexBuffer);
        glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
        glBufferData(GL_ARRAY_BUFFER, 4 * floatsPerVertice * areaSize * sizeof(float), positionsArray, GL_DYNAMIC_DRAW);

        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, floatsPerVertice, GL_FLOAT, GL_FALSE, sizeof(float) * floatsPerVertice, 0);

        glGenBuffers(1, &indexBuffer);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * areaSize * sizeof(unsigned int), indicesArray, GL_STATIC_DRAW);

        cout << "vb:" << vertexBuffer << endl;
        cout << "ib:" << indexBuffer << endl;
    };

    void runGeneration() {
        golObj.runGeneration();
    };

    void refreshBuffer() {
        glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
        glBufferData(GL_ARRAY_BUFFER, 4 * floatsPerVertice * areaSize * sizeof(float), positionsArray, GL_DYNAMIC_DRAW);

        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, floatsPerVertice, GL_FLOAT, GL_FALSE, sizeof(float) * floatsPerVertice, 0);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * areaSize * sizeof(unsigned int), indicesArray, GL_STATIC_DRAW);
    }

    void updateBufferArray() {
        
        for (int i = 0; i < golObj.cellsBornSurvive.size(); i++)
        {
            positionsArray[golObj.cellsBornSurvive[i] * (4 * floatsPerVertice) + (floatsPerVertice)-2] = 1.0f;
            positionsArray[golObj.cellsBornSurvive[i] * (4 * floatsPerVertice) + (2 * floatsPerVertice) - 2] = 1.0f;
            positionsArray[golObj.cellsBornSurvive[i] * (4 * floatsPerVertice) + (3 * floatsPerVertice) - 2] = 1.0f;
            positionsArray[golObj.cellsBornSurvive[i] * (4 * floatsPerVertice) + (4 * floatsPerVertice) - 2] = 1.0f;
        }

        for (int i = 0; i < golObj.cellsDied.size(); i++)
        {
            positionsArray[(golObj.cellsDied[i] * (4 * floatsPerVertice)) + (floatsPerVertice) - 2] = 0.0f;
            positionsArray[(golObj.cellsDied[i] * (4 * floatsPerVertice)) + (2 * floatsPerVertice) - 2] = 0.0f;
            positionsArray[(golObj.cellsDied[i] * (4 * floatsPerVertice)) + (3 * floatsPerVertice) - 2] = 0.0f;
            positionsArray[(golObj.cellsDied[i] * (4 * floatsPerVertice)) + (4 * floatsPerVertice) - 2] = 0.0f;

        }
    }

    void showArea() {
        int xpos = 0;
        for (int i = 0; i < areaSize; i++)
        {
            cout << cellArea[i];
            xpos++;
            if (xpos == areaWidth) {
                cout << endl;
                xpos = 0;
            }
        }
    }
};


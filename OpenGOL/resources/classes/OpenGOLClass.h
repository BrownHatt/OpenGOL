#pragma once
#include <vector>
#include <classes/GameOfLifeClass.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
using namespace std;

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

	int golID;

	short* cellArea;

	int areaWidth;
	int areaSize;

	int8_t rules[4]={2,3,3,3};
	int32_t debug[4]={0,0,0,0};

    OpenGOLClass(int id, int areaWidthCons) {
        golID = id;
        areaWidth = areaWidthCons;
        areaSize = areaWidth * areaWidth;
        cellArea = new short[areaSize];
        for (int i = 0; i < areaSize; i++)
            cellArea[i] = 0;
        cout << "init cell area done - (size) " << areaSize << endl;

        golObj = GameOfLifeClass(golID, areaWidth, areaWidth, areaSize, cellArea, rules, debug);
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

    void makeCellSquare(float posx, float posy, float cellsizex, float cellsizey, vector<float>& positions, short Active) {
        //cout << Active;
        float act = (float)Active;
        //float act = 0.0f;

        positions.push_back(posx);
        positions.push_back(posy - cellsizey);
        positions.push_back(act);

        positions.push_back(posx + cellsizex);
        positions.push_back(posy - cellsizey);
        positions.push_back(act);

        positions.push_back(posx + cellsizex);
        positions.push_back(posy);
        positions.push_back(act);

        positions.push_back(posx);
        positions.push_back(posy);
        positions.push_back(act);

    }

    void makeSquareIndices(int i, vector<unsigned int>& indices) {
        indices.push_back(i * 4);
        indices.push_back(i * 4 + 1);
        indices.push_back(i * 4 + 2);
        indices.push_back(i * 4 + 2);
        indices.push_back(i * 4 + 3);
        indices.push_back(i * 4);
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
            makeCellSquare(startx, starty, cellsizex, cellsizey, positionsArea, cellArea[i]);
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
        for (int i = 0; i < positionsArea.size(); i+=3)
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
        glBufferData(GL_ARRAY_BUFFER, 12 * areaSize * sizeof(float), positionsArray, GL_DYNAMIC_DRAW);

        glEnableVertexAttribArray(golID-1);
        glVertexAttribPointer(golID - 1, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, 0);

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
        glBufferData(GL_ARRAY_BUFFER, 12 * areaSize * sizeof(float), positionsArray, GL_DYNAMIC_DRAW);

        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, 0);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * areaSize * sizeof(unsigned int), indicesArray, GL_STATIC_DRAW);
    }

    void updateBufferArray() {
        for (int i = 0; i < golObj.cellsBornSurvive.size(); i++)
        {
            positionsArray[golObj.cellsBornSurvive[i] * 12 + 2] = 1.0f;
            positionsArray[golObj.cellsBornSurvive[i] * 12 + 5] = 1.0f;
            positionsArray[golObj.cellsBornSurvive[i] * 12 + 8] = 1.0f;
            positionsArray[golObj.cellsBornSurvive[i] * 12 + 11] = 1.0f;
        }

        for (int i = 0; i < golObj.cellsDied.size(); i++)
        {
            positionsArray[(golObj.cellsDied[i] * 12) + 2] = 0.0f;
            positionsArray[(golObj.cellsDied[i] * 12) + 5] = 0.0f;
            positionsArray[(golObj.cellsDied[i] * 12) + 8] = 0.0f;
            positionsArray[(golObj.cellsDied[i] * 12) + 11] = 0.0f;
        }
    }

};


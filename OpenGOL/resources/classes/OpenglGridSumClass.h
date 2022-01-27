#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <classes/GridSumClass.h>
#include <vector>
using namespace std;
class OpenglGridSumClass
{
public:
    GridSumClass gridsumObj;

    vector<float> positionsArea;
    vector<unsigned int> indicesArea;
    float* positionsArray;
    unsigned int* indicesArray;

    unsigned int vertexBuffer;
    unsigned int indexBuffer;

    int gsID;

    short* sumArea;

    int sumWidth;
    int sumSize;

    OpenglGridSumClass(int id, int width) {
        gsID = id;
        sumWidth = width;
        sumSize = width * width;
        sumArea = new short[sumSize];
        for (int i = 0; i < sumSize; i++)
            sumArea[i] = 0;
       
        cout << "init cell area done - (size) " << sumSize << endl;
        gridsumObj = GridSumClass(sumWidth,sumWidth,sumArea);
        cout << "GridSumClass done" << endl;

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
        float cellsizex = (float)1.5 / (sumWidth * 2);
        float cellsizey = (float)1.5 / (sumWidth * 2);
        vector<float> cell;
        cout << "creating cells " << sumSize << ": " << cellsizex << "...";
        for (int i = 0; i < (sumSize); i++)
        {
            cout << ".";
            makeCellSquare(startx, starty, cellsizex, cellsizey, positionsArea, sumArea[i]);
            makeSquareIndices(i, indicesArea);
            startx += (double)gridwidth + (double)cellsizex;
            if ((i + 1) % sumWidth == 0) {
                starty -= (double)gridwidth + (double)cellsizey;
                startx = origx;
            }
        }

        positionsArray = positionsArea.data();
        indicesArray = indicesArea.data();
        //showmePositions();
    }
    void setVertexAndIndexBuffer() {
        glGenBuffers(1, &vertexBuffer);
        glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
        glBufferData(GL_ARRAY_BUFFER, 12 * sumSize * sizeof(float), positionsArray, GL_DYNAMIC_DRAW);

        glEnableVertexAttribArray(gsID - 1);
        glVertexAttribPointer(gsID - 1, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, 0);

        glGenBuffers(1, &indexBuffer);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * sumSize * sizeof(unsigned int), indicesArray, GL_STATIC_DRAW);

        cout << "vb:" << vertexBuffer << endl;
        cout << "ib:" << indexBuffer << endl;
    };
    void refreshBuffer() {
        glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
        glBufferData(GL_ARRAY_BUFFER, 12 * sumSize * sizeof(float), positionsArray, GL_DYNAMIC_DRAW);

        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, 0);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * sumSize * sizeof(unsigned int), indicesArray, GL_STATIC_DRAW);
    }

    void updateBufferArray() {
        for (int i = 0; i < sumSize; i++)
        {
            positionsArray[i * 12 + 2] = (float)sumArea[i];
            positionsArray[i * 12 + 5] = (float)sumArea[i];
            positionsArray[i * 12 + 8] = (float)sumArea[i];
            positionsArray[i * 12 + 11] = (float)sumArea[i];
        }
    }

    void showmePositions() {
        for (int i = 0; i < positionsArea.size(); i += 3)
        {
            cout << "position:" << positionsArray[i] << "-" << positionsArray[i + 1];
        }
    }
};


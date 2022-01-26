#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <thread>
#include "classes/GameOfLifeClass.h"



using namespace std;

struct ShaderProgramSource {
    string VertexSource;
    string FragmentSource;
};

static ShaderProgramSource ParseShader(const string& filepath) {
    ifstream stream(filepath);

    enum class ShaderType {
        NONE = -1 , VT = 0, FM = 1
    };
    string line;
    stringstream ss[2];
    ShaderType type = ShaderType::NONE;

    while (getline(stream, line)) {
        if (line.find("#shader") != string::npos) {
            if (line.find("vertex") != string::npos)
                type = ShaderType::VT;
            else if (line.find("fragment") != string::npos)
                type = ShaderType::FM;

        }
        else {
            ss[(int)type] << line << '\n';
        }
    }

    return { ss[0].str(), ss[1].str()};

}
static unsigned int CompileShader(unsigned int type, const string& source) {
    unsigned int id = glCreateShader(type);
    const char* src = source.c_str();
    glShaderSource(id, 1, &src, nullptr);
    glCompileShader(id);
    return id;

}
static unsigned int CreateShader(const string& vertexshader, const string& fragmentshader) {
    unsigned int program = glCreateProgram();
    unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexshader);
    unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentshader);

    glAttachShader(program, vs);
    glAttachShader(program, fs);
    glLinkProgram(program);
    glValidateProgram(program);

    glDeleteShader(vs);
    glDeleteShader(fs);

    return program;
}

int screenwidth = 1000;
int screenheight = 1000;

void makeCellSquare(float posx, float posy, float cellsizex, float cellsizey, vector<float> &positions) {

    positions.push_back(posx);
    positions.push_back(posy + cellsizey);

    positions.push_back(posx + cellsizex);
    positions.push_back(posy + cellsizey);
    
    positions.push_back(posx + cellsizex);
    positions.push_back(posy);

    positions.push_back(posx);
    positions.push_back(posy);

}

void makeSquareIndices(int i, vector<unsigned int>& indices) {
    indices.push_back(i * 4);
    indices.push_back(i * 4+1);
    indices.push_back(i * 4+2);
    indices.push_back(i * 4+2);
    indices.push_back(i * 4+3);
    indices.push_back(i * 4);
}

void generateCellsRaster(float startx, float starty, int nrsCellsWidth, float size, vector<float> &positions, vector<unsigned int> &indices) {
    float origx = startx;
    float origy = starty;
    float gridwidth = 0.005f;
    float cellsizex = (float)size * ((float)screenheight / (float)screenwidth);
    //float cellsizex = size;
    float cellsizey = size;
    vector<float> cell;
    cout << "creating raster ";
    for (int i = 0; i < (nrsCellsWidth*nrsCellsWidth); i++)
    {
        makeCellSquare(startx, starty, cellsizex, cellsizey, positions);
        makeSquareIndices(i, indices);
        cout << ".";

        startx += (float)gridwidth + (float)cellsizex;
        if ((i+1) % nrsCellsWidth == 0) {
            cout << "next line" << endl;
            starty += (float)gridwidth + (float)cellsizey;
            startx = origx;
        }
    }
    
}

struct cellData {
    float positionx;
    float positiony;
    short active;
};

int main(void)
{
    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;

    

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(screenwidth, screenheight, "Hello World", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    if (glewInit() != GLEW_OK)
        std::cout << "woops!" << std::endl;

    vector<float> positionsArea;
    vector<unsigned int> indicesArea;
    unsigned int areaWidth = 100;
    



    generateCellsRaster(-0.9f, -0.9f, areaWidth, (float)areaWidth/(screenwidth*2), positionsArea, indicesArea);
    float* positionsArray = positionsArea.data();
    unsigned int* indicesArray = indicesArea.data();
        
    
    unsigned int buffer;
    glGenBuffers(1, &buffer);
    glBindBuffer(GL_ARRAY_BUFFER, buffer);
    glBufferData(GL_ARRAY_BUFFER, 4 * areaWidth * areaWidth * 2 * sizeof(float), positionsArray, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, 0);

    unsigned int ibo;
    glGenBuffers(1, &ibo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * areaWidth * areaWidth * sizeof(unsigned int), indicesArray, GL_STATIC_DRAW);
    
    
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    ShaderProgramSource sps = ParseShader("resources/shaders/basic.shader");
    cout << sps.FragmentSource << endl;
    cout << sps.VertexSource << endl;

    unsigned int shader = CreateShader(sps.VertexSource, sps.FragmentSource);

    glUseProgram(shader);

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT);

 /*       glBegin(GL_TRIANGLES);
        glVertex2f(-0.5f, 0.5f);
        glVertex2f(0.5f, 0.5f);
        glVertex2f(0.0f, 0.0f);
        glEnd();*/

        //glDrawArrays(GL_TRIANGLES, 0, 6);

        glDrawElements(GL_TRIANGLES, 6 * areaWidth * areaWidth, GL_UNSIGNED_INT, nullptr);

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    glDeleteProgram(shader);
    glfwTerminate();
    return 0;
}

void addPattern(GameOfLifeClass &gol, short* cellArea, short* pattern, int startx, int starty, int width) {
    int offset = starty * width + startx;

    int wctr = 0;
    
    for (int i = 0; i < 25; i++)
    {
        if (wctr == 5) { offset += width - 5; wctr = 0; }
        cellArea[offset] = pattern[i];

        if (cellArea[offset] == 1) gol.addToUpdate(offset);

        offset += 1;
        wctr += 1;
    }
}

void printCellArea(short* cellArea, int size, int width) {
    int wctr = 0;
    for (int i = 0; i < size; i++)
    {
        cout << cellArea[i];
        wctr += 1;
        if (wctr == width) { cout << endl; wctr = 0; }
    }
}

void printCellsToUpdate(GameOfLifeClass gol) {
    int ctu = gol.getUpdateList();
    while (ctu != -1) {
        cout << ctu << "*";
    }
    cout << endl;

}

int main_gol(void) {
    int width = 50;
    int height = 50;
    int areaSize = width*height;
    short* cellArea = new short[areaSize];
    for (int i = 0; i < areaSize; i++)
            cellArea[i] = 0;

    short* cellArea2 = new short[areaSize];
    for (int i = 0; i < areaSize; i++)
        cellArea2[i] = 0;

    short gliderPattern[25] = {
        0,0,0,0,0,
        0,0,0,1,0,
        0,1,0,1,0,
        0,0,1,1,0,
        0,0,0,0,0
    };
    int8_t rules[4] = { 2,3,3,3 };
    int32_t debugArray[4] = { 0,0,0,0 };
    GameOfLifeClass gol = GameOfLifeClass(1, width,height,areaSize,cellArea,rules,debugArray);    
    addPattern(gol, cellArea, gliderPattern, 1, 1, width);

    int8_t rules2[4] = { 2,3,2,3 };
    int32_t debugArray2[4] = { 0,0,0,0 };
    GameOfLifeClass gol2 = GameOfLifeClass(2, width, height, areaSize, cellArea2, rules2, debugArray2);
    addPattern(gol2, cellArea2, gliderPattern, 1, 1, width);

    cout << "population according to object:" << gol.populationCount() << endl;
    printCellArea(cellArea,areaSize,width);

    
    char key=0;
    while (key != 'n') {
        thread t1(gol, 1);
        thread t2(gol2, 2);

        this_thread::sleep_for(chrono::seconds(2));
        gol.running = false;
        gol2.running = false;

        printCellArea(cellArea, areaSize, width);
        cout << "******" << endl;
        printCellArea(cellArea2, areaSize, width);
        cout << endl;
        std::cin >> key;
    }
   

}
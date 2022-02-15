#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <thread>

#include <classes/OpenGOLClass.h>
#include <classes/ShaderClass.h>
#include <classes/GameOfLifeClass.h>

#include <classes/GridSumClass.h>
#include <classes/OpenglGridSumClass.h>

#include <classes/BmpToTerrain.h>

#include <array>
#include <chrono>


using namespace std;


int screenwidth = 1000;
int screenheight = 1000;


int main(void)
{
    GLFWwindow* window;
    char key;
    /* Initialize the library */
    if (!glfwInit())
        return -1;

    cout << "check init done..." << endl;
   //cin >> key;

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(screenwidth, screenheight, "Hello World", NULL, NULL);
    if (!window)
    {
        cout << "terminate!" << endl;

        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    if (glewInit() != GLEW_OK)
        std::cout << "woops!" << std::endl;

    cout << "context ok!" << endl;
   // cin >> key;

    short gliderPattern[25] = {
        0,0,0,0,0,
        0,0,0,1,0,
        0,1,0,1,0,
        0,0,1,1,0,
        0,0,0,0,0
    };
    
    short otherPattern[25] = {
      0,0,0,0,0,
      0,1,0,0,0,
      0,1,0,1,0,
      0,1,1,0,0,
      0,0,0,0,0
    };

    OpenGOLClass og = OpenGOLClass(1,256);
    og.addPattern(gliderPattern, 50, 30);
    og.addTerrain("");

    og.generateCellsRaster(-0.9f, 0.9f);
    og.setVertexAndIndexBuffer();
    //og.rules[2] = 2;

    //OpenglGridSumClass oggs = OpenglGridSumClass(2, og.areaWidth / 5);
    //oggs.generateCellsRaster(0.1f, 0.9f);
    //oggs.setVertexAndIndexBuffer();

    ShaderClass shaderobj = ShaderClass();

    ShaderClass::ShaderProgramSource sps = shaderobj.ParseShader("resources/shaders/basic.shader");
    cout << sps.FragmentSource << endl;
    cout << sps.VertexSource << endl;

    unsigned int shader = shaderobj.CreateShader(sps.VertexSource, sps.FragmentSource);

    ShaderClass::ShaderProgramSource sps2 = shaderobj.ParseShader("resources/shaders/basic2.shader");
    cout << sps2.FragmentSource << endl;
    cout << sps2.VertexSource << endl;

    unsigned int shader2 = shaderobj.CreateShader(sps2.VertexSource, sps2.FragmentSource);

    

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
       

        this_thread::sleep_for(chrono::milliseconds(10));
        og.runGeneration();
        //og.showArea();
        og.updateBufferArray();

        

        /*oggs.gridsumObj.analyzeCellArea(og.cellArea, og.areaSize, og.areaWidth);
        oggs.updateBufferArray();*/

        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(shader);
        og.refreshBuffer();
        glDrawElements(GL_TRIANGLES, 6 * og.areaSize, GL_UNSIGNED_INT, nullptr);

       /* glUseProgram(shader2);
        oggs.refreshBuffer();
        glDrawElements(GL_TRIANGLES, 6 * oggs.sumSize, GL_UNSIGNED_INT, nullptr);*/
        
        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    glDeleteProgram(shader);
    glfwTerminate();
    return 0;
}

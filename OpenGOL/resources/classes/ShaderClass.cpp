#include "ShaderClass.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

ShaderClass::ShaderClass() {

};

ShaderClass::ShaderProgramSource ShaderClass::ParseShader(const string& filepath) {
    ifstream stream(filepath);

    enum class ShaderType {
        NONE = -1, VT = 0, FM = 1
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

    return { ss[0].str(), ss[1].str() };

}
unsigned int  ShaderClass::CompileShader(unsigned int type, const string& source) {
    unsigned int id = glCreateShader(type);
    const char* src = source.c_str();
    glShaderSource(id, 1, &src, nullptr);
    glCompileShader(id);
    return id;

}
unsigned int  ShaderClass::CreateShader(const string& vertexshader, const string& fragmentshader) {
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
#pragma once
#include <string>
using namespace std;
class ShaderClass
{
public:
    struct ShaderProgramSource {
        string VertexSource;
        string FragmentSource;
    };
    ShaderClass();

    ShaderProgramSource ParseShader(const string& filepath);
    unsigned int CompileShader(unsigned int type, const string& source);
    unsigned int  CreateShader(const string& vertexshader, const string& fragmentshader);
};


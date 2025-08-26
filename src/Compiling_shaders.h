#pragma once
#include <iostream>
using std::string;

//Lendo shaders de arquivos externos
struct ShaderProgramSource
{
    string VertexSource;
    string FragmentSource;
};

ShaderProgramSource PegarShader(const string& filepath);


unsigned int CompileShader(unsigned int tipo, const string& source);



unsigned int criar_shadder(const string& vertexShader, const string& fragmentShader);
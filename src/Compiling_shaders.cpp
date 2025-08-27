#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <chrono>
#include <cstdint>

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include "LibCalls.h"
using std::string;

//--------------VERTEX SHADERS E FRAGMENT SHADERS-------------------
//     Falando com a GPU
//VERTEX -> diz onde cada vertice do desenha ira estar posicionado.
//FRAGMENT -> diz a cor de cada pixel e desenha na tela

unsigned int CompileShader(unsigned int tipo, const string& source)
{
    unsigned int id = glCreateShader(tipo);
    const char* src = source.c_str(); 

    glShaderSource(id, 1, &src, nullptr); //passo endereço do ponteiro como parametro
    glCompileShader(id);

    //adicionar validacao de shadder, glGetShaderiv

    return id;
}


unsigned int criar_shadder(const string& vertexShader, const string& fragmentShader)
{
    unsigned int programa = glCreateProgram();
    unsigned int vertex_shader = CompileShader(GL_VERTEX_SHADER, vertexShader);
    unsigned int frag_shader = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);

    //attach de shadders - linkar shaders
    glAttachShader(programa, vertex_shader);
    glAttachShader(programa, frag_shader);

    //linkar e validar programa
    glLinkProgram(programa);
    glValidateProgram(programa);

    //deletar intermediario(shader) pois ja foi anexado ao programa
    glDeleteShader(vertex_shader);
    glDeleteShader(frag_shader);

    return programa;
}


ShaderProgramSource PegarShader(const string& filepath)
{
    enum class ShaderType
    {
        NONE = -1, VERTEX = 0, FRAGMENT = 1
    };


    std::ifstream stream(filepath);
    string line;
    std::stringstream ss[2];
    ShaderType type = ShaderType::NONE;

    while (std::getline(stream, line))
    {
        if (line.find("#shader") != string::npos)
        {
            if (line.find("vertex") != string::npos)
                type = ShaderType::VERTEX;
            else if (line.find("fragment") != string::npos)
                type = ShaderType::FRAGMENT;
        }
        else
        {
            ss[(int)type] << line << '\n';
        }
    }
    return { ss[0].str(), ss[1].str() };
}

#pragma once
#include <chrono>
#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
using std::string;

GLFWwindow* LibCalls();

//Lendo shaders de arquivos externos
struct ShaderProgramSource
{
    string VertexSource;
    string FragmentSource;
};

ShaderProgramSource PegarShader(const string& filepath);

unsigned int CompileShader(unsigned int tipo, const string& source);

unsigned int criar_shadder(const string& vertexShader, const string& fragmentShader);

void CompileBuffers(unsigned int& id_buffer, unsigned int& id_ebo, unsigned int& id_vao, float* vertices_quadrado, size_t vertices_size, unsigned int* indices, size_t indices_size);

void Physics(float& pra_cima, float& aside, GLFWwindow* window, uint64_t& delaytick, int& delay, int& offsetLoc, int& offsetLocX);

static uint64_t timeSinceEpochMillisec() {
    using namespace std::chrono;
    return duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();
}
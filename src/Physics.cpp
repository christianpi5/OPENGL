#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "LibCalls.h"
#include <chrono>
#include <cstdint>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

uint64_t now = timeSinceEpochMillisec();

enum Estados {
    PARADO,
    CIMA,
    LEFT,
    BAIXO,
    RIGHT
};
Estados atual = Estados::PARADO;

void Physics(float& pra_cima, float& aside, GLFWwindow* window, uint64_t& delaytick, int& delay, int& offsetLoc, int& offsetLocX)
{
    now = timeSinceEpochMillisec();

    //Fecha janela se tocar nas bordas
    if (pra_cima >= 1.00f || pra_cima <= -1.00f || aside >= 1.0 || aside <= -1.00) {
        glfwSetWindowShouldClose(window, GLFW_TRUE); // fecha a janela
    }

    //---já começa o game indo pra cima
    if (pra_cima == 0.00 && atual == Estados::PARADO)
    {
        atual = Estados::CIMA;
    }

    if (now - delaytick > delay)
    {
        if (atual == Estados::CIMA) {
            pra_cima += 0.1f;
        }
        else if (atual == Estados::BAIXO) {
            pra_cima -= 0.1f;
        }
        else if (atual == Estados::LEFT) {
            aside -= 0.1f;
        }
        else if (atual == Estados::RIGHT) {
            aside += 0.1f;
        }
        delaytick = timeSinceEpochMillisec();

    }


    // INPUTS DE DIRECAO
    if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS && atual != Estados::RIGHT)
    {
        atual = Estados::LEFT;
    }
    if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS && atual != Estados::LEFT)
    {
        atual = Estados::RIGHT;
    }
    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS && atual != Estados::BAIXO)
    {
        atual = Estados::CIMA;
    }
    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS && atual != Estados::CIMA)
    {
        atual = Estados::BAIXO;
    }

    // Enviar o valor atualizado para o shader
    glUniform1f(offsetLoc, pra_cima);
    glUniform1f(offsetLocX, aside);
}
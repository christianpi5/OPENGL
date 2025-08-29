#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "LibCalls.h"
#include <iostream>

void CompileBuffers(unsigned int& id_buffer, unsigned int& id_ebo, unsigned int&  id_vao, float* vertices_quadrado, size_t vertices_size, unsigned int* indices, size_t indices_size)
{
    glGenVertexArrays(1, &id_vao);
    glGenBuffers(1, &id_buffer);//funcao vai me retornar um ID, que sera guardado na variavel id_buffer.     //sempre que eu quiser user aquele buffer, eu passo o valor da ID
    glGenBuffers(1, &id_ebo);
    glBindVertexArray(id_vao);

    glBindBuffer(GL_ARRAY_BUFFER, id_buffer); //Selecionando o buffer e dizer o target(propostio do buffer). agora o id_buffer está em contexto(binded), a funcao desenhar arrays ira desenhar ele
    glBufferData(GL_ARRAY_BUFFER, vertices_size, vertices_quadrado, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id_ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices_size, indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, (void*)0); //Dizendo pro opengl o layout do buffer
    glEnableVertexAttribArray(0);    //habilita vertex

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}
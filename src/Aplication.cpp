#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <chrono>
#include <cstdint>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include "LibCalls.h"

//snake - quadrado verde
float scale = 0.10f;
float vertices_quadrado[] = {
     0.55f * scale,  0.6f * scale, 0.0f,
     0.55f * scale, -0.6f * scale, 0.0f,
    -0.55f * scale, -0.6f * scale, 0.0f,
    -0.55f * scale,  0.6f * scale, 0.0f
};
unsigned int indices[] = {
       0, 1, 3,
       1, 2, 3
};

int main(void)
{
    //chamando libs
    GLFWwindow* window = LibCalls();

    //Compilando buffer
    unsigned int id_buffer,id_ebo,id_vao;
    CompileBuffers(id_buffer,id_ebo,id_vao,vertices_quadrado, sizeof(vertices_quadrado),indices,sizeof(indices));

    //Compilando shader
    ShaderProgramSource source = PegarShader("resources/shaders/Basic.shader");
    unsigned int shader_de_fato = criar_shadder(source.VertexSource, source.FragmentSource);
    if (source.VertexSource.empty() || source.FragmentSource.empty()) {
        std::cerr << "Erro: Shader não carregado!\n";
    }
    
    //Posicoes
    int offsetLoc = glGetUniformLocation(shader_de_fato, "offsetY");
    int offsetLocX = glGetUniformLocation(shader_de_fato, "offsetX");
    float pra_cima = 0.0f;
    float aside = 0.0f;

    //Simular Frame
    int delay = 110;
    uint64_t delaytick = timeSinceEpochMillisec();


    while (!glfwWindowShouldClose(window))
    {
        glClear(GL_COLOR_BUFFER_BIT);

        //Fisicas do quadrado
        Physics(pra_cima, aside, window, delaytick, delay,offsetLoc,offsetLocX);

        // Desenhar o quadrado
        glUseProgram(shader_de_fato);
        glBindVertexArray(id_vao);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT,0);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);


        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    glDeleteVertexArrays(1, &id_vao);
    glDeleteBuffers(1, &id_buffer);
    glDeleteBuffers(1, &id_ebo);

    glfwTerminate();
    return 0;
}
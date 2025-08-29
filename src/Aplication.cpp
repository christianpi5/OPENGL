#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <chrono>
#include <cstdint>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include "LibCalls.h"     
#include <vector>
#include <random>



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

//x1+size lado superior da snake maior que lado inferior(x1-size) da apple, sobreposição
//x1-size lado inferior da snake menor que lado superior(x2-size) da apple, sobreposição
bool checkCollision(float x1, float y1, float x2, float y2, float size) {
    return (x1 + size > x2 - size &&
        x1 - size < x2 + size &&
        y1 + size > y2 - size &&
        y1 - size < y2 + size);
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

    int colorLoc = glGetUniformLocation(shader_de_fato, "cor");

    //posicao inicial da primeira apple
    float appleX = 0.5f;
    float appleY = 0.5f;

    // metade da largura ou altura
    float metade_tamanho = scale * 0.55f; 

    //Snake como vetor
    struct Segment {
        float x;
        float y;
    };

    std::vector<Segment> snake;
    snake.push_back({ aside, pra_cima }); //acrescenta 2 valores a mais de x e y
    snake[0].x = aside; //atualilza cabeca x 
    snake[0].y = pra_cima;//atualilza cabeca y



    while (!glfwWindowShouldClose(window))
    {
        glClear(GL_COLOR_BUFFER_BIT);

        //Fisicas do quadrado
        Physics(pra_cima, aside, window, delaytick, delay,offsetLoc,offsetLocX);


        //faz o ultimo indice copiar o penultimo sempre
        //fazendo com que siga a posição do primeiro indice(cabeca)
        //do ultimo segmento (size - 1) ao segundo seguimento (indice 1, indice > 0)
        for (int i = snake.size() - 1; i > 0; --i)
        {
            snake[i] = snake[i - 1];
        }

        //novas coordenadas pra cabeca, vindo do offset do shader uniform
        snake[0].x = aside;
        snake[0].y = pra_cima;

        float snakeX = snake[0].x;
        float snakeY = snake[0].y;


        glUseProgram(shader_de_fato);
        glBindVertexArray(id_vao);

        //auto& seg : VETOR, para cada seguimento do vetor, FACA:
        for (auto& seg : snake)
        {
            //glUniform1f vai informar pro shader renderizar a snake nas novas posicoes(seg.x e seg.y)
            glUniform1f(offsetLoc, seg.y);
            glUniform1f(offsetLocX, seg.x);
            glUniform4f(colorLoc, 0.0f, 1.0f, 0.0f, 1.0f);
            glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        }

        //Quadrado apple
        glUniform1f(offsetLocX, appleX);   
        glUniform1f(offsetLoc, appleY);  
        glUniform4f(colorLoc, 1.0f, 0.0f, 0.0f, 1.0f); // vermelho RGBA
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        if (checkCollision(snakeX, snakeY, appleX, appleY, metade_tamanho))
        {
            //[head] -> [body] -> [tail] -> [new] -> [new] -> [new]
            int growAmount = 5; // quantos segmentos crescer de uma vez
            for (int i = 0; i < growAmount; ++i) {
                int lastIndex = snake.size() - 1;
                Segment newSeg = { snake[lastIndex].x, snake[lastIndex].y };
                snake.push_back(newSeg);
            }

            // Nova posição aleatória da apple
            static std::random_device rd;
            static std::mt19937 gen(rd());
            float maxOffset = 1.0f - metade_tamanho;
            std::uniform_real_distribution<float> dist(-maxOffset, maxOffset);
            appleX = dist(gen);
            appleY = dist(gen);
        }
       
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    glDeleteVertexArrays(1, &id_vao);
    glDeleteBuffers(1, &id_buffer);
    glDeleteBuffers(1, &id_ebo);

    glfwTerminate();
    return 0;
}
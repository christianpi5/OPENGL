#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <chrono>
#include <cstdint>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include "Compiling_shaders.h"

using std::string;

enum Estados {
    PARADO,
    CIMA,
    LEFT,
    BAIXO, 
    RIGHT
};

Estados atual = Estados::PARADO;


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


//pegando tempo em unix timestamp para poder setar o frame
static uint64_t timeSinceEpochMillisec() {
    using namespace std::chrono;
    return duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();
}

int main(void)
{
    GLFWwindow* window;

    if (!glfwInit())
        return -1;

    window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);

    if (glewInit() != GLEW_OK) {
        std::cout << "Error!\n"; //precisa vir depois de tornar context current
    }

//---------------------------Enviado os dados do BUFFER---------------------------------
    unsigned int id_buffer,id_ebo,id_vao;
    glGenBuffers(1, &id_buffer);//funcao vai me retornar um ID, que sera guardado na variavel id_buffer.     //sempre que eu quiser user aquele buffer, eu passo o valor da ID
    glGenBuffers(1, &id_ebo);
    glGenVertexArrays(1, &id_vao);
    glBindVertexArray(id_vao);
    glBindBuffer(GL_ARRAY_BUFFER, id_buffer); //Selecionando o buffer e dizer o target(propostio do buffer). agora o id_buffer está em contexto(binded), a funcao desenhar arrays ira desenhar ele
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id_ebo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices_quadrado), vertices_quadrado, GL_STATIC_DRAW);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, (void*)0); //Dizendo pro opengl o layout do buffer
    glEnableVertexAttribArray(0);    //habilita vertex
    glBindBuffer(GL_ARRAY_BUFFER, 0); 
    glBindVertexArray(0);
    //-----------------------chamando funcao criar_shader-----------------------

    ShaderProgramSource source = PegarShader("resources/shaders/Basic.shader");
    unsigned int shader_de_fato = criar_shadder(source.VertexSource, source.FragmentSource);

    if (source.VertexSource.empty() || source.FragmentSource.empty()) {
        std::cerr << "Erro: Shader não carregado!\n";
    }
    
    int offsetLoc = glGetUniformLocation(shader_de_fato, "offsetY");
    int offsetLocX = glGetUniformLocation(shader_de_fato, "offsetX");

    float pra_cima = 0.0f;
    float aside = 0.0f;
    int delay = 110;
    uint64_t now = timeSinceEpochMillisec();
    uint64_t delaytick = timeSinceEpochMillisec();
    
//---------------------------------------------------------------------------------


    while (!glfwWindowShouldClose(window))
    {
        now = timeSinceEpochMillisec();

        glClear(GL_COLOR_BUFFER_BIT);

        //Fecha janela se tocar nas bordas
        if (pra_cima >= 1.00f || pra_cima <= -1.00f || aside >= 1.0 || aside <= -1.00) {
            glfwSetWindowShouldClose(window, GLFW_TRUE); // fecha a janela
        }

        /*Debbug     
        if (pra_cima > 0.95f) {
            pra_cima = 0.95;
            estado = PARADO;
        }
        if (pra_cima < -0.95f ) {
            pra_cima = -0.95f;
            estado = PARADO;
        }
        if (aside > 0.95) {
            aside = 0.95;
            estado = PARADO;
        }
        if (aside < -0.95) {
            aside = -0.95;
            estado = PARADO;
        }
        */

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

        // Desenhar o quadrado
        glUseProgram(shader_de_fato);

        // <<< enviar o valor atualizado do uniform para o shader
        glUniform1f(offsetLoc, pra_cima);
        glUniform1f(offsetLocX, aside);

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
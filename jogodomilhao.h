#ifndef JOGODOMILHAO_H
#define JOGODOMILHAO_H

#include "raylib.h"

/**
 * @struct Pergunta
 * @brief Estrutura que representa uma pergunta do Quiz.
 */
typedef struct{
    char *enunciado, *alt1, *alt2, *alt3, *alt4, *dica;
    char opcoes[4][50];
    char resposta;
    int nivelDif, valor;
}Pergunta;

//Conta numero de linhas para definir o tamanho inicial do vetor
int contar_linhas(const char *nome_arquivo);

//Carrega as perguntas do arquivo csv
Pergunta* carrega_perguntas(const char *nome_arquivo, int tam);

//Aloca memoria e copia uma string
void aloca_copia(char **destino, const char *origem);

//Exibe o menu principal
void menu(Pergunta **perguntas, int *tam);

//Insere uma nova pergunta no vetor
void inserir(Pergunta **pergunta, int *tam);

//Lista as perguntas existentes no vetor
void listar(Pergunta *pergunta, int tam);

//Pesquisa uma pergunta pelo enunciado
int pesquisar(Pergunta *pergunta, int tam);

//Altera uma pergunta já exsitente
void alterar(Pergunta *pergunta, int tam);

//Exlcui uma pergunta já existente
void excluir(Pergunta **pergunta, int *tam);

//Salva as perguntas/alterações realizadas no arquivo
void salvar(Pergunta *pergunta, int tam);

//Libera a memória alocada pelas perguntas
void liberarMemoria(Pergunta *perguntas, int tam);

//Inicia o Jogo.
void jogar(Pergunta perguntas[], int total);

void button_animation(Rectangle button, Color color);

int centralizar_X(const char *name_button, int fontSize);

void jogo_encerrado(int pressed, int valorSeguro);
#endif
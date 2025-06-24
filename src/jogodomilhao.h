#ifndef JOGODOMILHAO_H
#define JOGODOMILHAO_H

#include "raylib.h"

/**
 * @struct Pergunta
 * @brief Estrutura que representa uma pergunta do Quiz.
 */

typedef struct{
    char *enunciado, *alt1, *alt2, *alt3, *alt4, *dica;
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
void liberar_memoria(Pergunta *perguntas, int tam);

//Inicia o Jogo.
void jogar(Pergunta *perguntas, int total);

//Faz a animação de hover e click em um botão
void button_animation(Rectangle button, Color color);

//Centraliza o eixo X de um texto na tela dinamicamente.
int centralizar_X(const char *name_button, int fontSize);

//Exibe a tela 'tutorial' antes de iniciar o jogo.
void tutorial();

//Exibe a tela de encerramento do jogo após ganhar ou perder.
void jogo_encerrado(int respostaSelecionada, int valorSeguro, int valorGanho);

//Le as strings digitadas e retorna para status para verificação de alocação de memória, 0 para OK, 1 para Cancelado. 
int ler_string(char *label, char *input, int posY);

//Exibe o texto centralizado dentro de um container, quebrando linhas automaticamente conforme a largura do container
void quebrar_linha(const char *text, Rectangle container, int fontSize, Color color);

#endif
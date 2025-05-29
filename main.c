#include <stdio.h>
#include <stdlib.h>
#include "jogodomilhao.h"
#include "raylib.h"

int main(){
    const char *arquivo = "questoes.csv";
    int tam = contar_linhas(arquivo);
    Pergunta *perguntas = carrega_perguntas(arquivo, tam);
    if (perguntas == NULL) {
        printf("Falha ao carregar perguntas.\n");
        return 1;
    }

    InitWindow(720, 520, "Jogo do Milhão");

    menu(&perguntas, &tam);
    
    CloseWindow();

    salvar(perguntas, tam);
    liberarMemoria(perguntas, tam);
    perguntas = NULL;
    return 0;
}
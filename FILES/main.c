#include <stdio.h>
#include <stdlib.h>
#include "jogodomilhao.h"

int main(){
    const char *arquivo = "questoes.csv";
    int tam = contar_linhas(arquivo);
    Pergunta *perguntas = carrega_perguntas(arquivo, tam);
    if (perguntas == NULL) {
        printf("Falha ao carregar perguntas.\n");
        return 1;
    }

    limparTela();
    menu(&perguntas, &tam);
    salvar(perguntas, tam);

    liberarMemoria(perguntas, tam);
    perguntas = NULL;
    return 0;
}
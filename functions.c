#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "raylib.h"
#include "jogodomilhao.h"


int contar_linhas(const char *nome_arquivo){
    FILE *fp = fopen(nome_arquivo, "r");
    if(fp == NULL){
        return 0;
    }

    int count = 0;
    char c;
    while((c = fgetc(fp)) != EOF){
        if(c == '\n'){
            count++;
        }
    }
    fclose(fp);
    return count;
}

Pergunta* carrega_perguntas(const char *nome_arquivo, int tam) {
    FILE *fp = fopen(nome_arquivo, "r");
    if (fp == NULL) {
        perror("Erro ao abrir arquivo");
        return NULL;
    }

    Pergunta *perguntas = malloc(tam * sizeof(Pergunta));
    if (perguntas == NULL) {
        perror("Erro de alocação");
        fclose(fp);
        return NULL;
    }

    char linha[1024];

    for (int i = 0; i < tam && fgets(linha, sizeof(linha), fp); i++) {
        char enunciado[256], alt1[64], alt2[64], alt3[64], alt4[64], dica[256];
        char resposta;
        int nivelDif, valor;

        sscanf(linha, "%255[^,],%63[^,],%63[^,],%63[^,],%63[^,],%c,%d,%255[^,],%d",
               enunciado, alt1, alt2, alt3, alt4, &resposta, &nivelDif, dica, &valor);

        aloca_copia(&perguntas[i].enunciado, enunciado);
        aloca_copia(&perguntas[i].alt1, alt1);
        aloca_copia(&perguntas[i].alt2, alt2);
        aloca_copia(&perguntas[i].alt3, alt3);
        aloca_copia(&perguntas[i].alt4, alt4);
        aloca_copia(&perguntas[i].dica, dica);

        perguntas[i].resposta = resposta;
        perguntas[i].nivelDif = nivelDif;
        perguntas[i].valor = valor;
    }

    fclose(fp);
    return perguntas;
}

void aloca_copia(char **destino, const char *origem) {
    *destino = malloc(strlen(origem) + 1);
    if (*destino == NULL) {
        perror("Erro de alocação");
        exit(1);
    }
    strcpy(*destino, origem);
}

void inserir(Pergunta **pergunta, int *tam){
    char buffer[200];
    *tam += 1;
    Pergunta *tmp = realloc(*pergunta, (*tam) * sizeof(Pergunta));
    if (!tmp) {
        perror("Erro ao alocar espaço para nova pergunta: ");
        exit(1);
    }
    *pergunta = tmp;

    int i = *tam - 1;

    printf("\nDigite o enunciado da pergunta: ");
    fgets(buffer, 200, stdin);
    buffer[strcspn(buffer, "\n")]='\0';
    (*pergunta)[i].enunciado = malloc(strlen(buffer)+1);
    if((*pergunta)[i].enunciado == NULL){
        perror("Erro de alocação: ");
        exit(1);
    }
    strcpy((*pergunta)[i].enunciado, buffer);

    char **alternativas[] = {&(*pergunta)[i].alt1, &(*pergunta)[i].alt2, &(*pergunta)[i].alt3, &(*pergunta)[i].alt4};
    for(int j = 0; j < 4; j++){
        printf("Digite a alternativa %d: ", j+1);
        fgets(buffer, 200, stdin);
        buffer[strcspn(buffer, "\n")]='\0';
        *alternativas[j] = malloc(strlen(buffer)+1);
        if(*alternativas[j] == NULL){
            perror("Erro de alocação: ");
            exit(1);
        }
        strcpy(*alternativas[j], buffer);
    }

    printf("Digite a dica da pergunta: ");
    fgets(buffer, 200, stdin);
    buffer[strcspn(buffer, "\n")]='\0';
    (*pergunta)[i].dica = malloc(strlen(buffer)+1);
    if((*pergunta)[i].dica == NULL){
        perror("Erro de alocação: ");
        exit(1);
    }
    strcpy((*pergunta)[i].dica, buffer);

    printf("Digite qual é a alternativa correta (A)(B)(C)(D): ");
    scanf(" %c", &(*pergunta)[i].resposta);

    printf("Digite qual o nível de dificuldade da pergunta: ");
    scanf("%d", &(*pergunta)[i].nivelDif);
    getchar();

    printf("Digite qual o valor da pergunta: ");
    scanf("%d", &(*pergunta)[i].valor);
    getchar();
    printf("\nDados inseridos com sucesso!\n");
}

void listar(Pergunta *pergunta, int tam){
    for(int i = 0; i < tam; i++){
        printf("\nPergunta %d\n", i+1);
        printf("Enunciado: %s\n", pergunta[i].enunciado);
        printf("Alternativas \n");
        printf("[A] %s\n", pergunta[i].alt1);
        printf("[B] %s\n", pergunta[i].alt2);
        printf("[C] %s\n", pergunta[i].alt3);
        printf("[D] %s\n", pergunta[i].alt4);
        printf("Alternativa correta: %c\n", pergunta[i].resposta);
        printf("Nível de dificuldade: %d\n\n", pergunta[i].nivelDif);
    }
}

int pesquisar(Pergunta *pergunta, int tam){
    char search[200];
    printf("Digite o enunciado da questão que deseja pesquisar: ");
    fgets(search, 200, stdin);
    search[strcspn(search, "\n")]='\0';

    for(int i = 0; i < tam; i++){
        if(strcmp(pergunta[i].enunciado, search) == 0){
            printf("Questão encontrada: ");
            printf("%s\n", pergunta[i].enunciado);
            printf("Alternativas \n");
            printf("[A] %s\n", pergunta[i].alt1);
            printf("[B] %s\n", pergunta[i].alt2);
            printf("[C] %s\n", pergunta[i].alt3);
            printf("[D] %s\n", pergunta[i].alt4);
            printf("Alternativa correta: %c\n", pergunta[i].resposta);
            printf("Nível de dificuldade: %d\n\n", pergunta[i].nivelDif);
            return i;
        }
    }

    printf("Questão não encontrada\n");
    return -1;
}

void alterar(Pergunta *pergunta, int tam){
    int indice;
    char buffer[200];

    indice = pesquisar(pergunta, tam);
    if(indice == -1){
        return;
    }

    printf("Digite os novos dados!\n");
    printf("Digite o enunciado: ");
    fgets(buffer, 200, stdin);
    buffer[strcspn(buffer, "\n")]='\0';
    pergunta[indice].enunciado = realloc(pergunta[indice].enunciado, strlen(buffer)+1);
    if(pergunta[indice].enunciado == NULL){
        perror("Erro de alocação: ");
        exit(1);
    }
    strcpy(pergunta[indice].enunciado, buffer);

    char **alternativas[] = {&pergunta[indice].alt1, &pergunta[indice].alt2, &pergunta[indice].alt3, &pergunta[indice].alt4};
    for(int i = 0; i < 4; i++){
        printf("Digite a alternativa %d: ", i+1);
        fgets(buffer, 200, stdin);
        buffer[strcspn(buffer, "\n")]='\0';
        *alternativas[i] = realloc(*alternativas[i], strlen(buffer)+1);
        if(*alternativas[i] == NULL){
            perror("Erro de alocação: ");
            exit(1);
        }
        strcpy(*alternativas[i], buffer);

    }

    printf("Digite a dica da pergunta: ");
    fgets(buffer, 200, stdin);
    buffer[strcspn(buffer, "\n")]='\0';
    pergunta[indice].dica = malloc(strlen(buffer)+1);
    if(pergunta[indice].dica == NULL){
        perror("Erro de alocação: ");
        exit(1);
    }
    strcpy(pergunta[indice].dica, buffer);

    printf("Digite a alternativa correta (A)(B)(C)(D): ");
    scanf(" %c", &pergunta[indice].resposta);

    printf("Digite a dificuldade: ");
    scanf("%d", &pergunta[indice].nivelDif);
    getchar();

    printf("Digite qual o valor da pergunta: ");
    scanf("%d", &pergunta[indice].valor);
    getchar();
    printf("\nDados inseridos com sucesso!\n");
}

void excluir(Pergunta **pergunta, int *tam){
    int indice;

    indice = pesquisar(*pergunta, *tam);
    if(indice == -1){
        return;
    }

    free((*pergunta)[indice].enunciado);
    free((*pergunta)[indice].alt1);
    free((*pergunta)[indice].alt2);
    free((*pergunta)[indice].alt3);
    free((*pergunta)[indice].alt4);
    free((*pergunta)[indice].dica);

    for(int i = indice; i < *tam - 1; i++){
        (*pergunta)[i] = (*pergunta)[i + 1];
    }

    (*tam)--;
    if(*tam > 0){
        Pergunta *tmp = realloc(*pergunta, *tam * sizeof(Pergunta));
        if(tmp != NULL){
            *pergunta = tmp;
        } else {
            perror("Erro ao reduzir o tamanho do vetor: ");
        }
    }else{
        free(*pergunta);
        *pergunta = NULL;
    }
    printf("Pergunta excluida com sucesso!");
}

void salvar(Pergunta *pergunta, int tam){
    FILE *fp = fopen("questoes.csv", "w");
    if (fp == NULL) {
        perror("Erro ao abrir o arquivo para salvar");
        return;
    }

    for (int i = 0; i < tam; i++) {
        fprintf(fp, "%s,%s,%s,%s,%s,%c,%d,%s,%d\n", pergunta[i].enunciado, pergunta[i].alt1, pergunta[i].alt2, pergunta[i].alt3, pergunta[i].alt4, pergunta[i].resposta, pergunta[i].nivelDif, pergunta[i].dica, pergunta[i].valor);
    }

    fclose(fp);
}

void menu(Pergunta **perguntas, int *tam) {
    Rectangle play = {200, 100, MeasureText("Jogar", 20), 20};
    Rectangle add = {200, 130, MeasureText("Adicionar pergunta", 20), 20};
    Rectangle list = {200, 160, MeasureText("Listar perguntas", 20), 20};
    Rectangle search = {200, 190, MeasureText("Pesquisar perguntas", 20), 20};
    Rectangle modify = {200, 220, MeasureText("Alterar perguntas", 20), 20};
    Rectangle remove = {200, 250, MeasureText("Excluir perguntas", 20), 20};
    Rectangle exit = {200, 280, MeasureText("Salvar e sair", 20), 20};
    bool clicou = false;

    while(!WindowShouldClose()){
        BeginDrawing();
        ClearBackground(WHITE);
        DrawText("===== MENU =====", 200, 50, 20, BLACK);

        button_animation(play);
        DrawText("Jogar", 200, 100, 20, BLACK);  

        button_animation(add);
        DrawText("Adicionar pergunta", 200, 130, 20, BLACK);

        button_animation(list);
        DrawText("Listar perguntas", 200, 160, 20, BLACK);

        button_animation(search);
        DrawText("Pesquisar perguntas", 200, 190, 20, BLACK);

        button_animation(modify);
        DrawText("Alterar perguntas", 200, 220, 20, BLACK);

        button_animation(remove);
        DrawText("Excluir perguntas", 200, 250, 20, BLACK);

        button_animation(exit);
        if(CheckCollisionPointRec(GetMousePosition(), exit)){
            if(IsMouseButtonReleased(MOUSE_LEFT_BUTTON)){
                return;
            }
        }
        DrawText("Salvar e sair", 200, 280, 20, BLACK);

        EndDrawing();
    }

        
/*
        switch(opcao) {
            case 1:
                jogar(*perguntas, *tam);
                break;
            
            case 2:
                inserir(perguntas, tam);
                break;

            case 3:
                listar(*perguntas, *tam);
                break;
            
            case 4:
                pesquisar(*perguntas, *tam);
                break;

            case 5:
                alterar(*perguntas, *tam);
                break;

            case 6:
                excluir(perguntas, tam);
                break;
        */
}

void liberarMemoria(Pergunta *perguntas, int tam){
    for(int i=0; i < tam; i++){
        free(perguntas[i].enunciado);
        free(perguntas[i].alt1);
        free(perguntas[i].alt2);
        free(perguntas[i].alt3);
        free(perguntas[i].alt4);
        free(perguntas[i].dica);
    }
    free(perguntas);
}

void jogar(Pergunta *pergunta, int total){
    int i;
    char resposta;
    int dicasDisponiveis = 2;
    int dinheiroGanho = 0;

    printf("===============================\n");
    printf("Vai começar o jogo do Milhão!\n");
    printf("Responda as pergunta com as letras A, B, C ou D.\n");
    printf("Você tem direito a 2 dicas curtas durante o jogo.\n");
    printf("Para usar uma dica, digite 'DC'.\n");
    printf("===============================\n\n");

    for(i = 0; i < total; i++){
        printf("Pergunta %d (R$ %d):\n%s\n", i + 1, pergunta[i].valor, pergunta[i].enunciado);
        /*for(int j = 0; j < 4; j++){
            printf("%c) %s\n", 'A' + j, pergunta[i].opcoes[j]);
        }//for*/
        printf("A) %s\n", pergunta[i].alt1);
        printf("B) %s\n", pergunta[i].alt2);
        printf("C) %s\n", pergunta[i].alt3);
        printf("D) %s\n", pergunta[i].alt4);
        if(dicasDisponiveis > 0 && strlen(pergunta[i].dica) > 0){
            printf("Você pode digitar 'DC' para uma dica (dicas restantes: %d).\n", dicasDisponiveis);
        }//if

        int recebeuDicaNaVez = 0;

        while(1){
            printf("Sua resposta (A, B, C, D ou DC para dica): ");
            resposta = lerResposta();

            if(resposta == 'X'){
                if(dicasDisponiveis > 0 && strlen(pergunta[i].dica) > 0){
                    if(!recebeuDicaNaVez){
                        printf("Dica: %s\n", pergunta[i].dica);
                        dicasDisponiveis--;
                        recebeuDicaNaVez = 1;
                        printf("Dicas restantes: %d\n", dicasDisponiveis);
                    }else{
                        printf("Você já usou a dica para esta pergunta.\n");
                    }//else
                }else{
                    printf("Você não tem mais dicas disponíveis ou esta pergunta não tem dica.\n");
                }//if
                continue; //pergunta a resposta após mostrar a dica
            }//if

            if(resposta >= 'A' && resposta <= 'D'){
                break; //resposta válida
            }//if

            printf("Resposta inválida. Digite A, B, C, D ou DC para dica.\n");
        }//while

        if(resposta == pergunta[i].resposta){
            printf("Correto!\n\n");
            dinheiroGanho = pergunta[i].valor;
        }else{
            printf("Incorreto! A resposta correta era %c.\n\n", pergunta[i].resposta);
            break;
        }//else
    }//void jogar

    //Marcos de segurança e valores correspondentes para garantir dinheiro em caso de perda
    int marco1 = 5; // pergunta 5
    int marco2 = 10; // pergunta 10
    int valorSeguro = 0;

    if(i >= marco2){
        valorSeguro = pergunta[marco2 - 1].valor;
    }else if(i >= marco1){
        valorSeguro = pergunta[marco1 - 1].valor;
    }else{
        valorSeguro = 0;
    }//else

    if(i == total){
        printf("Parabéns! Você respondeu todas as pergunta corretamente e ganhou o maior prêmio de R$ %d de reais!\n", dinheiroGanho);
    }else{
        printf("Fim de jogo. Você ganhou R$ %d reais.\n", valorSeguro);
        printf("Tente novamente!\n");
    }//else
}//void jogar

char lerResposta(){
    char buf[3]; //para ler duas letras + '\0'
    scanf("%2s", buf);

    //Converte para maiúscula
    for(int i = 0; i < (int)strlen(buf); i++){
        if(buf[i] >= 'a' && buf[i] <= 'z'){
            buf[i] = buf[i] - ('a' - 'A');
        }//if
    }//for

    if(strlen(buf) == 1){
        char c = buf[0];
        if(c >= 'A' && c <= 'D'){
            return c;
        }//if
    } else if(strlen(buf) == 2){
        if(buf[0] == 'D' && buf[1] == 'C'){
            return 'X'; //sinaliza pedido de dica
        }//if
    }//for
    return '\0'; //inválida
}//char ler resposta

//Função para converter caracteres minúsculos para maiúsculos
char paraMaiuscula(char c){
    if(c >= 'a' && c <= 'z'){
        return c - ('a' - 'A');
    }//if
    return c;
}//char converter para maiuscula

//Função para ler a resposta do jogador, aceita "DC" para dica
//Retorna:
//'A' 'B' 'C' 'D' para respostas válidas
//'X' para pedir dica (DC)
//'\0' para inválidos

void button_animation(Rectangle button){
    DrawRectangleRec(button, WHITE);
    if(CheckCollisionPointRec(GetMousePosition(), button)){
        DrawRectangleRec(button, LIGHTGRAY);
        if(IsMouseButtonDown(MOUSE_LEFT_BUTTON)){
            DrawRectangleRec(button, GOLD);
        }
    }
}



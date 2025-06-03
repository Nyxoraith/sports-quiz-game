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
    const char *nameButtons[] = {"Jogar", "Adicionar pergunta", "Listar perguntas", "Pesquisar perguntas", "Alterar perguntas", "Excluir perguntas", "Salvar e sair"};
    Texture2D logo_show = LoadTexture("resources/show.png");
    Rectangle play[7]; 

    for(int i = 0; i < 7; i++){
        play[i].x = centralizar_X(nameButtons[i], 20);
        play[i].y = 280 + i * 30;
        play[i].width = MeasureText(nameButtons[i], 20);
        play[i].height = 20;
    }

    while(!WindowShouldClose()){
        BeginDrawing();
        ClearBackground(WHITE);

        DrawTexture(logo_show, (GetScreenWidth() - logo_show.width)/2, 10, WHITE);
        DrawText("===== MENU =====", centralizar_X("===== MENU =====", 20), 230, 20, BLACK);
        for(int i = 0; i < 7; i++){
            button_animation(play[i], YELLOW);
            DrawText(nameButtons[i], play[i].x, play[i].y, 20, BLACK);  
        }

        //Jogar
        if(CheckCollisionPointRec(GetMousePosition(), play[0])){
            if(IsMouseButtonReleased(MOUSE_LEFT_BUTTON)){
                jogar(*perguntas, *tam);
            }
        }


        //Exit
        if(CheckCollisionPointRec(GetMousePosition(), play[6])){
            if(IsMouseButtonReleased(MOUSE_LEFT_BUTTON)){
                return;
            }
        }
        EndDrawing();
    }
    UnloadTexture(logo_show);
}      

void liberar_memoria(Pergunta *perguntas, int tam){
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
    //Declaração das variaveis/flags de controle
    float delay = 1.5; //Delay entre perguntas

    int marco1 = 5; // pergunta 5
    int marco2 = 10; // pergunta 10
    int gameFinal = 0;
    int dicaUsada = 0;
    int valorSeguro = 0;
    int perguntaAtual = 0;
    int dinheiroGanho = 0;
    int tempoEsperando = 0;
    int recebeuDicaNaVez = 0;   
    int dicasDisponiveis = 2;
    int respostaSelecionada = 0;
    int esperandoProxPergunta = 0;
    
    //Declaração de buffer que será utilizado para formatação de strings
    char buffer[256];

    //Cria os 'botões' referentes a label respectiva.
    Rectangle voltar = {centralizar_X("Voltar ao menu!", 20), 480, MeasureText("Voltar ao menu!", 20), 20};
    Rectangle alt_buttons[4];
    Rectangle dicas = {100, 270, MeasureText("Receber dica",20), 20};

    while(!WindowShouldClose()){
        BeginDrawing();
        ClearBackground(WHITE);
        char *alternativas[] = {pergunta[perguntaAtual].alt1, pergunta[perguntaAtual].alt2, pergunta[perguntaAtual].alt3, pergunta[perguntaAtual].alt4};
        for(int i = 0; i < 4; i++){
            sprintf(buffer, "%c) %s", 'A' + i, alternativas[i]);
            alt_buttons[i].x = centralizar_X(alternativas[i], 20);
            alt_buttons[i].y = 90 + i * 30;
            alt_buttons[i].width = MeasureText(buffer, 20);
            alt_buttons[i].height = 20;
        }

        //Enunciado
        DrawText(pergunta[perguntaAtual].enunciado, centralizar_X(pergunta[perguntaAtual].enunciado, 20), 50, 20, BLACK);

        //Questoes
        for(int i = 0; i < 4; i++){
            char opcao = 'A' + i;
            if(opcao == pergunta[perguntaAtual].resposta){
                button_animation(alt_buttons[i], GREEN);
            }else{
                button_animation(alt_buttons[i], RED);
            }
            
            sprintf(buffer, "%c) %s", 'A' + i, alternativas[i]);
            DrawText(buffer, centralizar_X(alternativas[i], 20), 90 + i * 30, 20, BLACK);

            //Verifica se a alternativa escolhida pelo usuario está correta ou não.
            if(CheckCollisionPointRec(GetMousePosition(), alt_buttons[i])){
                if(IsMouseButtonPressed(MOUSE_LEFT_BUTTON)){
                    if(opcao == pergunta[perguntaAtual].resposta){
                        respostaSelecionada = 1;
                    }else{
                        respostaSelecionada = 2;
                    }
                }
            }
        }

        //Exibe a mensage se o usuario respondeu a pergunta atual corretamente ou não, se sim atualiza o valor ganho.
        if (respostaSelecionada == 1){
            DrawText("Resposta correta!", centralizar_X("Reposta correta!", 20), 225, 20, GREEN);
            dinheiroGanho = pergunta[perguntaAtual].valor;
        }else if(respostaSelecionada == 2){
            DrawText("Resposta Incorreta!", centralizar_X("Reposta correta!", 20), 225, 20, RED);
        }

        //Guarda valores da pergunta 5 e pergunta 10 caso erre a próxima questão.
        if(perguntaAtual >= marco2){
            valorSeguro = pergunta[marco2 - 1].valor;
        }else if(perguntaAtual >= marco1){
            valorSeguro = pergunta[marco1 - 1].valor;
        }else{
            valorSeguro = 0;
        }

        //Mostra o valor que a pergunta vale
        DrawText("Valendo: ", 450, 270, 20, BLACK);
        sprintf(buffer, "R$ %d",pergunta[perguntaAtual].valor);
        DrawText(buffer, 600, 270, 20, BLACK);

        //Mostra o valor que ganha se Acertar
        DrawText("Valor Ganho: ", 450, 300, 20, BLACK);
        sprintf(buffer, "R$ %d", dinheiroGanho);
        DrawText(buffer, 600, 300, 20, BLACK);

        //Mostra o valor que ganha se errar
        DrawText("Se errar: ", 450, 330, 20, BLACK);
        sprintf(buffer, "R$ %d", valorSeguro);
        DrawText(buffer, 600, 330, 20, BLACK);

        //Cria um delay entre uma pergunta e outra
        if((respostaSelecionada == 1 || respostaSelecionada == 2) && !esperandoProxPergunta){
            esperandoProxPergunta = 1;
            tempoEsperando = GetTime();
        }

        if(esperandoProxPergunta){
            if(GetTime() - tempoEsperando >= delay){
                //Se respondeu errado
                if(respostaSelecionada == 2){
                    jogo_encerrado(respostaSelecionada, valorSeguro, 0);
                    break;
                }

                //Zera as variaveis e avança para a proxima pergunta
                esperandoProxPergunta = 0;
                respostaSelecionada = 0;
                recebeuDicaNaVez = 0;
                dicaUsada = 0;
                perguntaAtual++;

                //Se respondeu todas as perguntas corretas
                if(perguntaAtual >= total){
                    jogo_encerrado(respostaSelecionada, valorSeguro, dinheiroGanho);
                    break;
                }
            }
        }

        //Faz a validação das dicas disponiveis
        if(dicasDisponiveis > 0 && strlen(pergunta[perguntaAtual].dica) > 0){
            sprintf(buffer, "%d", dicasDisponiveis);
            if(!dicaUsada){
                button_animation(dicas, GOLD);
            }else{
                button_animation(dicas, RED);
            }
            DrawText("Receber dica", 100, 270, 20, BLACK);
            DrawText("Dicas Disponiveis:", 100, 300, 20, BLACK); 
            DrawText(buffer, 280, 300, 20, BLACK);
            if(CheckCollisionPointRec(GetMousePosition(), dicas)){
                if(IsMouseButtonPressed(MOUSE_LEFT_BUTTON)){
                    if(recebeuDicaNaVez == 0){
                        recebeuDicaNaVez++;
                        dicasDisponiveis--;
                        DrawText(buffer, 280, 300, 20, BLACK);
                    }
                    else{
                        dicaUsada = 1;
                    }
                }   
            }
        }else{
            DrawText("Você não tem mais dicas disponíveis ou esta pergunta não tem dica.", centralizar_X("Você não tem mais dicas disponíveis ou esta pergunta não tem dica.", 20), 430, 20, BLACK);
        }

        //Exibe o texto que já foi utilizado dica na pergunta
        if (recebeuDicaNaVez == 1) {
            DrawText(pergunta[perguntaAtual].dica, centralizar_X(pergunta[perguntaAtual].dica, 20), 380, 20, BLACK);
           if (dicaUsada == 1) {
                DrawText("Você já usou a dica para esta pergunta.", centralizar_X("Você já usou a dica para esta pergunta.", 20), 430, 20, BLACK);
            }
        }


        //Botão para voltar ao menu
        button_animation(voltar, GOLD);
        DrawText("Voltar ao menu!", voltar.x, voltar.y, 20, BLACK);  
        if(CheckCollisionPointRec(GetMousePosition(), voltar)){
            if(IsMouseButtonDown(MOUSE_LEFT_BUTTON)){
                return;
            }
        }

        EndDrawing();
    }
}//void jogar

void button_animation(Rectangle button, Color color){
    DrawRectangleRec(button, WHITE);
    if(CheckCollisionPointRec(GetMousePosition(), button)){
        DrawRectangleRec(button, LIGHTGRAY);
        if(IsMouseButtonDown(MOUSE_LEFT_BUTTON)){
            DrawRectangleRec(button, color);
        }
    }
}

int centralizar_X(const char *name_button, int fontSize){
    return (GetScreenWidth() - MeasureText(name_button, fontSize)) / 2;
}

void tutorial(){
    const char *tutorial[] = {
        "Vai começar o jogo do Milhão!",
        "Responda as pergunta escolhendo alguma das alternativas",
        "Você pode escolher entre as alternativas A, B, C ou D.",
        "Você tem direito a 2 dicas curtas durante o jogo.",
        "Para usar uma dica, clique no botão 'Receber Dica'."
    };
    int tempoEsperando = GetTime();
    while(!WindowShouldClose()){
        BeginDrawing();
        ClearBackground(WHITE);
        for(int i = 0; i < 5; i++){
            DrawText(tutorial[i], centralizar_X(tutorial[i], 20), 150 + i * 50, 20, BLACK);
        }  
        EndDrawing();
        if(GetTime() - tempoEsperando >= 7){
            return;
        }
    }
}

void jogo_encerrado(int respostaSelecionada, int valorSeguro, int valorGanho){
    char buffer[255];
    while(!WindowShouldClose()){
        BeginDrawing();
        ClearBackground(WHITE);

        if(respostaSelecionada == 0){
            sprintf(buffer, "Você ganhou: R$ %d Reais!", valorGanho);
            DrawText("Parabéns! Você respondeu todas as perguntas!", centralizar_X("Parabéns! Você respondeu todas as perguntas!", 20), 200, 20, BLACK);
            DrawText(buffer, centralizar_X(buffer, 20), 230, 20, BLACK);
            DrawText("Clique para voltar ao menu", centralizar_X("Clique para voltar ao menu", 20), 430, 20, BLACK);
        }else if(respostaSelecionada == 2){
            sprintf(buffer, "Você ganhou: R$ %d Reais!", valorSeguro);
            DrawText("Fim de Jogo. Tente Novamente!", centralizar_X("Fim de Joogo. Tente Novamente!", 20), 200, 20, BLACK);
            DrawText(buffer, centralizar_X(buffer, 20), 230, 20, BLACK);
            DrawText("Clique para voltar ao menu", centralizar_X("Clique para voltar ao menu", 20), 430, 20, BLACK);
        }

        EndDrawing();
        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)){
            return;
        }
    }
}
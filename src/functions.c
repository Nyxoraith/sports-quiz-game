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

void menu(Pergunta **perguntas, int *tam) {
    const char *nameButtons[] = {"Jogar", "Adicionar pergunta", "Listar perguntas", "Pesquisar perguntas", "Alterar perguntas", "Excluir perguntas", "Sair sem salvar", "Sair e salvar"};
    Texture2D logo_show = LoadTexture("resources/show.png");
    Rectangle play[8]; 

    for(int i = 0; i < 8; i++){
        play[i].x = centralizar_X(nameButtons[i], 20);
        play[i].y = 330 + i * 30;
        play[i].width = MeasureText(nameButtons[i], 20);
        play[i].height = 20;
    }

    while(!WindowShouldClose()){
        BeginDrawing();
        ClearBackground(WHITE);

        DrawTexture(logo_show, (GetScreenWidth() - logo_show.width)/2, 25, WHITE);
        DrawText("===== MENU =====", centralizar_X("===== MENU =====", 20), 280, 20, BLACK);
        for(int i = 0; i < 8; i++){
            button_animation(play[i], YELLOW);
            DrawText(nameButtons[i], play[i].x, play[i].y, 20, BLACK);  
        }

        EndDrawing();

        //Jogar
        if(CheckCollisionPointRec(GetMousePosition(), play[0])){
            if(IsMouseButtonReleased(MOUSE_LEFT_BUTTON)){
                jogar(*perguntas, *tam);
            }
        }

        //Inserir
        if(CheckCollisionPointRec(GetMousePosition(), play[1])){
            if(IsMouseButtonReleased(MOUSE_LEFT_BUTTON)){
                inserir(perguntas, tam);
            }
        }

        //Listar
        if(CheckCollisionPointRec(GetMousePosition(), play[2])){
            if(IsMouseButtonReleased(MOUSE_LEFT_BUTTON)){
                listar(*perguntas, *tam);
            }
        }

        //Pesquisar
        if(CheckCollisionPointRec(GetMousePosition(), play[3])){
            if(IsMouseButtonReleased(MOUSE_LEFT_BUTTON)){
                pesquisar(*perguntas, *tam);
            }
        }

        //Alterar
        if(CheckCollisionPointRec(GetMousePosition(), play[4])){
            if(IsMouseButtonReleased(MOUSE_LEFT_BUTTON)){
                alterar(*perguntas, *tam);
            }
        }
              
        //Excluir
        if(CheckCollisionPointRec(GetMousePosition(), play[5])){
            if(IsMouseButtonReleased(MOUSE_LEFT_BUTTON)){
                excluir(perguntas, tam);
            }
        }

        //Exit
        if(CheckCollisionPointRec(GetMousePosition(), play[6])){
            if(IsMouseButtonReleased(MOUSE_LEFT_BUTTON)){
                return;
            }
        }
        
        //Save & Exit
        if(CheckCollisionPointRec(GetMousePosition(), play[7])){
            if(IsMouseButtonReleased(MOUSE_LEFT_BUTTON)){
                salvar(*perguntas, *tam);
                return;
            }
        }
    }
    UnloadTexture(logo_show);
}    

void inserir(Pergunta **pergunta, int *tam){
    //Variaveis temporarias para exibição
    char buffer[256];
    char label[64];
    char enunciadoTmp[256];
    char altsTmp[4][64];
    char dicaTmp[256];
    char respostaTmp[64];
    char difTmp[64];
    char valorTmp[64];

    Rectangle back = {530, 480, MeasureText("Voltar ao menu!", 20), 20};

    //Inicio Alocações
    strcpy(label, "Digite o enunciado da pergunta:");
    if(ler_string(label, buffer, 200) == 1){
        return;
    }

    //Realoca o vetor
    *tam += 1;
    Pergunta *tmp = realloc(*pergunta, (*tam) * sizeof(Pergunta));
    if (tmp == NULL) {
        perror("Erro ao alocar espaço para nova pergunta: ");
        exit(1);
    }
    *pergunta = tmp;

    int i = *tam - 1;

    //Aloca o enunciado
    (*pergunta)[i].enunciado = malloc(strlen(buffer)+1);
    if((*pergunta)[i].enunciado == NULL){
        perror("Erro de alocação: ");
        exit(1);
    }
    strcpy((*pergunta)[i].enunciado, buffer);
    sprintf(enunciadoTmp, "Enunciado: %s", (*pergunta)[i].enunciado);

    //Alternativas
    char **alternativas[] = {&(*pergunta)[i].alt1, &(*pergunta)[i].alt2, &(*pergunta)[i].alt3, &(*pergunta)[i].alt4};
    for(int j = 0; j < 4; j++){
        sprintf(label, "Digite a alternativa %c: ", 'A' + j);
        if(ler_string(label, buffer, 200) == 1){
            for(int k = 0; k < j; k++){
                free(*alternativas[k]);
            }
            free((*pergunta)[i].enunciado);
            *tam -= 1;
            return;
        }

        *alternativas[j] = malloc(strlen(buffer)+1);
        if(*alternativas[j] == NULL){
            perror("Erro de alocação: ");
            exit(1);
        }
        strcpy(*alternativas[j], buffer);
        sprintf(altsTmp[j], "Alternativa %c: %s", 'A' + j, *alternativas[j]);
    }

    //Dica
    strcpy(label, "Digite a dica da pergunta:");
    if(ler_string(label, buffer, 200) == 1){
        for(int k = 0; k < 4; k++){
            free(*alternativas[k]);
        }
        free((*pergunta)[i].enunciado);
        *tam -= 1;
        return;
    }
    (*pergunta)[i].dica = malloc(strlen(buffer)+1);
    if((*pergunta)[i].dica == NULL){
        perror("Erro de alocação: ");
        exit(1);
    }
    strcpy((*pergunta)[i].dica, buffer);
    sprintf(dicaTmp, "Dica: %s", (*pergunta)[i].dica);

    //Reposta
    strcpy(label, "Digite qual é a alternativa correta (A)(B)(C)(D):");
    if(ler_string(label, buffer, 200) == 1){
        free((*pergunta)[i].dica);
        for(int k = 0; k < 4; k++){
            free(*alternativas[k]);
        }
        free((*pergunta)[i].enunciado);
        *tam -= 1;
        return;
    }
    (*pergunta)[i].resposta = buffer[0];
    sprintf(respostaTmp, "Resposta: %c", (*pergunta)[i].resposta);

    //Dificuldade
    strcpy(label, "Digite qual o nível de dificuldade da pergunta:");
    if(ler_string(label, buffer, 200) == 1){
        free((*pergunta)[i].dica);
        for(int k=0; k<4; k++) free(*alternativas[k]);
        free((*pergunta)[i].enunciado);
        *tam -= 1;
        return;
    }
    (*pergunta)[i].nivelDif = atoi(buffer);
    sprintf(difTmp, "Dificuldade: %d", (*pergunta)[i].nivelDif);

    //Valor
    strcpy(label, "Digite qual o valor da pergunta:");
    if(ler_string(label, buffer, 200) == 1){
        free((*pergunta)[i].dica);
        for(int k=0; k<4; k++) free(*alternativas[k]);
        free((*pergunta)[i].enunciado);
        *tam -= 1;
        return;
    }
    (*pergunta)[i].valor = atoi(buffer);
    sprintf(valorTmp, "Valor: %d", (*pergunta)[i].valor);

    //Fim Alocações

    while(!WindowShouldClose()){
        BeginDrawing();
        ClearBackground(WHITE);

        DrawText("Dados inseridos com sucesso!", centralizar_X("Dados inseridos com sucesso!", 20), 180, 20, BLACK);
        //Enunciado
        DrawText(enunciadoTmp, centralizar_X(enunciadoTmp, 20), 200, 20, BLACK);
        
        //Alternativas
        for(int j = 0; j < 4; j++){
            DrawText(altsTmp[j], centralizar_X(altsTmp[j], 20), 220 + j * 20, 20, BLACK);
        }

        //Dica
        DrawText(dicaTmp, centralizar_X(dicaTmp, 20), 300, 20, BLACK);
      
        //Reposta
        DrawText(respostaTmp, centralizar_X(respostaTmp, 20), 320, 20, BLACK);

        //Dificuldade
        DrawText(difTmp, centralizar_X(difTmp, 20), 340, 20, BLACK);

        //Valor
        DrawText(valorTmp, centralizar_X(valorTmp, 20), 360, 20, BLACK);

        //Botão para voltar ao menu
        button_animation(back, GOLD);
        DrawText("Voltar ao menu!", back.x, back.y, 20, BLACK);  

        EndDrawing();

        //Voltar com enter
        if(IsKeyPressed(KEY_ENTER)){
            return;
        }

        //Botão voltar
        if(CheckCollisionPointRec(GetMousePosition(), back)){
            if(IsMouseButtonReleased(MOUSE_LEFT_BUTTON)){
                return;
            }
        }
    }
}

void listar(Pergunta *pergunta, int tam){
    Rectangle back = {530, 480, MeasureText("Voltar ao menu!", 20), 20};
    Rectangle next = {centralizar_X("Proxima pergunta!", 20), 450, MeasureText("Proxima pergunta!", 20), 20};

    for(int i = 0; i < tam;){
        BeginDrawing();
        ClearBackground(WHITE);

        DrawText(TextFormat("Pergunta %d", i+1), centralizar_X(TextFormat("Pergunta %d", i+1), 20), 200, 20, BLACK);
        DrawText(TextFormat("Enunciado: %s", pergunta[i].enunciado), centralizar_X(TextFormat("Enunciado: %s", pergunta[i].enunciado), 20), 220, 20, BLACK);
        DrawText(TextFormat("Alternativas"), centralizar_X(TextFormat("Alternativas"), 20), 240, 20, BLACK);
        DrawText(TextFormat("[A] %s", pergunta[i].alt1), centralizar_X(TextFormat("[A] %s", pergunta[i].alt1), 20), 260, 20, BLACK);
        DrawText(TextFormat("[B] %s", pergunta[i].alt2), centralizar_X(TextFormat("[B] %s", pergunta[i].alt2), 20), 280, 20, BLACK);
        DrawText(TextFormat("[C] %s", pergunta[i].alt3), centralizar_X(TextFormat("[C] %s", pergunta[i].alt3), 20), 300, 20, BLACK);
        DrawText(TextFormat("[D] %s", pergunta[i].alt4), centralizar_X(TextFormat("[D] %s", pergunta[i].alt4), 20), 320, 20, BLACK);
        DrawText(TextFormat("Alternativa correta: %c", pergunta[i].resposta), centralizar_X(TextFormat("Alternativa correta: %c", pergunta[i].resposta), 20), 340, 20, BLACK);
        DrawText(TextFormat("Nível de dificuldade: %d", pergunta[i].nivelDif), centralizar_X(TextFormat("Nível de dificuldade: %d", pergunta[i].nivelDif), 20), 360, 20, BLACK);
        DrawText(TextFormat("Dica: %s", pergunta[i].dica), centralizar_X(TextFormat("Dica: %s", pergunta[i].dica), 20), 380, 20, BLACK);
        DrawText(TextFormat("Valor: %d", pergunta[i].valor), centralizar_X(TextFormat("Valor: %d", pergunta[i].valor), 20), 400, 20, BLACK);

        //Botão para avançar apara proxima pergunta
        if(i + 1 < tam){
            button_animation(next, GOLD);
            DrawText("Proxima pergunta!", next.x, next.y, 20, BLACK);

            //Avançar
            if(CheckCollisionPointRec(GetMousePosition(), next)){
                if(IsMouseButtonReleased(MOUSE_BUTTON_LEFT)){
                    i++;
                }
            }
        }

        //Botão para voltar ao menu
        button_animation(back, GOLD);
        DrawText("Voltar ao menu!", back.x, back.y, 20, BLACK);  
        
        EndDrawing();

        //Voltar
        if(CheckCollisionPointRec(GetMousePosition(), back)){
            if(IsMouseButtonReleased(MOUSE_LEFT_BUTTON)){
                return;
            }
        }

    }
}

int pesquisar(Pergunta *pergunta, int tam){
    char search[256];
    char label[64];
    strcpy(label, "Digite o enunciado da questão que deseja pesquisar:");
    if(ler_string(label, search, 200) == 1){
        return -1;
    }

    for(int i = 0; i < tam; i++){
        if(strcmp(pergunta[i].enunciado, search) == 0){
            BeginDrawing();
            ClearBackground(WHITE);

            DrawText("Questão encontrada:", centralizar_X("Questão encontrada:", 20), 200, 20, BLACK);
            DrawText(TextFormat("%s", pergunta[i].enunciado), centralizar_X(TextFormat("%s", pergunta[i].enunciado), 20), 220, 20, BLACK);
            DrawText("Alternativas", centralizar_X("Alternativas", 20), 240, 20, BLACK);
            DrawText(TextFormat("[A] %s", pergunta[i].alt1), centralizar_X(TextFormat("[A] %s", pergunta[i].alt1), 20), 260, 20, BLACK);
            DrawText(TextFormat("[B] %s", pergunta[i].alt2), centralizar_X(TextFormat("[B] %s", pergunta[i].alt2), 20), 280, 20, BLACK);
            DrawText(TextFormat("[C] %s", pergunta[i].alt3), centralizar_X(TextFormat("[C] %s", pergunta[i].alt3), 20), 300, 20, BLACK);
            DrawText(TextFormat("[D] %s", pergunta[i].alt4), centralizar_X(TextFormat("[D] %s", pergunta[i].alt4), 20), 320, 20, BLACK);
            DrawText(TextFormat("Alternativa correta: %c", pergunta[i].resposta), centralizar_X(TextFormat("Alternativa correta: %c", pergunta[i].resposta), 20), 340, 20, BLACK);
            DrawText(TextFormat("Nível de dificuldade: %d", pergunta[i].nivelDif), centralizar_X(TextFormat("Nível de dificuldade: %d", pergunta[i].nivelDif), 20), 360, 20, BLACK);
            DrawText(TextFormat("Dica: %s", pergunta[i].dica), centralizar_X(TextFormat("Dica: %s", pergunta[i].dica), 20), 380, 20, BLACK);
            DrawText(TextFormat("Valor: %d", pergunta[i].valor), centralizar_X(TextFormat("Valor: %d", pergunta[i].valor), 20), 400, 20, BLACK);
            EndDrawing();

            WaitTime(5);
            return i;
        }
    }
    BeginDrawing();
    ClearBackground(WHITE);

    DrawText("Questão não encontrada", centralizar_X("Questão não encontrada", 20), 200, 20, BLACK);
    EndDrawing();

    WaitTime(2);
    return -1;
}

void alterar(Pergunta *pergunta, int tam){
    int indice;
    int difTmp;
    int valorTmp;
    char buffer[256];
    char label[64];
    char enunciadoTmp[256];
    char altsTmp[4][64];
    char dicaTmp[256];
    char respostaTmp;

    indice = pesquisar(pergunta, tam);
    if(indice == -1){
        return;
    }
    
    char **alternativas[] = {&pergunta[indice].alt1, &pergunta[indice].alt2, &pergunta[indice].alt3, &pergunta[indice].alt4};

    strcpy(enunciadoTmp, pergunta[indice].enunciado);
    for(int i = 0; i < 4; i++){
        strcpy(altsTmp[i], *alternativas[i]);
    }
    strcpy(dicaTmp, pergunta[indice].dica);
    difTmp = pergunta[indice].nivelDif;
    valorTmp = pergunta[indice].valor;
    

    strcpy(label, "Digite o novo enunciado");
    if(ler_string(label, buffer, 200) == 1){
        return;
    }

    pergunta[indice].enunciado = realloc(pergunta[indice].enunciado, strlen(buffer)+1);
    if(pergunta[indice].enunciado == NULL){
        perror("Erro de alocação: ");
        exit(1);
    }
    strcpy(pergunta[indice].enunciado, buffer);
    
    for(int i = 0; i < 4; i++){
        sprintf(label, "Digite a alternativa %c: ", 'A' + i);
        if(ler_string(label, buffer, 200) == 1){
            for(int j = 0; j < i; j++){
                strcpy(*alternativas[j], altsTmp[j]);
                printf("%s - %s\n", altsTmp[j], *alternativas[j]);
            }
            strcpy(pergunta[indice].enunciado, enunciadoTmp);
            return;
        }
        *alternativas[i] = realloc(*alternativas[i], strlen(buffer)+1);
        if(*alternativas[i] == NULL){
            perror("Erro de alocação: ");
            exit(1);
        }
        strcpy(*alternativas[i], buffer);
    }
    
    strcpy(label, "Digite a dica da pergunta:");
    if(ler_string(label, buffer, 200) == 1){
        for(int i = 0; i < 4; i++){
            strcpy(*alternativas[i], altsTmp[i]);
        }
        strcpy(pergunta[indice].enunciado, enunciadoTmp);
        return;
    }
    
    pergunta[indice].dica = realloc(pergunta[indice].dica, strlen(buffer)+1);
    if(pergunta[indice].dica == NULL){
        perror("Erro de alocação: ");
        exit(1);
    }
    strcpy(pergunta[indice].dica, buffer);
    
    strcpy(label,"Digite a alternativa correta (A)(B)(C)(D):");
    if(ler_string(label, buffer, 200) == 1){
        for(int i = 0; i < 4; i++){
            strcpy(*alternativas[i], altsTmp[i]);
        }
        strcpy(pergunta[indice].enunciado, enunciadoTmp);
        strcpy(pergunta[indice].dica, dicaTmp);
        return;
    }
    pergunta[indice].resposta = buffer[0];

    strcpy(label, "Digite a dificuldade:");
    if(ler_string(label, buffer, 200) == 1){
        for(int i = 0; i < 4; i++){
            strcpy(*alternativas[i], altsTmp[i]);
        }
        strcpy(pergunta[indice].enunciado, enunciadoTmp);
        strcpy(pergunta[indice].dica, dicaTmp);
        pergunta[indice].resposta = respostaTmp;
        return;
    }
    pergunta[indice].nivelDif = atoi(buffer);
    
    strcpy(label, "Digite qual o valor da pergunta:");
    if(ler_string(label, buffer, 200) == 1){
        for(int i = 0; i < 4; i++){
            strcpy(*alternativas[i], altsTmp[i]);
        }
        strcpy(pergunta[indice].enunciado, enunciadoTmp);
        strcpy(pergunta[indice].dica, dicaTmp);
        pergunta[indice].resposta = respostaTmp;
        pergunta[indice].nivelDif = difTmp;
        return;
    }
    pergunta[indice].valor = atoi(buffer);

    BeginDrawing();
    ClearBackground(WHITE);
    DrawText("Dados inseridos com sucesso!", centralizar_X("Dados inseridos com sucesso!", 20), 200, 20, BLACK);
    EndDrawing();
    WaitTime(3);
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
    
    BeginDrawing();
    ClearBackground(WHITE);
    DrawText("Pergunta excluida com sucesso!", centralizar_X("Pergunta excluida com sucesso!", 20), 200, 20, BLACK);
    EndDrawing();
    WaitTime(3);
}

void salvar(Pergunta *pergunta, int tam){
    FILE *fp = fopen("resources/questoes.csv", "w");
    if (fp == NULL) {
        perror("Erro ao abrir o arquivo para salvar");
        return;
    }

    for (int i = 0; i < tam; i++) {
        fprintf(fp, "%s,%s,%s,%s,%s,%c,%d,%s,%d\n", pergunta[i].enunciado, pergunta[i].alt1, pergunta[i].alt2, pergunta[i].alt3, pergunta[i].alt4, pergunta[i].resposta, pergunta[i].nivelDif, pergunta[i].dica, pergunta[i].valor);
    }

    fclose(fp);
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

    int play = 1;
    int marco1 = 5; // pergunta 5
    int marco2 = 10; // pergunta 10
    int gameFinal = 0;
    int dicaUsada = 0;
    int muteControl = 0;
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
    Rectangle back = {550, 530, MeasureText("Voltar ao menu!", 20), 20};
    Rectangle alt_buttons[4];
    Rectangle dicas = {100, 320, MeasureText("Receber dica",20), 20};
    Rectangle container_Enum = {50, 50, 700, 50};
    Rectangle container_valores = {430, 310, 305, 100};
    Rectangle bt_audio = {15, 15, 32, 32};

    Texture2D mute = LoadTexture("resources/mute.png");
    Texture2D unmute = LoadTexture("resources/unmute.png");

    Sound continue_sound = LoadSound("resources/continue.mp3");
    Sound fail_sound = LoadSound("resources/fail.mp3");
    
    SetMasterVolume(1);
    while(!WindowShouldClose()){
    
        char *alternativas[] = {pergunta[perguntaAtual].alt1, pergunta[perguntaAtual].alt2, pergunta[perguntaAtual].alt3, pergunta[perguntaAtual].alt4};
        for(int i = 0; i < 4; i++){
            sprintf(buffer, "%c) %s", 'A' + i, alternativas[i]);
            alt_buttons[i].x = centralizar_X(alternativas[i], 20);
            alt_buttons[i].y = 130 + i * 30;
            alt_buttons[i].width = MeasureText(buffer, 20);
            alt_buttons[i].height = 20;
        }

        BeginDrawing();
        ClearBackground(WHITE);

        if(muteControl == 0){
            DrawTexture(unmute, bt_audio.x, bt_audio.y, WHITE);
           
        }else{
            DrawTexture(mute, bt_audio.x, bt_audio.y, WHITE);
        }

        DrawRectangleLinesEx(container_Enum, 3, MAROON);
        DrawRectangleLinesEx(container_valores, 3, MAROON);
        //Enunciado
        DrawText(pergunta[perguntaAtual].enunciado, centralizar_X(pergunta[perguntaAtual].enunciado, 20), 65, 20, BLACK);

        //Questoes
        for(int i = 0; i < 4; i++){
            char opcao = 'A' + i;
            if(opcao == pergunta[perguntaAtual].resposta){
                button_animation(alt_buttons[i], GREEN);
            }else{
                button_animation(alt_buttons[i], RED);
            }
            
            sprintf(buffer, "%c) %s", 'A' + i, alternativas[i]);
            DrawText(buffer, centralizar_X(alternativas[i], 20), alt_buttons[i].y, 20, BLACK);

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
            DrawText("Resposta correta!", centralizar_X("Reposta correta!", 20), 260, 20, GREEN);
            dinheiroGanho = pergunta[perguntaAtual].valor;
            if(play == 1){
                PlaySound(continue_sound);   
                play = 0;
            }
        }else if(respostaSelecionada == 2){
            DrawText("Resposta Incorreta!", centralizar_X("Reposta Incorreta!", 20), 260, 20, RED);
            if(play == 1){
                PlaySound(fail_sound);   
                play = 0;
            }
        }

        //Mostra o valor que a pergunta vale
        DrawText("Valendo: ", 450, 320, 20, BLACK);
        sprintf(buffer, "R$ %d",pergunta[perguntaAtual].valor);
        DrawText(buffer, 600, 320, 20, BLACK);

        //Mostra o valor que ganha se Acertar
        DrawText("Valor Ganho: ", 450, 350, 20, BLACK);
        sprintf(buffer, "R$ %d", dinheiroGanho);
        DrawText(buffer, 600, 350, 20, GREEN);

        //Mostra o valor que ganha se errar
        DrawText("Se errar: ", 450, 380, 20, BLACK);
        sprintf(buffer, "R$ %d", valorSeguro);
        DrawText(buffer, 600, 380, 20, RED);

        //Faz a validação das dicas disponiveis
        if(dicasDisponiveis > 0 && strlen(pergunta[perguntaAtual].dica) > 0){
            sprintf(buffer, "%d", dicasDisponiveis);
            if(!dicaUsada){
                button_animation(dicas, GOLD);
            }else{
                button_animation(dicas, RED);
            }
            DrawText("Receber dica", 100, dicas.y, 20, BLACK);
            DrawText("Dicas Disponiveis:", 100, 350, 20, BLACK); 
            DrawText(buffer, 280, 350, 20, BLACK);
            if(CheckCollisionPointRec(GetMousePosition(), dicas)){
                if(IsMouseButtonPressed(MOUSE_LEFT_BUTTON)){
                    if(recebeuDicaNaVez == 0){
                        recebeuDicaNaVez++;
                        dicasDisponiveis--;
                        DrawText(buffer, 280, 350, 20, BLACK);
                    }
                    else{
                        dicaUsada = 1;
                    }
                }   
            }
        }else if((dicasDisponiveis == 0 || strlen(pergunta[perguntaAtual].dica) == 0) && recebeuDicaNaVez == 0){
            DrawText("Você não tem mais dicas disponíveis ou esta pergunta não tem dica.", centralizar_X("Você não tem mais dicas disponíveis ou esta pergunta não tem dica.", 20), 450, 20, RED);
        }

        //Exibe o texto que já foi utilizado dica na pergunta
        if (recebeuDicaNaVez == 1) {
            DrawText(pergunta[perguntaAtual].dica, centralizar_X(pergunta[perguntaAtual].dica, 20), 480, 20, GREEN);
           if (dicaUsada == 1) {
                DrawText("Você já usou a dica para esta pergunta.", centralizar_X("Você já usou a dica para esta pergunta.", 20), 430, 20, RED);
            }
        }

        //Botão para voltar ao menu
        button_animation(back, GOLD);
        DrawText("Voltar ao menu!", back.x, back.y, 20, BLACK);  

        EndDrawing();

        //Guarda valores da pergunta 5 e pergunta 10 caso erre a próxima questão.
        if(perguntaAtual >= marco2){
            valorSeguro = pergunta[marco2 - 1].valor;
        }else if(perguntaAtual >= marco1){
            valorSeguro = pergunta[marco1 - 1].valor;
        }else{
            valorSeguro = 0;
        }

        //Cria um delay entre uma pergunta e outra
        if((respostaSelecionada == 1 || respostaSelecionada == 2) && !esperandoProxPergunta){
            esperandoProxPergunta = 1;
            tempoEsperando = GetTime();
        }

        //Avança Pergunta
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
                play = 1;
                perguntaAtual++;

                //Se respondeu todas as perguntas corretas
                if(perguntaAtual >= total){
                    jogo_encerrado(respostaSelecionada, valorSeguro, dinheiroGanho);
                    break;
                }
            }
        }

         //Mutar
        if(CheckCollisionPointRec(GetMousePosition(), bt_audio)){
            if(IsMouseButtonPressed(MOUSE_LEFT_BUTTON)){
                if(muteControl == 0){
                    SetMasterVolume(0);
                    muteControl = 1;
                }else{
                    SetMasterVolume(1);
                    muteControl = 0;
                }
            }
        }

        //Botão voltar
        if(CheckCollisionPointRec(GetMousePosition(), back)){
            if(IsMouseButtonReleased(MOUSE_LEFT_BUTTON)){
                UnloadSound(fail_sound);
                UnloadSound(continue_sound);
                return;
            }
        }
    }
}

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
    int play = 1;
    int contadorLetra = 0;
    int linhaAtual = 0;
    const char *tutorial[] = {
        "Vai começar o jogo do Milhão!",
        "Responda as pergunta escolhendo alguma das alternativas",
        "Você pode escolher entre as alternativas A, B, C ou D.",
        "Você tem direito a 2 dicas curtas durante o jogo.",
        "Para usar uma dica, clique no botão 'Receber Dica'."
    };
    int tempoEsperando = GetTime();
    Sound intro = LoadSound("resources/intro.mp3");
    while(!WindowShouldClose()){
        BeginDrawing();
        ClearBackground(WHITE);

        if(play == 1){
            PlaySound(intro);   
            play = 0;
        }

        for(int i = 0; i < linhaAtual; i++){
            DrawText(tutorial[i], centralizar_X(tutorial[i], 20), 180 + i * 50, 20, BLACK);
        }
        
        if(linhaAtual < 5){
            int comprimento = contadorLetra / 2;
            int tam = strlen(tutorial[linhaAtual]);
            if(comprimento > tam){
                comprimento = tam;
            }

            DrawText(TextSubtext(tutorial[linhaAtual], 0, comprimento), centralizar_X(tutorial[linhaAtual], 20), 180 + linhaAtual * 50, 20, BLACK);

            if(comprimento >= tam){
                linhaAtual++;
                contadorLetra = 0;
            }else{
                contadorLetra++;
            }

        }

        EndDrawing();
        if(GetTime() - tempoEsperando >= 10){
            UnloadSound(intro);
            return;
        }
    }
}

void jogo_encerrado(int respostaSelecionada, int valorSeguro, int valorGanho){
    char buffer[256];
    Sound win_sound = LoadSound("resources/win.mp3");
    int play = 1;
    while(!WindowShouldClose()){
        BeginDrawing();
        ClearBackground(WHITE);

        if(respostaSelecionada == 0){
            if(play == 1){
                PlaySound(win_sound);   
                play = 0;
            }
            sprintf(buffer, "Você ganhou: R$ %d Reais!", valorGanho);
            DrawText("Parabéns! Você respondeu todas as perguntas!", centralizar_X("Parabéns! Você respondeu todas as perguntas!", 20), 200, 20, GREEN);
            DrawText(buffer, centralizar_X(buffer, 20), 230, 20, BLACK);
            DrawText("Clique para voltar ao menu", centralizar_X("Clique para voltar ao menu", 20), 430, 20, BLACK);
        }else if(respostaSelecionada == 2){
            sprintf(buffer, "Você ganhou: R$ %d Reais!", valorSeguro);
            DrawText("Fim de Jogo. Tente Novamente!", centralizar_X("Fim de Joogo. Tente Novamente!", 20), 200, 20, RED);
            DrawText(buffer, centralizar_X(buffer, 20), 230, 20, BLACK);
            DrawText("Clique para voltar ao menu", centralizar_X("Clique para voltar ao menu", 20), 430, 20, BLACK);
            
        }
        EndDrawing();
        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)){
            return;
        }
    }
    UnloadSound(win_sound);
}

int ler_string(char *label, char *input, int posY){
    int contadorLetras = 0;
    input[0]='\0';
    Rectangle back = {530, 480, MeasureText("Voltar ao menu!", 20), 20};
    Rectangle next = {centralizar_X("Avançar!", 20), posY + 80, MeasureText("Avançar!", 20), 20};

    while(!WindowShouldClose()){
        int key = GetCharPressed();
    
        BeginDrawing();
        ClearBackground(WHITE);

        DrawText(label, centralizar_X(label, 20), posY, 20, BLACK);
        DrawText(input, centralizar_X(input, 20), posY + 40, 20, BLACK);
        
        //Botão para avançar
        button_animation(next, GOLD);
        DrawText("Avançar!", next.x, next.y, 20, BLACK);

        //Botão para voltar ao menu
        button_animation(back, GOLD);
        DrawText("Voltar ao menu!", back.x, back.y, 20, BLACK);  
        
        EndDrawing();

        //Pega as letras digitadas
        while(key > 0){
            if(key >= 32 && key <= 125 && contadorLetras < 255){
                input[contadorLetras] = (char)key;
                contadorLetras++;
                input[contadorLetras] = '\0';
            }
            key = GetCharPressed();
        }

        //Apagar
        if(IsKeyPressed(KEY_BACKSPACE)){
            contadorLetras--;
            if(contadorLetras < 0){
                contadorLetras = 0;
            }
            input[contadorLetras] = '\0';
        }

        //Para avançar
        if(IsKeyPressed(KEY_ENTER)){
            return 0;
        }

        //Botão Avançar
        if(CheckCollisionPointRec(GetMousePosition(), next)){
            if(IsMouseButtonReleased(MOUSE_LEFT_BUTTON)){
                return 0;
            }
        }

        //Botão cancelar
        if(CheckCollisionPointRec(GetMousePosition(), back)){
            if(IsMouseButtonReleased(MOUSE_LEFT_BUTTON)){
                return 1;
            }
        }
    }
    return 1;
}
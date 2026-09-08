#include "raylib.h"
#include <stdlib.h>
#include <time.h>

#define LARGURA 800
#define ALTURA 600
#define TAM_CELULA 40
#define LINHAS (ALTURA / TAM_CELULA)
#define COLUNAS (LARGURA / TAM_CELULA)

typedef struct {
    Vector2 pos;
    Vector2 vel;
    float raio;
    Color cor;
} Bola;

int **criarMatriz(int linhas, int colunas) {
    int **matriz = malloc(linhas * sizeof(int *));

    for (int i = 0; i < linhas; i++) {
        matriz[i] = malloc(colunas * sizeof(int));

        for (int j = 0; j < colunas; j++) {
            matriz[i][j] = 0;
        }
    }

    return matriz;
}

void liberarMatriz(int **matriz, int linhas) {
    for (int i = 0; i < linhas; i++) {
        free(matriz[i]);
    }

    free(matriz);
}

void desenharMatriz(int **matriz) {
    for (int i = 0; i < LINHAS; i++) {
        for (int j = 0; j < COLUNAS; j++) {
            Rectangle celula = {
                j * TAM_CELULA,
                i * TAM_CELULA,
                TAM_CELULA,
                TAM_CELULA
            };

            if (matriz[i][j] == 1) {
                DrawRectangleRec(celula, LIGHTGRAY);
            }

            DrawRectangleLinesEx(celula, 1, DARKGRAY);
        }
    }
}

Bola *criarBolas(int quantidade) {
    Bola *bolas = malloc(quantidade * sizeof(Bola));

    for (int i = 0; i < quantidade; i++) {
        bolas[i].pos = (Vector2){
            GetRandomValue(50, LARGURA - 50),
            GetRandomValue(50, ALTURA - 50)
        };

        bolas[i].vel = (Vector2){
            GetRandomValue(-3, 3),
            GetRandomValue(-3, 3)
        };

        bolas[i].raio = 10;
        bolas[i].cor = RED;
    }

    return bolas;
}

void atualizarBola(Bola *b) {
    b->pos.x += b->vel.x;
    b->pos.y += b->vel.y;

    if (b->pos.x - b->raio < 0 || b->pos.x + b->raio > LARGURA) {
        b->vel.x *= -1;
    }

    if (b->pos.y - b->raio < 0 || b->pos.y + b->raio > ALTURA) {
        b->vel.y *= -1;
    }
}

int main() {
    InitWindow(LARGURA, ALTURA, "Atividade 2");
    SetTargetFPS(60);

    srand(time(NULL));

    int **matriz = criarMatriz(LINHAS, COLUNAS);

    int quantidadeBolas = 12;
    Bola *bolas = criarBolas(quantidadeBolas);

    while (!WindowShouldClose()) {

        for (int i = 0; i < quantidadeBolas; i++) {
            atualizarBola(&bolas[i]);

            int coluna = (int)(bolas[i].pos.x / TAM_CELULA);
            int linha = (int)(bolas[i].pos.y / TAM_CELULA);

            if (linha >= 0 && linha < LINHAS &&
                coluna >= 0 && coluna < COLUNAS) {
                matriz[linha][coluna] = 1;
            }
        }

        int visitadas = 0;

        for (int i = 0; i < LINHAS; i++) {
            for (int j = 0; j < COLUNAS; j++) {
                if (matriz[i][j] == 1) {
                    visitadas++;
                }
            }
        }

        BeginDrawing();

        ClearBackground(BLACK);

        desenharMatriz(matriz);

        for (int i = 0; i < quantidadeBolas; i++) {
            DrawCircleV(bolas[i].pos, bolas[i].raio, bolas[i].cor);
        }

        DrawText(
            TextFormat("Celulas visitadas: %d", visitadas),
            10, 10, 20, WHITE
        );

        DrawText(
            "Pressione ESC para sair",
            10, 35, 20, WHITE
        );

        EndDrawing();
    }

    free(bolas);
    liberarMatriz(matriz, LINHAS);

    CloseWindow();

    return 0;
}

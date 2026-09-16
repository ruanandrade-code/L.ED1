#include "raylib.h"
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#define LARGURA_JANELA 800
#define ALTURA_JANELA 600

#define TOTAL_INIMIGOS 5
#define VIDA_INICIAL 60
#define DANO_TIRO 20
#define CURA 10

typedef enum {
    INIMIGO_VIVO,
    INIMIGO_MORTO
} EstadoInimigo;

typedef struct {
    Vector2 pos;
    float raio;
    int vida;
    EstadoInimigo estado;
} Inimigo;

void inicializarInimigos(Inimigo *vetor, int n)
{
    Vector2 posicoes[] = {
        {150, 150},
        {400, 120},
        {650, 180},
        {250, 400},
        {600, 420}
    };

    for (int i = 0; i < n; i++)
    {
        Inimigo *ini = vetor + i;

        ini->pos = posicoes[i];
        ini->raio = 25;
        ini->vida = VIDA_INICIAL;
        ini->estado = INIMIGO_VIVO;
    }
}

void atingirInimigo(Inimigo *inimigo, int dano)
{
    if (inimigo == NULL ||
        inimigo->estado == INIMIGO_MORTO)
    {
        return;
    }

    inimigo->vida -= dano;

    if (inimigo->vida <= 0)
    {
        inimigo->vida = 0;
        inimigo->estado = INIMIGO_MORTO;
    }
}

Inimigo *encontrarInimigoMaisProximo(
    Inimigo *vetor,
    int n,
    Vector2 posJogador)
{
    Inimigo *maisProximo = NULL;
    float menorDistancia = 0.0f;

    for (int i = 0; i < n; i++)
    {
        Inimigo *ini = vetor + i;

        if (ini->estado == INIMIGO_MORTO)
        {
            continue;
        }

        float dx = ini->pos.x - posJogador.x;
        float dy = ini->pos.y - posJogador.y;

        float distancia = sqrtf(dx * dx + dy * dy);

        if (maisProximo == NULL ||
            distancia < menorDistancia)
        {
            maisProximo = ini;
            menorDistancia = distancia;
        }
    }

    return maisProximo;
}

void curarTodos(Inimigo *vetor, int n, int cura)
{
    for (int i = 0; i < n; i++)
    {
        Inimigo *ini = vetor + i;

        if (ini->estado == INIMIGO_MORTO)
        {
            continue;
        }

        ini->vida += cura;

        if (ini->vida > VIDA_INICIAL)
        {
            ini->vida = VIDA_INICIAL;
        }
    }
}

Inimigo *encontrarInimigoMaisFraco(
    Inimigo *vetor,
    int n)
{
    Inimigo *maisFraco = NULL;

    for (int i = 0; i < n; i++)
    {
        Inimigo *ini = vetor + i;

        if (ini->estado == INIMIGO_MORTO)
        {
            continue;
        }

        if (maisFraco == NULL ||
            ini->vida < maisFraco->vida)
        {
            maisFraco = ini;
        }
    }

    return maisFraco;
}

void desenharInimigos(Inimigo *vetor, int n)
{
    for (int i = 0; i < n; i++)
    {
        Inimigo *ini = vetor + i;

        if (ini->estado == INIMIGO_MORTO)
        {
            DrawCircleV(ini->pos, ini->raio, DARKGRAY);

            DrawText(
                "MORTO",
                ini->pos.x - 30,
                ini->pos.y - 5,
                12,
                WHITE
            );

            continue;
        }

        DrawCircleV(
            ini->pos,
            ini->raio,
            RED
        );
      
        DrawRectangle(
            ini->pos.x - 30,
            ini->pos.y - 45,
            60,
            8,
            DARKGRAY
        );

        DrawRectangle(
            ini->pos.x - 30,
            ini->pos.y - 45,
            ini->vida,
            8,
            GREEN
        );

        DrawText(
            TextFormat("%d", ini->vida),
            ini->pos.x - 10,
            ini->pos.y - 65,
            14,
            WHITE
        );
    }
}

int main(void)
{
    InitWindow(
        LARGURA,
        ALTURA,
        "Atividade 4 - Ponteiros para Struct"
    );

    SetTargetFPS(60);

    Inimigo *inimigos =
        (Inimigo *)malloc(
            TOTAL_INIMIGOS * sizeof(Inimigo)
        );

    if (inimigos == NULL)
    {
        printf("Erro ao alocar memoria.\n");

        CloseWindow();

        return 1;
    }

    inicializarInimigos(
        inimigos,
        TOTAL_INIMIGOS
    );

    Vector2 jogador = {
        LARGURA / 2.0f,
        ALTURA / 2.0f
    };


    while (!WindowShouldClose())
    {
    
        if (IsKeyDown(KEY_W))
        {
            jogador.y -= 4;
        }

        if (IsKeyDown(KEY_S))
        {
            jogador.y += 4;
        }

        if (IsKeyDown(KEY_A))
        {
            jogador.x -= 4;
        }

        if (IsKeyDown(KEY_D))
        {
            jogador.x += 4;
        }

        if (IsKeyPressed(KEY_SPACE))
        {
            Inimigo *alvo =
                encontrarInimigoMaisFraco(
                    inimigos,
                    TOTAL_INIMIGOS
                );

            atingirInimigo(
                alvo,
                DANO_TIRO
            );
        }

        if (IsKeyPressed(KEY_C))
        {
            curarTodos(
                inimigos,
                TOTAL_INIMIGOS,
                CURA
            );
        }

        BeginDrawing();

        ClearBackground(BLACK);

        DrawText(
            "WASD - mover",
            20,
            20,
            20,
            WHITE
        );

        DrawText(
            "ESPACO - atacar inimigo mais fraco",
            20,
            50,
            20,
            WHITE
        );

        DrawText(
            "C - curar todos",
            20,
            80,
            20,
            WHITE
        );

        DrawCircleV(
            jogador,
            20,
            BLUE
        );

        DrawText(
            "JOGADOR",
            jogador.x - 35,
            jogador.y + 30,
            12,
            WHITE
        );

        desenharInimigos(
            inimigos,
            TOTAL_INIMIGOS
        );


        EndDrawing();
    }

    free(inimigos);

    CloseWindow();

    return 0;
}

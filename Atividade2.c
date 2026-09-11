#include "raylib.h"
#include <stdlib.h>
#include <stdbool.h>

#define LARGURA 800
#define ALTURA 600

typedef enum {
    MOEDA_BRONZE,
    MOEDA_PRATA,
    MOEDA_OURO,
    MOEDA_DIAMANTE
} TipoMoeda;

typedef struct {
    Vector2 pos;
    float raio;
    TipoMoeda tipo;
    int valor;
    bool coletada;
    float tempoColeta;
} Moeda;

Color corDaMoeda(TipoMoeda tipo)
{
    switch (tipo)
    {
        case MOEDA_BRONZE:
            return (Color){160, 90, 40, 255};

        case MOEDA_PRATA:
            return (Color){190, 190, 190, 255};

        case MOEDA_OURO:
            return GOLD;

        case MOEDA_DIAMANTE:
            return SKYBLUE;

        default:
            return WHITE;
    }
}

int valorDaMoeda(TipoMoeda tipo)
{
    switch (tipo)
    {
        case MOEDA_BRONZE:
            return 5;

        case MOEDA_PRATA:
            return 10;

        case MOEDA_OURO:
            return 25;

        case MOEDA_DIAMANTE:
            return 50;

        default:
            return 0;
    }
}

Moeda *criarMoedas(int quantidade)
{
    Moeda *moedas = (Moeda *)malloc(quantidade * sizeof(Moeda));

    if (moedas == NULL)
    {
        return NULL;
    }

    for (int i = 0; i < quantidade; i++)
    {
        Moeda *m = moedas + i;

        m->pos = (Vector2){
            GetRandomValue(30, LARGURA_JANELA - 30),
            GetRandomValue(30, ALTURA_JANELA - 30)
        };

        m->raio = 10.0f;
      
        int sorteioDiamante = GetRandomValue(0, 9);

        if (sorteioDiamante == 0)
        {
            m->tipo = MOEDA_DIAMANTE;
        }
        else
        {

            m->tipo = (TipoMoeda)GetRandomValue(
                MOEDA_BRONZE,
                MOEDA_OURO
            );
        }

        m->valor = valorDaMoeda(m->tipo);
        m->coletada = false;
        m->tempoColeta = 0.0f;
    }

    return moedas;
}

bool tentarColetar(Moeda *m, Vector2 posJogador, float raioJogador)
{
    if (m->coletada)
    {
        return false;
    }

    float dx = m->pos.x - posJogador.x;
    float dy = m->pos.y - posJogador.y;

    float distancia = (dx * dx) + (dy * dy);

    float somaRaios = (m->raio + raioJogador) *
                       (m->raio + raioJogador);

    if (distancia <= somaRaios)
    {
        m->coletada = true;

        m->tempoColeta = GetTime();

        return true;
    }

    return false;
}

void desenharMoeda(const Moeda *m)
{
    if (!m->coletada)
    {
        Color cor = corDaMoeda(m->tipo);

        DrawCircleV(m->pos, m->raio, cor);

        DrawCircle(
            (int)m->pos.x - 3,
            (int)m->pos.y - 3,
            2,
            WHITE
        );
    }
}

void novaPosicaoMoeda(Moeda *m)
{
    m->pos = (Vector2){
        GetRandomValue(30, LARGURA_JANELA - 30),
        GetRandomValue(30, ALTURA_JANELA - 30)
    };
}

int main(void)
{
    InitWindow(
        LARGURA_JANELA,
        ALTURA_JANELA,
        "Atividade 2 - Moedas"
    );

    SetTargetFPS(60);

    int quantidadeMoedas = 12;

    Moeda *moedas = criarMoedas(quantidadeMoedas);

    if (moedas == NULL)
    {
        CloseWindow();
        return 1;
    }

    Vector2 posJogador = {
        LARGURA_JANELA / 2.0f,
        ALTURA_JANELA / 2.0f
    };

    float raioJogador = 15.0f;
    float velocidade = 4.0f;

    int pontuacao = 0;

    while (!WindowShouldClose())
    {
        // Movimento do jogador
        if (IsKeyDown(KEY_W))
        {
            posJogador.y -= velocidade;
        }

        if (IsKeyDown(KEY_S))
        {
            posJogador.y += velocidade;
        }

        if (IsKeyDown(KEY_A))
        {
            posJogador.x -= velocidade;
        }

        if (IsKeyDown(KEY_D))
        {
            posJogador.x += velocidade;
        }

        if (posJogador.x < raioJogador)
        {
            posJogador.x = raioJogador;
        }

        if (posJogador.x > LARGURA_JANELA - raioJogador)
        {
            posJogador.x = LARGURA_JANELA - raioJogador;
        }

        if (posJogador.y < raioJogador)
        {
            posJogador.y = raioJogador;
        }

        if (posJogador.y > ALTURA_JANELA - raioJogador)
        {
            posJogador.y = ALTURA_JANELA - raioJogador;
        }

        for (int i = 0; i < quantidadeMoedas; i++)
        {
            Moeda *m = moedas + i;

            if (tentarColetar(m, posJogador, raioJogador))
            {
                pontuacao += m->valor;
            }
          
            if (m->coletada &&
                GetTime() - m->tempoColeta >= 3.0)
            {
                novaPosicaoMoeda(m);

                m->coletada = false;
                m->tempoColeta = 0.0f;
            }
        }

        BeginDrawing();

        ClearBackground(RAYWHITE);

        for (int i = 0; i < quantidadeMoedas; i++)
        {
            desenharMoeda(moedas + i);
        }

        DrawCircleV(
            posJogador,
            raioJogador,
            BLUE
        );

        DrawText(
            "W A S D - Mover",
            10,
            10,
            20,
            DARKGRAY
        );

        DrawText(
            TextFormat("Pontuacao: %d", pontuacao),
            10,
            40,
            20,
            DARKGRAY
        );

        DrawText(
            "Bronze = 5 | Prata = 10 | Ouro = 25 | Diamante = 50",
            10,
            ALTURA_JANELA - 30,
            18,
            DARKGRAY
        );

        EndDrawing();
    }

    free(moedas);

    CloseWindow();

    return 0;
}

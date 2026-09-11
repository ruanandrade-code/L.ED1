#include "raylib.h"
#include <stdlib.h>
#include <stdbool.h>

#define LARGURA_JANELA 800
#define ALTURA_JANELA 600

typedef struct
{
    float dano;
    int alcance;
} DadosArma;

typedef struct
{
    int cura;
} DadosPocao;

typedef struct
{
    int absorcao;
} DadosEscudo;

typedef union
{
    DadosArma arma;
    DadosPocao pocao;
    DadosEscudo escudo;
} DadosItem;

typedef enum
{
    ITEM_ARMA,
    ITEM_POCAO,
    ITEM_ESCUDO
} TipoItem;

typedef struct
{
    Vector2 pos;
    float raio;
    TipoItem tipo;
    DadosItem dados;
    bool coletado;
} Item;

typedef struct
{
    Vector2 pos;
    float raio;
    float vida;
    float dano;
    int armadura;
} Jogador;

Item *criarItens(int quantidade)
{
    Item *itens = (Item *)malloc(quantidade * sizeof(Item));

    if (itens == NULL)
    {
        return NULL;
    }

    for (int i = 0; i < quantidade; i++)
    {
        Item *it = itens + i;

        it->pos = (Vector2)
        {
            GetRandomValue(30, LARGURA_JANELA - 30),
            GetRandomValue(30, ALTURA_JANELA - 30)
        };

        it->raio = 12.0f;
        it->coletado = false;

        it->tipo = (TipoItem)GetRandomValue(
            ITEM_ARMA,
            ITEM_ESCUDO
        );

        if (it->tipo == ITEM_ARMA)
        {
            it->dados.arma.dano =
                (float)GetRandomValue(2, 8);

            it->dados.arma.alcance =
                GetRandomValue(1, 3);
        }
        else if (it->tipo == ITEM_POCAO)
        {
          
            int sorteioVeneno = GetRandomValue(0, 9);

            int valorCura = GetRandomValue(10, 30);

            if (sorteioVeneno < 3)
            {

                it->dados.pocao.cura = -valorCura;
            }
            else
            {
                it->dados.pocao.cura = valorCura;
            }
        }
        else if (it->tipo == ITEM_ESCUDO)
        {
            it->dados.escudo.absorcao =
                GetRandomValue(5, 15);
        }
    }

    return itens;
}

void aplicarItem(Jogador *j, Item *item)
{
    switch (item->tipo)
    {
        case ITEM_ARMA:

            j->dano += item->dados.arma.dano;

            break;


        case ITEM_POCAO:

            j->vida += item->dados.pocao.cura;
          
            if (j->vida < 0)
            {
                j->vida = 0;
            }

            break;


        case ITEM_ESCUDO:

            j->armadura += item->dados.escudo.absorcao;

            break;
    }

    item->coletado = true;
}

void desenharItem(const Item *item)
{
    if (item->coletado)
    {
        return;
    }

    Color cor;

    switch (item->tipo)
    {
        case ITEM_ARMA:
            cor = RED;
            break;

        case ITEM_POCAO:

            if (item->dados.pocao.cura >= 0)
            {
                cor = GREEN;
            }
            else
            {
              
                cor = PURPLE;
            }

            break;

        case ITEM_ESCUDO:
            cor = BLUE;
            break;

        default:
            cor = WHITE;
            break;
    }

    DrawCircleV(
        item->pos,
        item->raio,
        cor
    );

    DrawCircle(
        (int)item->pos.x - 3,
        (int)item->pos.y - 3,
        2,
        WHITE
    );
}

bool tentarColetar(
    Jogador *j,
    Item *item
)
{
    if (item->coletado)
    {
        return false;
    }

    float dx = item->pos.x - j->pos.x;
    float dy = item->pos.y - j->pos.y;

    float distancia =
        (dx * dx) + (dy * dy);

    float somaRaios =
        (item->raio + j->raio) *
        (item->raio + j->raio);

    if (distancia <= somaRaios)
    {
        aplicarItem(j, item);

        return true;
    }

    return false;
}

int main(void)
{
    InitWindow(
        LARGURA_JANELA,
        ALTURA_JANELA,
        "Atividade 3 - Union e Enum"
    );

    SetTargetFPS(60);

    int quantidadeItens = 12;

    Item *itens = criarItens(quantidadeItens);

    if (itens == NULL)
    {
        CloseWindow();
        return 1;
    }

    Jogador jogador;

    jogador.pos = (Vector2)
    {
        LARGURA_JANELA / 2.0f,
        ALTURA_JANELA / 2.0f
    };

    jogador.raio = 15.0f;
    jogador.vida = 100.0f;
    jogador.dano = 10.0f;
    jogador.armadura = 0;


    float velocidade = 4.0f;

    while (!WindowShouldClose())
    {

        if (IsKeyDown(KEY_W))
        {
            jogador.pos.y -= velocidade;
        }

        if (IsKeyDown(KEY_S))
        {
            jogador.pos.y += velocidade;
        }

        if (IsKeyDown(KEY_A))
        {
            jogador.pos.x -= velocidade;
        }

        if (IsKeyDown(KEY_D))
        {
            jogador.pos.x += velocidade;
        }

        if (jogador.pos.x < jogador.raio)
        {
            jogador.pos.x = jogador.raio;
        }

        if (jogador.pos.x >
            LARGURA_JANELA - jogador.raio)
        {
            jogador.pos.x =
                LARGURA_JANELA - jogador.raio;
        }

        if (jogador.pos.y < jogador.raio)
        {
            jogador.pos.y = jogador.raio;
        }

        if (jogador.pos.y >
            ALTURA_JANELA - jogador.raio)
        {
            jogador.pos.y =
                ALTURA_JANELA - jogador.raio;
        }

        for (int i = 0; i < quantidadeItens; i++)
        {
            tentarColetar(
                &jogador,
                itens + i
            );
        }

        BeginDrawing();

        ClearBackground(RAYWHITE);

        for (int i = 0; i < quantidadeItens; i++)
        {
            desenharItem(itens + i);
        }

        DrawCircleV(
            jogador.pos,
            jogador.raio,
            DARKBLUE
        );

        DrawText(
            "W A S D - Mover",
            10,
            10,
            20,
            DARKGRAY
        );

        DrawText(
            TextFormat(
                "Vida: %.0f",
                jogador.vida
            ),
            10,
            40,
            20,
            DARKGRAY
        );

        DrawText(
            TextFormat(
                "Dano: %.0f",
                jogador.dano
            ),
            10,
            70,
            20,
            DARKGRAY
        );

        DrawText(
            TextFormat(
                "Armadura: %d",
                jogador.armadura
            ),
            10,
            100,
            20,
            DARKGRAY
        );
        DrawText(
            "Vermelho = Arma | Verde = Pocao | Roxo = Veneno | Azul = Escudo",
            10,
            ALTURA_JANELA - 30,
            18,
            DARKGRAY
        );


        EndDrawing();
    }

    free(itens);

    CloseWindow();

    return 0;
}

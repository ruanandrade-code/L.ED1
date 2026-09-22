```c
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

typedef enum
{
    INIMIGO_VIVO,
    INIMIGO_MORTO
} EstadoInimigo;

typedef struct
{
    Vector2 pos;
    float raio;
    int vida;
    EstadoInimigo estado;
} Inimigo;

void inicializarInimigos(Inimigo *vetor, int n)
{
    Vector2 posicoes[] =
    {
        {150, 150},
        {400, 120},
        {650, 180},
        {250, 400},
        {600, 420}
    };

    for (int i = 0; i < n; i++)
    {
        Inimigo *ini = vet*
```

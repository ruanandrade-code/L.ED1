
#ifndef INVENTARIO_H
#define INVENTARIO_H

#include "entidade.h"

typedef struct inventario Inventario; // tipo opaco: struct definida só em inventario.c

Inventario *inventarioCriar(int capacidade);
void inventarioDestruir(Inventario *inv);

bool inventarioAdicionar(Inventario *inv, Entidade *e);
void inventarioRemover(Inventario *inv, int indice);

Entidade *inventarioObter(Inventario *inv, int indice);
int inventarioTotal(Inventario *inv);

// (Exercício 1): verifica se o inventário está cheio
bool inventarioEstaCheio(Inventario *inv);

// NOVO (Exercício 2): conta quantas entidades de um tipo existem
int inventarioContarPorTipo(Inventario *inv, TipoEntidade tipo);

#endif

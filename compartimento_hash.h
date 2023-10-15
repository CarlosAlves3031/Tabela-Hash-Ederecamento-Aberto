#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "clientes.h"

#define TAMANHO_HASH 7
#define REGISTRO_CLIENTE "clientes.dat"
#define METADADOS "meta.dat"

typedef struct Cliente {
    int chave;
    char nome[50];
    int estado;
    int prox;
} Cliente;

Cliente *criarCliente(int chavecliente, char *nomecliente){
    Cliente *novo = (Cliente *)malloc(sizeof(Cliente));
    if (novo) {
        memset(novo, 0, sizeof(Cliente));
        novo->chave = chavecliente;
        strcpy(novo->nome, nomecliente);
        novo->estado = 1;
        novo->prox = -1;
        printf("\n> Novo cliente criado\n");
    }
    return novo;
}

//Endereçamento Aberto Quadratico
int incremento_quadratico(int tentativa) {
    return tentativa * tentativa; // Fórmula de incremento quadrático
}

void inserir_quadratico(FILE *tabhash, FILE *meta, FILE *clientes, Cliente *info) {
    int posicao, contador, valor, tentativa = 0;
    int validade = 0;
    Cliente *checagem = (Cliente *)malloc(sizeof(Cliente));
    posicao = info->chave % TAMANHO_HASH;

    // Implementar endereçamento aberto quadrático
    while (1) {
        posicao = (posicao + incremento_quadratico(tentativa)) % TAMANHO_HASH;

        checagem = busca(tabhash, clientes, info->chave);

        if (checagem->chave == info->chave) {
            printf("A chave escolhida já é cadastrada pelo cliente %s, por favor escolha uma que não esteja em uso.\n", checagem->nome);
            free(checagem);
            free(info);
            return;
        }

        rewind(tabhash);
        if (posicao != 0) {
            fseek(tabhash, sizeof(int) * (posicao), SEEK_SET);
            fread(&posicao, sizeof(int), 1, tabhash);
        } else {
            fread(&posicao, sizeof(int), 1, tabhash);
        }

        rewind(meta);

        fread(&contador, sizeof(int), 1, meta);

        if (posicao != -1) {
            // Continuar com o restante da lógica como na função inserir original
        } else {
            // Continuar com o restante da lógica como na função inserir original
        }

        tentativa++; // Incremento quadrático
    }
}

//Hash duplo
int hash_duplo(int chave, int tentativa) {
    int hash1 = chave % TAMANHO_HASH;
    int hash2 = 1 + (chave % (TAMANHO_HASH - 1)); // Escolha um segundo hash
    return (hash1 + tentativa * hash2) % TAMANHO_HASH;
}

void inserir_duplo(FILE *tabhash, FILE *meta, FILE *clientes, Cliente *info) {
    int posicao, contador, valor, tentativa = 0;
    int validade = 0;
    Cliente *checagem = (Cliente *)malloc(sizeof(Cliente));
    posicao = info->chave % TAMANHO_HASH;

    // Implementar endereçamento aberto duplo
    while (1) {
        posicao = hash_duplo(info->chave, tentativa);

        checagem = busca(tabhash, clientes, info->chave);

        if (checagem->chave == info->chave) {
            printf("A chave escolhida já é cadastrada pelo cliente %s, por favor escolha uma que não esteja em uso.\n", checagem->nome);
            free(checagem);
            free(info);
            return;
        }

        rewind(tabhash);
        if (posicao != 0) {
            fseek(tabhash, sizeof(int) * (posicao), SEEK_SET);
            fread(&posicao, sizeof(int), 1, tabhash);
        } else {
            fread(&posicao, sizeof(int), 1, tabhash);
        }

        rewind(meta);

        fread(&contador, sizeof(int), 1, meta);

        if (posicao != -1) {
            // Continuar com o restante da lógica como na função inserir original
        } else {
            // Continuar com o restante da lógica como na função inserir original
        }

        tentativa++;
    }
}



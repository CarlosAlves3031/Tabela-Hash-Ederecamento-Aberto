#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "clientes.h"


#define TAMANHO_HASH 7
#define REGISTRO_CLIENTE "clientes.dat"
#define METADADOS "meta.dat"


Cliente criarCliente(int chavecliente, const char *nomecliente) {
    Cliente novo;
    novo.chave = chavecliente;
    strcpy(novo.nome, nomecliente);
    novo.estado = 1;
    novo.prox = -1;
    return novo;
}

// Endereçamento Aberto Quadrático
int incremento_quadratico(int tentativa) {
    return (C1 * tentativa + C2 * tentativa * tentativa) % TAMANHO_HASH;
}

// Endereçamento Aberto Linear
int incremento_linear(int tentativa, int chave) {
    int h_x = chave % TAMANHO_HASH; // h'(x) = x mod 23
    return (h_x + tentativa) % TAMANHO_HASH;
}

// Endereçamento Aberto Duplo
int hash_duplo(int chave, int tentativa) {
    int hash1 = chave % TAMANHO_HASH;
    int hash2 = 1 + (chave % (TAMANHO_HASH - 1)); // Escolha um segundo hash
    return (hash1 + tentativa * hash2) % TAMANHO_HASH;
}

// Insere um cliente na tabela hash
void inserirCliente(FILE *tabhash, const char *nomeArquivo, Cliente *cliente, int (*probe)(int, int)) {
    int posicao, tentativa = 0;
    posicao = cliente->chave % TAMANHO_HASH;

    while (1) {
        posicao = (posicao + probe(cliente->chave, tentativa)) % TAMANHO_HASH;

        // Verifica se a posição está ocupada
        fseek(tabhash, sizeof(Cliente) * posicao, SEEK_SET);
        Cliente * checagem;
        fread(&checagem, sizeof(Cliente), 1, tabhash);

        if (checagem.chave == cliente->chave) {
            printf("A chave escolhida já é cadastrada pelo cliente %s, por favor escolha uma que não esteja em uso.\n", checagem.nome);
            return;
        }

        // Se a posição estiver vazia, insere o cliente
        if (checagem.chave == -1) {
            fseek(tabhash, sizeof(Cliente) * posicao, SEEK_SET);
            fwrite(cliente, sizeof(Cliente), 1, tabhash);
            fflush(tabhash);
            return;
        }

        tentativa++;
    }
}

// Função para buscar um cliente na tabela hash
Cliente buscarCliente(FILE *tabhash, int chave, int (*probe)(int, int)) {
    int posicao, tentativa = 0;
    posicao = chave % TAMANHO_HASH;

    while (1) {
        posicao = (posicao + probe(chave, tentativa)) % TAMANHO_HASH;

        fseek(tabhash, sizeof(Cliente) * posicao, SEEK_SET);
        Cliente checagem;
        fread(&checagem, sizeof(Cliente), 1, tabhash);

        if (checagem.chave == chave) {
            return checagem; // Retorna o cliente encontrado
        }

        if (checagem.chave == -1 || tentativa >= TAMANHO_HASH) {
            // Cliente não encontrado ou percorreu toda a tabela
            Cliente naoEncontrado;
            naoEncontrado.chave = -1;
            return naoEncontrado;
        }

        tentativa++;
    }
}

// Insere um cliente na tabela hash usando endereçamento aberto linear
void inserir_linear(FILE *tabhash, const char *nomeArquivo, Cliente *cliente) {
    int posicao, tentativa = 0;
    posicao = cliente->chave % TAMANHO_HASH;

    while (1) {
        posicao = (posicao + incremento_linear(tentativa)) % TAMANHO_HASH;

        fseek(tabhash, sizeof(Cliente) * posicao, SEEK_SET);
        Cliente checagem;
        fread(&checagem, sizeof(Cliente), 1, tabhash);

        if (checagem.chave == cliente->chave) {
            printf("A chave escolhida já é cadastrada pelo cliente %s, por favor escolha uma que não esteja em uso.\n", checagem.nome);
            return;
        }

        if (checagem.chave == -1) {
            fseek(tabhash, sizeof(Cliente) * posicao, SEEK_SET);
            fwrite(cliente, sizeof(Cliente), 1, tabhash);
            fflush(tabhash);
            return;
        }

        tentativa++;
    }
}

// Insere um cliente na tabela hash usando endereçamento aberto duplo
void inserir_duplo(FILE *tabhash, const char *nomeArquivo, Cliente *cliente) {
    int posicao, tentativa = 0;
    posicao = cliente->chave % TAMANHO_HASH;

    while (1) {
        posicao = hash_duplo(cliente->chave, tentativa);

        fseek(tabhash, sizeof(Cliente) * posicao, SEEK_SET);
        Cliente checagem;
        fread(&checagem, sizeof(Cliente), 1, tabhash);

        if (checagem.chave == cliente->chave) {
            printf("A chave escolhida já é cadastrada pelo cliente %s, por favor escolha uma que não esteja em uso.\n", checagem.nome);
            return;
        }

        if (checagem.chave == -1) {
            fseek(tabhash, sizeof(Cliente) * posicao, SEEK_SET);
            fwrite(cliente, sizeof(Cliente), 1, tabhash);
            fflush(tabhash);
            return;
        }

        tentativa++;
    }
}

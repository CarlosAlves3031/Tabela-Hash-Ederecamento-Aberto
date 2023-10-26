#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "clientes.h"


#define TAMANHO_HASH 8
#define REGISTRO_CLIENTE "clientes.dat"
#define METADADOS "meta.dat"


Cliente *criarCliente(int chavecliente, const char *nomecliente) {
    Cliente *novo = (Cliente *)malloc(sizeof(Cliente));
    novo->chave = chavecliente;
    strcpy(novo->nome, nomecliente);
    novo->estado = 1;
    novo->prox = -1;
    return novo;
}

// Endereçamento Aberto Quadrático
int incremento_quadratico(int tentativa, int chave) {
    int base;
    if(tentativa == 0){
        return chave % TAMANHO_HASH;
    }
    else{
        base = incremento_quadratico(tentativa - 1, chave);
        return base % TAMANHO_HASH;
    }
    //return (C1 * tentativa + C2 * tentativa * tentativa) % TAMANHO_HASH;
}

// Endereçamento Aberto Linear
int incremento_linear(int tentativa, int chave) {
    int h_x = chave % TAMANHO_HASH; // h'(x) = x mod 23
    return (h_x + tentativa) % TAMANHO_HASH;
}

// Endereçamento Aberto Duplo
int hash_duplo( int tentativa, int chave) {
    int hash1 = chave % TAMANHO_HASH;
    int hash2 = 1 + (chave % (TAMANHO_HASH - 1)); // Escolha um segundo hash
    return (hash1 + tentativa * hash2) % TAMANHO_HASH;
}

// Insere um cliente na tabela hash
void inserirCliente(FILE *clientes, FILE *meta, Cliente *info, int (*probe)(int, int), int modelo) {
    Cliente * checagem;
    int posicao, tentativa, validade = 0;

    tentativa = 0;
    posicao = info->chave % TAMANHO_HASH;

    checagem = buscarCliente(clientes, info->chave, modelo);
    if (checagem->chave == info->chave) {
            printf("A chave escolhida já é cadastrada pelo cliente %s, por favor escolha uma que não esteja em uso.\n", checagem.nome);
            return;
        }

    while (validade == 0) {
        if(modelo == 1){
            posicao = incremento_linear(tentativa, info->chave);
        }
        else if(modelo == 2){
            posicao = incremento_quadratico(tentativa, info->chave);
        }
        else{ 
            posicao = hash_duplo(tentativa, info->chave);
        }
       
        // Verifica se a posição está ocupado

        rewind(clientes);
        fseek(clientes, sizeof(Cliente) * posicao, SEEK_SET);
     
        fread(&checagem->chave, sizeof(int), 1, clientes);
        fread(checagem->nome, sizeof(char), sizeof(checagem->nome), clientes);
        fread(&checagem->estado, sizeof(int), 1, clientes);
        // Se a posição estiver vazia, insere o cliente
        if (checagem->estado == 0){
            validade = 1;
            }
            /*fseek(tabhash, sizeof(Cliente) * posicao, SEEK_SET);
            fwrite(cliente, sizeof(Cliente), 1, tabhash);
            fflush(tabhash);
            return;*/
        }
    else if(tentativa >= TAMANHO_HASH){
            validade = 3;
    }    
        tentativa++;
    }
    if(validade != 3){
        rewind(clientes);
        fseek(clientes, sizeof(Cliente) * posicao, SEEK_SET);
        fwrite(&info->chave, sizeof(int), 1, clientes);
        fwrite(info->nome, sizeof(char), sizeof(info->nome), clientes);
        fwrite(&info->estado, sizeof(int), 1, clientes);
    else{
        printf("Não há espaço no compartimento");
    }
    }
    
}

// Função para buscar um cliente na tabela hash
Cliente *buscarCliente(FILE *clientes, int chave, int modelo) {
    Cliente *checagem = (Cliente *)malloc(sizeof(Cliente));
    int validade = 0, posicao, tentativa = 0;
    

    while (validade == 0) {
        rewind(clientes);
        if(modelo == 1){
            posicao = incremento_linear(tentativa, chave);
        }
        else if(modelo == 2){
            posicao = incremento_quadratico(tentativa, chave);
        }
        else{ 
            posicao = hash_duplo(tentativa, chave);
        }
       
        
        //posicao = (posicao + probe(chave, tentativa)) % TAMANHO_HASH;

        fseek(clientes, sizeof(Cliente) * posicao, SEEK_SET);

        //fread(checagem, sizeof(Cliente), 1, clientes);
        fread(&checagem->chave, sizeof(int), 1, clientes);
        fread(checagem->nome, sizeof(char), sizeof(checagem->nome), clientes);
        fread(&checagem->estado, sizeof(int), 1, clientes);
        fread(&checagem->prox, sizeof(int), 1, clientes);
        if (checagem->chave == chave) {
            return checagem; // Retorna o cliente encontrado
        }

        if (checagem->prox == -1 || tentativa >= TAMANHO_HASH) {
            checagem->chave = -1;
            return checagem;
        }

        tentativa++;
    }
}

// Insere um cliente na tabela hash usando endereçamento aberto linear
/*void inserir_linear(FILE *tabhash, const char *nomeArquivo, Cliente *cliente) {
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
*/

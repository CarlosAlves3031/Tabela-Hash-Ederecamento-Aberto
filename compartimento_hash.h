#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "clientes.h"


#define TAMANHO_HASH 8
#define REGISTRO_CLIENTE "clientes.dat"
#define METADADOS "meta.dat"


Cliente *criarCliente(int chavecliente, char *nomecliente) {
    Cliente *novo = (Cliente *)malloc(sizeof(Cliente));
    novo->chave = chavecliente;
    strcpy(novo->nome, nomecliente);
    novo->estado = 1;
    return novo;
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
        if (checagem->chave == chave) {
            return checagem; // Retorna o cliente encontrado
        }

        if (tentativa >= TAMANHO_HASH) {
            checagem->chave = -1;
            return checagem;
        }

        tentativa++;
    }
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
void inserirCliente(FILE *clientes, Cliente *info, int modelo) {
    Cliente * checagem;
    int posicao, tentativa, validade = 0;
    

    tentativa = 0;

    checagem = buscarCliente(clientes, info->chave, modelo);
    if (checagem->chave == info->chave) {
            printf("A chave escolhida já é cadastrada pelo cliente %s, por favor escolha uma que não esteja em uso.\n", checagem->nome);
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
    }
    else{
        printf("Não há espaço no compartimento\n");
    }

    printf("quantidade de colisoes %d \n", tentativa)
}

void deletar(FILE *clientes, int chave, int modelo){
    int validade = 0, proximo, posicao;
    int tentativa = 0;
    Cliente *atual = (Cliente *)malloc(sizeof(Cliente));


    //printf("%d", posicao);
    printf("\n> Procurando cliente...\n");
        while (validade == 0) {
            if(modelo == 1){
            posicao = incremento_linear(tentativa, chave);
            }
            else if(modelo == 2){
                posicao = incremento_quadratico(tentativa, chave);
            }
            else{ 
                posicao = hash_duplo(tentativa, chave);
            }
            rewind(clientes);
            fseek(clientes, sizeof(Cliente) * posicao, SEEK_SET);
            fread(&atual->chave, sizeof(int), 1, clientes);
            fread(atual->nome, sizeof(char), sizeof(atual->nome), clientes);
            fread(&atual->estado, sizeof(int), 1, clientes);
            //printf("\nteste");
            if (atual->chave == chave) {
                validade = 1;
                atual->chave = -2;
                strcpy(atual->nome, "----");
                atual->estado = 0;

                rewind(clientes);
                fseek(clientes, sizeof(Cliente) * posicao, SEEK_SET);

                fwrite(&atual->chave, sizeof(int), 1, clientes);
                fwrite(atual->nome, sizeof(char), sizeof(atual->nome), clientes);
                fwrite(&atual->estado, sizeof(int), 1, clientes);

                rewind(clientes);
                fseek(clientes, sizeof(Cliente) * posicao, SEEK_SET);

                fread(&atual->chave, sizeof(int), 1, clientes);
                fread(atual->nome, sizeof(char), sizeof(atual->nome), clientes);
                fread(&atual->estado, sizeof(int), 1, clientes);

                printf("Estado do cliente deletado: %d", atual->estado);

                printf("\nCliente deletado com sucesso!\n");
                return;
            } else if (tentativa >= TAMANHO_HASH) {
                validade = -1;
                printf("Cliente não encontrado, por favor insira ele na Hash primeiro.\n");
                free(atual);
                break;
            } else {
                tentativa++;
            }
        }
    free(atual);
}

void mostrarRegistros(FILE *clientes, FILE*meta){
    Cliente *reg = (Cliente *)malloc(sizeof(Cliente));
    int contador, i = 0;

    rewind(meta);
    fread(&contador, sizeof(int), 1, meta);
    if(contador == 0){
        printf("Não existe nenhum registro de clientes nesse arquivo");
        free(reg);
        return;
    }
    printf("\nREGISTROS\n");
    rewind(clientes);
    
    for(i = 0; i < TAMANHO_HASH; i++){ 
        rewind(clientes);
        fseek(clientes, sizeof(Cliente) * i, SEEK_SET);
        fread(&reg->chave, sizeof(int), 1, clientes);
        if(reg->chave < 0){   
            continue;
        }
        fread(reg->nome, sizeof(char), sizeof(reg->nome), clientes);
        fread(&reg->estado, sizeof(int), 1, clientes);
        printf("--------------------------------------------------\n");
        printf("Posição: %d\n", i);
        printf("Cliente: %s\n", reg->nome);
        printf("Chave do cliente: %d\n", reg->chave);
    }
    free(reg);
}

void zerar(FILE *meta, FILE *clientes){
    int contador = TAMANHO_HASH;
    int novo;
    int a = -1;
    Cliente *atual = (Cliente *)malloc(sizeof(Cliente));
    int b, estado, prox;

    atual->chave = -1;
    strcpy(atual->nome, "-----");
    atual->estado = 0;
    
    fclose(meta);


    if ((meta = fopen(METADADOS, "wb")) == NULL) {
        printf("Erro ao abrir o arquivo da tabela meta\n");
        exit(1);
    }

    for (int i = 0; i < TAMANHO_HASH; i++) {
        rewind(clientes);
        fseek(clientes, sizeof(Cliente) * i, SEEK_SET);
        fwrite(&atual->chave, sizeof(int), 1, clientes);
        fwrite(atual->nome, sizeof(Cliente), sizeof(atual->nome), clientes);
        fwrite(&atual->estado, sizeof(int), 1, clientes);
    }

    rewind(meta);
    fwrite(&contador, sizeof(int), 1, meta);

    fclose(meta);

    if ((meta = fopen(METADADOS, "r+b")) == NULL) {
        printf("Erro ao abrir o arquivo da tabela meta\n");
        exit(1);
    }

    rewind(meta);

    fread(&novo, sizeof(int), 1 ,meta);

    printf("\n\n> Contador: %d\n", novo);
    printf("> Tabela Clientes zerada\n");
    printf("Arquivos zerados com sucesso!\n\n");

    /*for (int i = 0; i < TAMANHO_HASH; i++) {
        rewind(clientes);
        fseek(clientes, sizeof(Cliente) * i, SEEK_SET);
        fread(&b, sizeof(int), 1, clientes);
        printf("Local da hash %d, recebe %d \n", i, b);
    }*/
    rewind(clientes);
    free(atual);

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

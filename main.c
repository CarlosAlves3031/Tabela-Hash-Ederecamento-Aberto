#include <stdio.h>
#include <stdlib.h>
#include "clientes.h"
#include "compartimento_hash.h"
#include <time.h>


void menu(FILE *meta, FILE *clientes, int modo){
    int escolha, chave;
    char nome[100];
    Cliente *novo;

    while(1){
        printf("\n-------------Menu-------------\n 1 - Inserção \n 2 - Deletar \n 3 - Busca \n 4 - Zerar  \n 5 - Mostrar registros  \n 6 - Sair \n -----------------------------");
        printf("\nDigite uma das opcoes acima:");
        scanf("%d", &escolha);
        switch (escolha)
        {
        case (1) :
            printf("Digite o número da chave do cliente que você deseja inserir: \n");
            scanf("%d", &chave);
            printf("Digite o nome do cliente que você deseja inserir:\n");
            scanf("%s", nome);
            novo = criarCliente(chave, nome);

            double time_spent1 = 0.0;

            clock_t begin1 = clock();
            inserirCliente(clientes, novo, modo);
            clock_t end1 = clock();
            time_spent1 += (double)(end1 - begin1) / CLOCKS_PER_SEC;

            printf("> Tempo de inserção: %f ms\n", time_spent1 * 1000);

            free(novo);
            //printf("fechando arquivos e ponteiros");
            break;
        case (2):
            printf("Digite a chave do cliente que você quer deletar:\n");
            scanf("%d", &chave);

            double time_spent2 = 0.0;

            clock_t begin2 = clock();
            deletar(clientes, chave, modo);
            clock_t end2 = clock();

            time_spent2 += (double)(end2 - begin2) / CLOCKS_PER_SEC;
            printf("> Tempo de deleção: %f ms\n", time_spent2 * 1000);

            free(novo);
            break;
        case (3):
            printf("Digite a chave do cliente que você quer buscar:\n");
            scanf("%d", &chave);

            double time_spent3 = 0.0;

            clock_t begin3 = clock();
            novo = buscarCliente(clientes, chave, modo);
            clock_t end3 = clock();

            time_spent3 += (double)(end3 - begin3) / CLOCKS_PER_SEC; 


            if(novo->chave == -1){
                printf("Cliente não encontrado,por favor insira ele na Hash primeiro");
            }
            else{
                printf("> Chave do cliente: %d \n", novo->chave);
                printf("> Nome do cliente: %s \n", novo->nome);
                printf("> Tempo de busca: %f ms\n", time_spent3 * 1000);
            }
            break;
        case (4):
            zerar(meta, clientes);
            break;
        case(5):
            mostrarRegistros(clientes, meta);
            break;
        case (6):
            fclose(meta);
            fclose(clientes);
            exit(1);
            break;
        default:
            printf("Escolha um dos números do menu\n");
            break;
        }
    }
    
}

int main(){
    FILE *meta;
    FILE *clientes;
    int x;

    if ((meta = fopen("meta.dat", "r+b")) == NULL){
    printf("Erro ao abrir o arquivo da tabela Meta");
    exit(1);
    }

    if ((clientes = fopen("clientes.dat", "r+b")) == NULL){
    printf("Erro ao abrir o arquivo da tabela Clientes");
    exit(1);
    }

    printf("Qual tipo de tratamento de dispersão você deseja:\n 1 - Tentativa linear \n 2 - Tentativa quadrática \n 3 - Dispersão dupla\n");
    scanf("%d", &x);

    if(x<1 || x>3){
        printf("Opção invalida");
    }else{
        menu(meta, clientes, x);
    }

    return 0;
}

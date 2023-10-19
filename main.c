#include <stdio.h>
#include <stdlib.h>
#include "clientes.h"
#include "compartimento_hash.h"

int main() {
    // Abre os arquivos
    FILE *tabhash = abrirArquivo(REGISTRO_CLIENTE);
    FILE *meta = abrirArquivo(METADADOS);

    if (tabhash == NULL || meta == NULL) {
        printf("Erro ao abrir os arquivos.\n");
        return 1;
    }

    Cliente novoCliente = criarCliente(1, "Cliente1"); // Substitua pelos dados reais

    inserirCliente(tabhash, REGISTRO_CLIENTE, &novoCliente, incremento_quadratico);

    // Exemplo de busca
    int chaveBusca = 1;
    Cliente clienteEncontrado = buscarCliente(tabhash, chaveBusca, incremento_quadratico);
    if (clienteEncontrado.chave != -1) {
        printf("Cliente encontrado: %s\n", clienteEncontrado.nome);
    } else {
        printf("Cliente não encontrado.\n");
    }

    // Fecha os arquivos
    fecharArquivo(tabhash);
    fecharArquivo(meta);

    return 0;
}
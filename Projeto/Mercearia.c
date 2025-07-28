#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX 300                  // quantidade maxima que o estoque suporta
#define TAM_DATA 11              // tamanho da data dd/mm/aaaa +\0
#define TAM_NOME 50              // quantidade maxima de caracteres que o produto pode ter é 49 +\0
#define RELATORIO "Estoque.txt"  // nome do arquivo de texto do relatório

typedef struct {
    char nome[TAM_NOME];
    int quantidade;
    float preco;
    char data_validade[TAM_DATA];
} Produto;

// Funções:
int adicionar_produto(Produto estoque, int num_produtos, Produto novo_produto);
void verificar_validade(const Produto estoque, int num_produtos, const char data_hoje);

int atualizar_estoque int buscar_produto_por_nome void gerar_relatorio

    // Menu do usuario
    int
    main()
{
    printf("----Sistema de controle de estoque----\n");
    printf("1.Cadastrar novo produto\n");
    printf("2.Verificar os produtos em estoque\n");
    printf("3.Verificar validade dos produtos\n");
    printf("4.Atualizar estoque\n");
    printf("5.Gerar relatorio\n");
    printf("6.Buscar produto pelo nome\n");
    printf("7.Sair");

    return 0;
}

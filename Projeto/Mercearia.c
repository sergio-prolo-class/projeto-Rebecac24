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
//  Adiciona um novo produto ao array de estoque.
int adicionar_produto(Produto estoque[], int *num_produtos, Produto novo_produto);

// Percorre o estoque e imprime na tela os produtos vencidos.
void verificar_validade(const Produto estoque[], int num_produtos, const char *data_hoje);

// Atualiza a quantidade ou o preço de um produto específico.
int atualizar_estoque(Produto *produto_alvo, int operacao, float valor);

// Busca um produto no estoque pelo nome.
int buscar_produto_por_nome(const Produto estoque[], int num_produtos, const char *nome_busca);

// Gera um arquivo de texto (.txt) com um relatório de todos os produtos.
void gerar_relatorio_texto(const Produto estoque[], int num_produtos);

// Menu do usuario
int main()
{
    int quant_produtos = 0;
    int opcao;

    do {
        printf("----Sistema de controle de estoque----\n");
        printf("1.Cadastrar novo produto\n");
        printf("2.Verificar os produtos em estoque\n");
        printf("3.Verificar validade dos produtos\n");
        printf("4.Atualizar estoque\n");
        printf("5.Gerar relatorio\n");
        printf("6.Buscar produto pelo nome\n");
        printf("7.Aplicar desconto em % em produtos\n");
        printf("0.Sair do menu\n");
        scanf("d", &opcao);

        while (scanf() != '\n')
            switch (opcao) {
                case 1: {
                    Produto p;  // é uma estrutura temporaria para o novo produto
                    printf("Digite o nome do produto: ");
                    scanf("%49 %s\n", p.nome);  // para acessar campos de uma estrutura usa o operador ponto: nome.campo

                    printf("Digite o preço do produto:");

                    printf("Digite a data de validade do produto (dd/mm/aaaa)");

                    if (adicionar_produto)
                        printf("Produto cadastrado no sistema.");
                    else
                        printf("Erro não foi possivel cadastrar o produto estoque pode estar cheio ou dados invalidos");
                }

                case 2: {
                    printf("Lista dos produtos em estoque:");
                }

                case 3: {
                    printf("Digite a data atual em dd/mm/aaaa");
                }

                case 4 {
                    printf("Digite o nome do produto para atualizar");

                }

                case 5: {  // gerar relatório em arquivo de texto
                    printf(" ");
                }

                case 6: {
                    printf(" ");
                }

                case 7: {
                    printf(" ");
                }

                case 0: {
                    printf("Saindo do estoque.")
                }
            }

        return 0;
    }

    int adicionar_produto if (num_produtos >= MAX)
    {  // verifica se o estoque está cheio
        return
    }

    void verificar_validade(const Produto estoque[], int num_produtos, const char *data_hoje)
    {
        int vencidos = 0;

        int atualizar_estoque(Produto * produto_para_atualizar, int atualizacao, float valor)
        {
            switch (atualizacao)

            // para acessar campos de ums estrutura usa o operador ponto: nome.campo
            // strcmp da biblioteca string.h  determina se duas sequências de caracteres são idênticas ou qual delas vem primeiro em ordem alfabética.
            // getchar função usada para limpeza do buffer de entrada está na biblioteca stdio.h
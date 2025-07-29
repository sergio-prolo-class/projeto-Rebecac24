#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "estoque.h"

// Menu do usuario
int main()
{
    Produto estoque[MAX];
    int num_produtos = 0;  // O estoque começa vazio
    int opcao;

    do {
        printf("----Sistema de controle de estoque----\n");
        printf("1.Cadastrar novo produto\n");
        printf("2.Verificar os produtos em estoque\n");
        printf("3.Verificar validade dos produtos\n");
        printf("4.Atualizar estoque\n");
        printf("5.Gerar relatorio\n");
        printf("6.Buscar produto pelo nome\n");
        printf("7.Aplicar desconto em porcentagem em produto \n");
        printf("0.Sair do menu\n");
        scanf("%d", &opcao);

        // Limpa o buffer
        while (getchar() != '\n')
            ;  // getchar função usada para limpeza do buffer de entrada está na biblioteca stdio.h

        switch (opcao) {
            case 1: {       // Cadastra um novo produto
                Produto p;  // é uma estrutura temporaria para o novo produto

                printf("Digite o nome do produto: ");
                scanf("%50[^\n]", p.nome);  // usando para limitar quantos caracteres serão lidos pelo scanf, le 50 caracteres até um /n //para acessar campos de uma estrutura usa o operador ponto: nome.campo
                while (getchar() != '\n')
                    ;

                printf("Digite a quantidade: ");
                scanf("%d", &p.quantidade);
                while (getchar() != '\n')
                    ;

                printf("Digite o preco: ");
                scanf("%f", &p.preco);
                while (getchar() != '\n')
                    ;

                printf("Digite a data de validade em dd/mm/aaaa): ");
                scanf("%10s", p.data_validade);
                while (getchar() != '\n')
                    ;

                if (adicionar_produto(estoque, &num_produtos, p)) {  // chama função para adicionar o produto e main mostra o resultado
                    printf("\n Produto '%s' cadastrado com sucesso\n", p.nome);
                } else {
                    printf("\n Erro, nao foi possivel cadastrar o produto. Estoque pode estar cheio ou dados invalidos.\n");
                }
                break;
            }
            case 2: {  // Listar os produtos
                printf("\nLista dos Produtos em Estoque\n");
                if (num_produtos == 0) {
                    printf("Nenhum produto cadastrado.\n");
                } else {
                    for (int i = 0; i < num_produtos; i++) {
                        printf("Nome: %s | Qtd: %d | Preco: R$%.2f | Validade: %s\n",
                               estoque[i].nome, estoque[i].quantidade,
                               estoque[i].preco, estoque[i].data_validade);
                    }
                }
                printf("\n");
                break;
            }
            case 3: {  // Verificar validade
                char data_atual[TAM_DATA];
                printf("Digite a data de hoje em dd/mm/aaaa: ");
                scanf("%10s", data_atual);
                while (getchar() != '\n')
                    ;

                printf("\nVerificando Validade dos Produtos baseado na data: %s \n", data_atual);

                int dia_hoje, mes_hoje, ano_hoje;
                if (extrair_data_componentes(data_atual, &dia_hoje, &mes_hoje, &ano_hoje) == 0) {  // Código a ser executado se a extração da data falhar
                    printf("Erro: Formato de data de hoje invalido. Use DD/MM/AAAA.\n");
                    break;  // Sai do case
                }

                if (num_produtos == 0) {
                    printf("Nenhum produto cadastrado.\n");
                } else {
                    int algum_vencido = 0;
                    for (int i = 0; i < num_produtos; i++) {
                        int dia_prod, mes_prod, ano_prod;
                        if (extrair_data_componentes(estoque[i].data_validade, &dia_prod, &mes_prod, &ano_prod)) {
                            printf("ALERTA: Produto '%s' tem formato de validade invalido: %s. Ignorando validacao.\n",
                                   estoque[i].nome, estoque[i].data_validade);
                            continue;
                        }

                        if (ano_prod < ano_hoje ||
                            (ano_prod == ano_hoje && mes_prod < mes_hoje) ||
                            (ano_prod == ano_hoje && mes_prod == mes_hoje && dia_prod < dia_hoje)) {
                            printf("ALERTA: Produto '%s' esta VENCIDO! (Validade: %s)\n",
                                   estoque[i].nome, estoque[i].data_validade);
                            algum_vencido = 1;
                        }
                    }
                    if (!algum_vencido) {
                        printf("Nenhum produto vencido encontrado.\n");
                    }
                }
                printf("\n");
                break;
            }
            case 4: {  // Atualizar estoque: adicionar,remover,alterar preco
                char nome_busca[TAM_NOME];
                printf("Digite o nome do produto que deseja atualizar: ");
                scanf("%49[^\n]", nome_busca);
                while (getchar() != '\n')
                    ;

                int indice = buscar_produto_por_nome(estoque, num_produtos, nome_busca);

                if (indice != -1) {  // Se o produto foi encontrado
                    int sub_opcao;
                    float valor;

                    printf("Produto encontrado: %s\n", estoque[indice].nome);
                    printf("O que deseja fazer?\n");
                    printf("  1. Adicionar quantidade\n");
                    printf("  2. Remover quantidade\n");
                    printf("  3. Alterar preco\n");
                    printf("Escolha uma opcao: ");
                    scanf("%d", &sub_opcao);
                    while (getchar() != '\n')
                        ;

                    printf("Digite o valor (quantidade ou novo preco): ");
                    scanf("%f", &valor);
                    while (getchar() != '\n')
                        ;

                    if (atualizar_estoque(&estoque[indice], sub_opcao, valor)) {
                        printf("\nEstoque foi atualizado com sucesso!\n\n");
                    } else {
                        printf("\nErro, operacao invalida ou valor incorreto (ex: remover mais do que tem).\n\n");
                    }
                } else {
                    printf("\n>>> Produto '%s' nao encontrado no estoque.\n\n", nome_busca);
                }
                break;
            }
            case 5: {  // Imprime o relatorio no terminal
                printf("\nRelatorio de Estoque da Mercearia\n");
                printf("Total de produtos: %d\n", num_produtos);

                if (num_produtos == 0) {
                    printf("Nenhum produto cadastrado para o relatorio.\n");
                } else {
                    for (int i = 0; i < num_produtos; i++) {
                        printf("Produto: %s\n", estoque[i].nome);
                        printf("  Quantidade: %d unidades\n", estoque[i].quantidade);
                        printf("  Preco Unitario: R$%.2f\n", estoque[i].preco);
                        printf("  Data de Validade: %s\n\n", estoque[i].data_validade);
                    }
                }
                printf("Fim do Relatorio\n");
                printf("\nRelatorio impresso no terminal.\n\n");
                break;
            }
            case 6: {  // Buscar produto por nome
                char nome_busca[TAM_NOME];
                printf("Digite o nome do produto para buscar: ");
                scanf("%49[^\n]", nome_busca);
                while (getchar() != '\n')
                    ;

                int indice = buscar_produto_por_nome(estoque, num_produtos, nome_busca);

                if (indice != -1) {
                    printf("\n--- Produto Encontrado ---\n");
                    printf("Nome: %s\n", estoque[indice].nome);
                    printf("Quantidade: %d\n", estoque[indice].quantidade);
                    printf("Preco: R$%.2f\n", estoque[indice].preco);
                    printf("Validade: %s\n\n", estoque[indice].data_validade);
                } else {
                    printf("\nProduto '%s' nao encontrado no estoque.\n\n", nome_busca);
                }
                break;
            }
            case 7: {  // Aplicar desconto
                char nome_busca[TAM_NOME];
                float porcentagem;

                printf("Digite o nome do produto para aplicar o desconto: ");
                scanf("%49[^\n]", nome_busca);
                while (getchar() != '\n')
                    ;

                int indice = buscar_produto_por_nome(estoque, num_produtos, nome_busca);

                if (indice != -1) {
                    printf("Produto encontrado: %s (Preco atual: R$%.2f)\n", estoque[indice].nome, estoque[indice].preco);
                    printf("Digite a porcentagem de desconto (ex: 10 para 10%%): ");
                    scanf("%f", &porcentagem);
                    while (getchar() != '\n')
                        ;

                    if (aplicar_desconto(&estoque[indice], porcentagem)) {
                        printf("\n>>> Desconto de %.2f%% aplicado com sucesso! Novo preco: R$%.2f\n\n",
                               porcentagem, estoque[indice].preco);
                    } else {
                        printf("\n Erro, porcentagem de desconto invalida (deve ser entre 0 e 100).\n\n");
                    }
                } else {
                    printf("\n Produto '%s' nao encontrado no estoque.\n\n", nome_busca);
                }
                break;
            }
            case 0: {  // Sair
                printf("\nSaindo do programa.\n");
                break;
            }
            default: {
                printf("\nOpcao invalida. Tente novamente.\n\n");
                break;
            }
        }

    } while (opcao != 0);

    return 0;
}

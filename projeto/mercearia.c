#include <stdio.h>
#include <stdlib.h>
#include "estoque.h"
#include "util.h"

int main()
{
    carregar_estoque();

    int opcao;
    do {
        printf("\nSistema de Controle de Estoque:\n");
        printf("1. Cadastrar novo produto\n");
        printf("2. Verificar produtos em estoque\n");
        printf("3. Verificar validade dos produtos\n");
        printf("4. Atualizar estoque\n");
        printf("5. Salvar atualizações\n");
        printf("6. Gerar relatório completo\n");
        printf("7. Buscar produtos\n");
        printf("0. Sair\n");
        printf("Escolha uma opção: ");

        if (scanf("%d", &opcao) != 1) {
            limpar_buffer();
            printf("Entrada inválida! Digite um número.\n");
            continue;
        }
        limpar_buffer();

        switch (opcao) {
            case 1: {
                int codigo, quantidade;
                char nome[50], data_validade[DATA_TAMANHO];
                float preco;

                printf("Código: ");
                if (scanf("%d", &codigo) != 1) {
                    printf("Código inválido!\n");
                    limpar_buffer();
                    break;
                }
                limpar_buffer();

                printf("Nome: ");
                if (scanf(" %49[^\n]", nome) != 1) {
                    printf("Nome inválido!\n");
                    limpar_buffer();
                    break;
                }
                limpar_buffer();

                printf("Quantidade: ");
                if (scanf("%d", &quantidade) != 1) {
                    printf("Quantidade inválida!\n");
                    limpar_buffer();
                    break;
                }
                limpar_buffer();

                printf("Preço: ");
                if (scanf("%f", &preco) != 1) {
                    printf("Preço inválido!\n");
                    limpar_buffer();
                    break;
                }
                limpar_buffer();

                do {
                    printf("Data de validade (DD/MM/AAAA): ");
                    if (scanf("%10s", data_validade) != 1) {
                        printf("Data inválida!\n");
                        limpar_buffer();
                        continue;
                    }
                    limpar_buffer();

                    if (!validar_data(data_validade)) {
                        printf("Formato inválido! Use DD/MM/AAAA.\n");
                    }
                } while (!validar_data(data_validade));

                int resultado = adicionar_produto(codigo, nome, quantidade, preco, data_validade);
                switch (resultado) {
                    case -1: printf("Erro: Estoque cheio!\n"); break;
                    case -2: printf("Erro: Código já existe!\n"); break;
                    case -3: printf("Erro: Quantidade inválida!\n"); break;
                    case -4: printf("Erro: Preço inválido!\n"); break;
                    case 0: printf("Produto adicionado com sucesso!\n"); break;
                    default: printf("Erro desconhecido!\n");
                }
                break;
            }

            case 2: {
                printf("\n--- Produtos em Estoque ---\n");
                if (total_produtos == 0) {
                    printf("Nenhum produto cadastrado.\n");
                } else {
                    for (int i = 0; i < total_produtos; i++) {
                        printf("[%d] %s - %d unidades - R$%.2f - Val: %s\n",
                               estoque[i].codigo, estoque[i].nome,
                               estoque[i].quantidade, estoque[i].preco,
                               estoque[i].data_validade);
                    }
                }
                break;
            }

            case 3: {
                char *resultados[MAX_PRODUTOS];
                int total;

                verificar_validade(resultados, &total);
                printf("\n--- Produtos Vencidos ---\n");
                if (total == 0) {
                    printf("Nenhum produto vencido.\n");
                } else {
                    for (int i = 0; i < total; i++) {
                        printf("%s\n", resultados[i]);
                        free(resultados[i]);
                    }
                    printf("Total de produtos vencidos: %d\n", total);
                }
                break;
            }

            case 4: {
                int sub_opcao, codigo, quantidade;
                float novo_preco;

                printf("\n--- Atualização de Estoque ---\n");
                printf("1. Adicionar quantidade\n");
                printf("2. Remover quantidade\n");
                printf("3. Alterar preço\n");
                printf("Escolha uma opção: ");

                if (scanf("%d", &sub_opcao) != 1) {
                    printf("Opção inválida!\n");
                    limpar_buffer();
                    break;
                }
                limpar_buffer();

                printf("Digite o código do produto: ");
                if (scanf("%d", &codigo) != 1) {
                    printf("Código inválido!\n");
                    limpar_buffer();
                    break;
                }
                limpar_buffer();

                if (sub_opcao == 1 || sub_opcao == 2) {
                    printf("Quantidade: ");
                    if (scanf("%d", &quantidade) != 1) {
                        printf("Quantidade inválida!\n");
                        limpar_buffer();
                        break;
                    }
                    limpar_buffer();
                    novo_preco = 0;
                } else if (sub_opcao == 3) {
                    printf("Novo preço: ");
                    if (scanf("%f", &novo_preco) != 1) {
                        printf("Preço inválido!\n");
                        limpar_buffer();
                        break;
                    }
                    limpar_buffer();
                    quantidade = 0;
                } else {
                    printf("Opção inválida!\n");
                    break;
                }

                int resultado = atualizar_estoque(sub_opcao, codigo, quantidade, novo_preco);
                switch (resultado) {
                    case -1: printf("Produto não encontrado!\n"); break;
                    case -2: printf("Quantidade inválida!\n"); break;
                    case -3: printf("Quantidade maior que estoque disponível!\n"); break;
                    case -4: printf("Preço inválido!\n"); break;
                    case -5: printf("Opção inválida!\n"); break;
                    default:
                        if (sub_opcao == 1 || sub_opcao == 2) {
                            printf("Quantidade atualizada. Novo total: %d\n", resultado);
                        } else {
                            printf("Preço atualizado com sucesso!\n");
                        }
                }
                break;
            }

            case 5: {
                int resultado = salvar_estoque();
                if (resultado == -1) {
                    printf("Erro ao salvar o estoque!\n");
                } else {
                    printf("Estoque salvo com sucesso no arquivo 'estoque.txt'!\n");
                }
                break;
            }

            case 6: {
                char relatorio[5000];
                gerar_relatorio(relatorio);
                printf("\n%s", relatorio);
                break;
            }

            case 7: {
                int sub_opcao;
                char termo[50];
                char *resultados[MAX_PRODUTOS];
                int total;

                printf("\n--- Busca de Produtos ---\n");
                printf("1. Por código\n");
                printf("2. Por nome\n");
                printf("Escolha uma opção: ");

                if (scanf("%d", &sub_opcao) != 1) {
                    printf("Opção inválida!\n");
                    limpar_buffer();
                    break;
                }
                limpar_buffer();

                if (sub_opcao == 1) {
                    printf("Digite o código: ");
                } else if (sub_opcao == 2) {
                    printf("Digite o nome ou parte do nome: ");
                } else {
                    printf("Opção inválida!\n");
                    break;
                }

                if (scanf(" %49[^\n]", termo) != 1) {
                    printf("Termo inválido!\n");
                    limpar_buffer();
                    break;
                }
                limpar_buffer();

                int resultado = buscar_produto(sub_opcao, termo, resultados, &total);
                if (resultado == -1) {
                    printf("Opção inválida!\n");
                } else if (total == 0) {
                    printf("Nenhum produto encontrado.\n");
                } else {
                    printf("\nResultados da busca:\n");
                    for (int i = 0; i < total; i++) {
                        printf("%s\n", resultados[i]);
                        free(resultados[i]);
                    }
                    printf("\nTotal encontrado: %d\n", total);
                }
                break;
            }

            case 0:
                printf("Saindo do sistema...\n");
                break;

            default:
                printf("Opção inválida!\n");
        }
    } while (opcao != 0);

    return 0;
}
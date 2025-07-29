#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX 300      // quantidade maxima que o estoque suporta
#define TAM_DATA 11  // tamanho da data dd/mm/aaaa +\0
#define TAM_NOME 51  // quantidade maxima de caracteres que o produto pode ter é 50 +\0

typedef struct {
    char nome[TAM_NOME];
    int quantidade;
    float preco;
    char data_validade[TAM_DATA];  // dd/mm/aaaa
} Produto;

// Funções:
int adicionar_produto(Produto estoque[], int *num_produtos, Produto novo_produto);  // adiciona um novo produto no arranjo de estoque

void verificar_validade(const Produto estoque[], int num_produtos, const char *data_hoje, char **vencidos_nomes, int *num_vencidos);  // verifica o estoque e imprime no terminal os produtos vencidos.

int atualizar_estoque(Produto *produto_para_atualizar, int atualizacao, float valor);  // atualiza o preço ou quantidade de um produto do estoque

int buscar_produto_por_nome(const Produto estoque[], int num_produtos, const char *nome_busca);  // procura um produto no estoque pelo nome.

void gerar_relatorio(const Produto estoque[], int num_produtos, char *relatorio_buffer, int buffer_tamanho);  // gera um relatório de todos os produtos

int extrair_produtos(const char *data_str, int *dia, int *mes, int *ano);

int aplicar_desconto(Produto *produto_para_atulizar, float porcentagem_desconto);

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
        printf("7.Aplicar desconto em % em produtos\n");
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
                printf("\n--- Lista de Produtos em Estoque ---\n");
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
                if (!extrair_data_componentes(data_atual, &dia_hoje, &mes_hoje, &ano_hoje)) {
                    printf("Erro: Formato de data de hoje invalido. Use DD/MM/AAAA.\n");
                    break;  // Sai do case
                }

                if (num_produtos == 0) {
                    printf("Nenhum produto cadastrado.\n");
                } else {
                    int algum_vencido = 0;
                    for (int i = 0; i < num_produtos; i++) {
                        int dia_prod, mes_prod, ano_prod;
                        if (data_componentes(estoque[i].data_validade, &dia_prod, &mes_prod, &ano_prod)) {
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
            case 5: {  // Imprimir relatório no terminal
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
                printf("--- Fim do Relatorio ---\n");
                printf("\n>>> Relatorio impresso no terminal.\n\n");
                break;
            }
            case 6: {  // Buscar produto por nome
                char nome_busca[TAM_NOME];
                printf("Digite o nome do produto a ser buscado: ");
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
                    printf("\n>>> Produto '%s' nao encontrado no estoque.\n\n", nome_busca);
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

// Função auxiliar para extrair componentes da data
int extrair_data_componentes(const char *data_str, int *dia, int *mes, int *ano)
{
    int d = 0, m = 0, y = 0;
    int parte_atual = 0;  // 0 = dia, 1 = mês, 2 = ano
    int i = 0;            // Índice para percorrer a string

    *dia = 0;
    *mes = 0;
    *ano = 0;  // Zera os valores

    while (data_str[i] != '\0') {
        if (data_str[i] >= '0' && data_str[i] <= '9') {
            if (parte_atual == 0) {
                d = d * 10 + (data_str[i] - '0');
            } else if (parte_atual == 1) {
                m = m * 10 + (data_str[i] - '0');
            } else if (parte_atual == 2) {
                y = y * 10 + (data_str[i] - '0');
            }
        } else if (data_str[i] == '/') {
            parte_atual++;
            if (parte_atual > 2)
                return 0;  // Formato inválido
        } else {
            return 0;  // Caractere inválido
        }
        i++;
    }

    if (parte_atual != 2) {
        return 0;
    }  // Formato incompleto

    *dia = d;
    *mes = m;
    *ano = a;
    return 1;  // Sucesso
}

int adicionar_produto(Produto estoque[], int *num_produtos, Produto novo_produto)
{
    if (*num_produtos >= MAX_PRODUTOS) {
        return 0;  // Falha: estoque está cheio
    }
    if (novo_produto.quantidade < 0 || novo_produto.preco <= 0.0) {
        return 0;  // Falha: dados inválidos
    }
    estoque[*num_produtos] = novo_produto;
    (*num_produtos)++;
    return 1;  // Sucesso
}

// para acessar campos de ums estrutura usa o operador ponto: nome.campo
// strcmp da biblioteca string.h  determina se duas sequências de caracteres são idênticas ou qual delas vem primeiro em ordem alfabética.

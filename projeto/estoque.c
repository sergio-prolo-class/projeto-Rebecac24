#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#ifndef PRODUTO_H
#define PRODUTO_H

#define MAX_PRODUTOS 100
#define DATA_TAMANHO 11

typedef struct {
    int codigo;
    char nome[50];
    int quantidade;
    float preco;
    char data_validade[DATA_TAMANHO];
} Produto;

Produto estoque[MAX_PRODUTOS];
int total_produtos = 0;

// Função para limpar o buffer de entrada
void limpar_buffer()
{
    int c;
    while ((c = getchar()) != '\n' && c != EOF) {
    }
}

// Implementação manual de strncpy
void copiar_string(char *dest, const char *src, int n)
{
    int i;
    for (i = 0; i < n && src[i] != '\0'; i++) {
        dest[i] = src[i];
    }
    for (; i < n; i++) {
        dest[i] = '\0';
    }
}

// Implementação manual de strcmp
int comparar_strings(const char *s1, const char *s2)
{
    while (*s1 && (*s1 == *s2)) {
        s1++;
        s2++;
    }
    return *(const unsigned char *)s1 - *(const unsigned char *)s2;
}

#endif

// Função para validar o formato da data (DD/MM/AAAA)
int validar_data(const char *data)
{
    // Verifica o tamanho e as barras
    if (data[0] == '\0' || data[10] != '\0' || data[2] != '/' || data[5] != '/')
        return 0;

    // Verifica se todos os outros caracteres são dígitos
    for (int i = 0; i < 10; i++) {
        if (i != 2 && i != 5 && (data[i] < '0' || data[i] > '9'))
            return 0;
    }

    return 1;
}

// Função para adicionar novo produto
int adicionar_produto(int codigo, const char *nome, int quantidade, float preco, const char *data_validade)
{
    if (total_produtos >= MAX_PRODUTOS)
        return -1;

    for (int i = 0; i < total_produtos; i++) {
        if (estoque[i].codigo == codigo)
            return -2;
    }

    if (quantidade <= 0)
        return -3;
    if (preco <= 0)
        return -4;
    if (!validar_data(data_validade))
        return -5;

    estoque[total_produtos].codigo = codigo;
    copiar_string(estoque[total_produtos].nome, nome, 49);
    estoque[total_produtos].quantidade = quantidade;
    estoque[total_produtos].preco = preco;
    copiar_string(estoque[total_produtos].data_validade, data_validade, DATA_TAMANHO - 1);

    total_produtos++;
    return 0;
}

// Função para atualizar estoque
int atualizar_estoque(int opcao, int codigo, int quantidade, float novo_preco)
{
    int index = -1;
    for (int i = 0; i < total_produtos; i++) {
        if (estoque[i].codigo == codigo) {
            index = i;
            break;
        }
    }
    if (index == -1)
        return -1;

    switch (opcao) {
        case 1:  // Adicionar quantidade
            if (quantidade <= 0)
                return -2;
            estoque[index].quantidade += quantidade;
            return estoque[index].quantidade;

        case 2:  // Remover quantidade
            if (quantidade <= 0)
                return -2;
            if (quantidade > estoque[index].quantidade)
                return -3;
            estoque[index].quantidade -= quantidade;
            return estoque[index].quantidade;

        case 3:  // Alterar preço
            if (novo_preco <= 0)
                return -4;
            estoque[index].preco = novo_preco;
            return 0;

        default: return -5;
    }
}

// Função para verificar validade dos produtos
int verificar_validade(char *resultados[], int *total_resultados)
{
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    char data_atual[DATA_TAMANHO];
    sprintf(data_atual, "%02d/%02d/%04d", tm.tm_mday, tm.tm_mon + 1, tm.tm_year + 1900);

    int count = 0;
    for (int i = 0; i < total_produtos; i++) {
        if (comparar_strings(estoque[i].data_validade, data_atual) < 0) {
            char *buffer = malloc(200);
            sprintf(buffer, "[VENCIDO] Código: %d | Nome: %s | Validade: %s",
                    estoque[i].codigo, estoque[i].nome, estoque[i].data_validade);
            resultados[count++] = buffer;
        }
    }
    *total_resultados = count;
    return count;
}

int tamanho_string(const char *s)
{
    int len = 0;
    while (s[len] != '\0')
        len++;
    return len;
}

void concatenar_string(char *dest, const char *src)
{
    int len = tamanho_string(dest);
    int i = 0;
    while (src[i] != '\0') {
        dest[len + i] = src[i];
        i++;
    }
    dest[len + i] = '\0';
}

// Função para gerar relatório
void gerar_relatorio(char *relatorio)
{
    relatorio[0] = '\0';
    char temp[200];

    sprintf(temp, "--- Relatório Completo do Estoque ---\nTotal de produtos: %d\n\n", total_produtos);
    concatenar_string(relatorio, temp);

    concatenar_string(relatorio, "Código    Nome                Quantidade Preço     Validade\n");
    concatenar_string(relatorio, "----------------------------------------------------------\n");

    float valor_total = 0;
    for (int i = 0; i < total_produtos; i++) {
        sprintf(temp, "%-10d %-20s %-10d R$%-8.2f %-12s\n",
                estoque[i].codigo,
                estoque[i].nome,
                estoque[i].quantidade,
                estoque[i].preco,
                estoque[i].data_validade);
        concatenar_string(relatorio, temp);
        valor_total += estoque[i].quantidade * estoque[i].preco;
    }

    sprintf(temp, "\nValor total em estoque: R$%.2f\n", valor_total);
    concatenar_string(relatorio, temp);
}

// Função para buscar produtos
int buscar_produto(int opcao, const char *termo, char *resultados[], int *total_resultados)
{
    *total_resultados = 0;
    int count = 0;

    if (opcao == 1) {  // Busca por código
        int codigo = atoi(termo);
        for (int i = 0; i < total_produtos; i++) {
            if (estoque[i].codigo == codigo) {
                char *buffer = malloc(200);
                sprintf(buffer, "Código: %d | Nome: %s | Quantidade: %d | Preço: R$%.2f | Validade: %s",
                        estoque[i].codigo, estoque[i].nome, estoque[i].quantidade,
                        estoque[i].preco, estoque[i].data_validade);
                resultados[count++] = buffer;
                break;
            }
        }
    } else if (opcao == 2) {  // Busca por nome
        for (int i = 0; i < total_produtos; i++) {
            const char *pos = estoque[i].nome;
            const char *needle = termo;

            while (*pos) {
                if (*pos == *needle) {
                    const char *p = pos + 1;
                    const char *q = needle + 1;
                    while (*q && *p == *q) {
                        p++;
                        q++;
                    }
                    if (!*q) {
                        char *buffer = malloc(200);
                        sprintf(buffer, "[%d] %s - %d unidades - R$%.2f - Val: %s",
                                estoque[i].codigo,
                                estoque[i].nome,
                                estoque[i].quantidade,
                                estoque[i].preco,
                                estoque[i].data_validade);
                        resultados[count++] = buffer;
                        break;
                    }
                }
                pos++;
            }
        }
    } else {
        return -1;
    }

    *total_resultados = count;
    return count;
}

// Função para salvar estoque em arquivo
int salvar_estoque()
{
    FILE *arquivo = fopen("estoque.txt", "w");
    if (!arquivo)
        return -1;

    for (int i = 0; i < total_produtos; i++) {
        fprintf(arquivo, "%d;%s;%d;%.2f;%s\n",
                estoque[i].codigo,
                estoque[i].nome,
                estoque[i].quantidade,
                estoque[i].preco,
                estoque[i].data_validade);
    }

    fclose(arquivo);
    return 0;
}

// Função para carregar estoque do arquivo
int carregar_estoque()
{
    FILE *arquivo = fopen("estoque.txt", "r");
    if (!arquivo)
        return -1;

    total_produtos = 0;
    char linha[200];

    while (fgets(linha, sizeof(linha), arquivo) && total_produtos < MAX_PRODUTOS) {
        int i = 0, j = 0;

        // Parse do código
        estoque[total_produtos].codigo = 0;
        while (linha[i] != ';') {
            estoque[total_produtos].codigo = estoque[total_produtos].codigo * 10 + (linha[i] - '0');
            i++;
        }
        i++;

        // Parse do nome
        j = 0;
        while (linha[i] != ';' && j < 49) {
            estoque[total_produtos].nome[j++] = linha[i++];
        }
        estoque[total_produtos].nome[j] = '\0';
        i++;

        // Parse da quantidade
        estoque[total_produtos].quantidade = 0;
        while (linha[i] != ';') {
            estoque[total_produtos].quantidade = estoque[total_produtos].quantidade * 10 + (linha[i] - '0');
            i++;
        }
        i++;

        // Parse do preço
        float frac = 0.1f;
        estoque[total_produtos].preco = 0;
        while (linha[i] != ';') {
            if (linha[i] == '.') {
                i++;
                while (linha[i] != ';') {
                    estoque[total_produtos].preco += (linha[i] - '0') * frac;
                    frac *= 0.1f;
                    i++;
                }
            } else {
                estoque[total_produtos].preco = estoque[total_produtos].preco * 10 + (linha[i] - '0');
                i++;
            }
        }
        i++;

        // Parse da data
        j = 0;
        while (linha[i] != '\0' && linha[i] != '\n' && j < DATA_TAMANHO - 1) {
            estoque[total_produtos].data_validade[j++] = linha[i++];
        }
        estoque[total_produtos].data_validade[j] = '\0';

        total_produtos++;
    }

    fclose(arquivo);
    return total_produtos;
}

// Função principal
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

                // Loop até receber uma data válida
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
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "estoque.h"
#include "util.h"

Produto estoque[MAX_PRODUTOS];
int total_produtos = 0;

int validar_data(const char *data)
{
    if (data[0] == '\0' || data[10] != '\0' || data[2] != '/' || data[5] != '/')
        return 0;
    for (int i = 0; i < 10; i++) {
        if (i != 2 && i != 5 && (data[i] < '0' || data[i] > '9'))
            return 0;
    }
    return 1;
}

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
        case 1:
            if (quantidade <= 0)
                return -2;
            estoque[index].quantidade += quantidade;
            return estoque[index].quantidade;
        case 2:
            if (quantidade <= 0)
                return -2;
            if (quantidade > estoque[index].quantidade)
                return -3;
            estoque[index].quantidade -= quantidade;
            return estoque[index].quantidade;
        case 3:
            if (novo_preco <= 0)
                return -4;
            estoque[index].preco = novo_preco;
            return 0;
        default: return -5;
    }
}

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

int buscar_produto(int opcao, const char *termo, char *resultados[], int *total_resultados)
{
    *total_resultados = 0;
    int count = 0;

    if (opcao == 1) {
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
    } else if (opcao == 2) {
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

int carregar_estoque()
{
    FILE *arquivo = fopen("estoque.txt", "r");
    if (!arquivo)
        return -1;

    total_produtos = 0;
    char linha[200];

    while (fgets(linha, sizeof(linha), arquivo) && total_produtos < MAX_PRODUTOS) {
        int i = 0, j = 0;

        estoque[total_produtos].codigo = 0;
        while (linha[i] != ';') {
            estoque[total_produtos].codigo = estoque[total_produtos].codigo * 10 + (linha[i] - '0');
            i++;
        }
        i++;

        j = 0;
        while (linha[i] != ';' && j < 49) {
            estoque[total_produtos].nome[j++] = linha[i++];
        }
        estoque[total_produtos].nome[j] = '\0';
        i++;

        estoque[total_produtos].quantidade = 0;
        while (linha[i] != ';') {
            estoque[total_produtos].quantidade = estoque[total_produtos].quantidade * 10 + (linha[i] - '0');
            i++;
        }
        i++;

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
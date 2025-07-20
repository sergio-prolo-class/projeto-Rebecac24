#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "estoque.h"
#include "util.h"

Produto estoque[MAX_PRODUTOS];
int total_produtos = 0;

// valida a data no formato dd/mm/aaaa
int validar_data(const char *data)
{
    if (!data || data[2] != '/' || data[5] != '/' || data[10] != '\0')
        return 0;
    for (int i = 0; i < 10; i++) {
        if ((i == 2 || i == 5))
            continue;
        if (data[i] < '0' || data[i] > '9')
            return 0;
    }
    return 1;
}

// Adiciona produto ao estoque
int adicionar_produto(int codigo, const char *nome, int quantidade, float preco, const char *data_validade)
{
    if (total_produtos >= MAX_PRODUTOS)
        return -1;
    for (int i = 0; i < total_produtos; i++)
        if (estoque[i].codigo == codigo)
            return -2;
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

// Atualiza quantidade ou o preço do produto
int atualizar_estoque(int opcao, int codigo, int quantidade, float novo_preco)
{
    int idx = -1;
    for (int i = 0; i < total_produtos; i++)
        if (estoque[i].codigo == codigo) {
            idx = i;
            break;
        }
    if (idx == -1)
        return -1;

    if (opcao == 1) {  // Entrada
        if (quantidade <= 0)
            return -2;
        estoque[idx].quantidade += quantidade;
        return estoque[idx].quantidade;
    } else if (opcao == 2) {  // Saída
        if (quantidade <= 0)
            return -2;
        if (quantidade > estoque[idx].quantidade)
            return -3;
        estoque[idx].quantidade -= quantidade;
        return estoque[idx].quantidade;
    } else if (opcao == 3) {  // Atualiza preço
        if (novo_preco <= 0)
            return -4;
        estoque[idx].preco = novo_preco;
        return 0;
    }
    return -5;
}

// verifica se tem produtos vencidos
int verificar_validade(char *resultados[], int *total_resultados)
{
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    int dia_atual = tm.tm_mday;
    int mes_atual = tm.tm_mon + 1;
    int ano_atual = tm.tm_year + 1900;

    int count = 0;
    for (int i = 0; i < total_produtos; i++) {
        // Parse manual da data de validade (dd/mm/aaaa)
        int dia = (estoque[i].data_validade[0] - '0') * 10 + (estoque[i].data_validade[1] - '0');
        int mes = (estoque[i].data_validade[3] - '0') * 10 + (estoque[i].data_validade[4] - '0');
        int ano = (estoque[i].data_validade[6] - '0') * 1000 +
                  (estoque[i].data_validade[7] - '0') * 100 +
                  (estoque[i].data_validade[8] - '0') * 10 +
                  (estoque[i].data_validade[9] - '0');

        // Produto vencido se data de validade < data atual
        int vencido = 0;
        if (ano < ano_atual)
            vencido = 1;
        else if (ano == ano_atual && mes < mes_atual)
            vencido = 1;
        else if (ano == ano_atual && mes == mes_atual && dia < dia_atual)
            vencido = 1;

        if (vencido) {
            char *buffer = malloc(200);
            sprintf(buffer, "[VENCIDO] Código: %d | Nome: %s | Validade: %s",
                    estoque[i].codigo, estoque[i].nome, estoque[i].data_validade);
            resultados[count++] = buffer;
        }
    }
    *total_resultados = count;
    return count;
}

// gera relatorio do estoque
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

// busca produto por código ou nome
int buscar_produto(int opcao, const char *termo, char *resultados[], int *total_resultados)
{
    *total_resultados = 0;
    int count = 0;

    if (opcao == 1) {  // Busca por código
        int codigo = 0, i = 0;
        while (termo[i] >= '0' && termo[i] <= '9') {
            codigo = codigo * 10 + (termo[i] - '0');
            i++;
        }
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
    } else if (opcao == 2) {  // Busca por nome (substring manual)
        for (int i = 0; i < total_produtos; i++) {
            int found = 0;
            for (int j = 0; estoque[i].nome[j] != '\0'; j++) {
                int k = 0;
                while (termo[k] != '\0' && estoque[i].nome[j + k] == termo[k])
                    k++;
                if (termo[k] == '\0') {
                    found = 1;
                    break;
                }
            }
            if (found) {
                char *buffer = malloc(200);
                sprintf(buffer, "[%d] %s - %d unidades - R$%.2f - Val: %s",
                        estoque[i].codigo,
                        estoque[i].nome,
                        estoque[i].quantidade,
                        estoque[i].preco,
                        estoque[i].data_validade);
                resultados[count++] = buffer;
            }
        }
    } else {
        return -1;
    }
    *total_resultados = count;
    return count;
}

// Salva estoque em arquivo texto
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

// Carrega estoque do arquivo texto
int carregar_estoque()
{
    FILE *arquivo = fopen("estoque.txt", "r");
    if (!arquivo)
        return -1;
    total_produtos = 0;
    char linha[200];
    while (fgets(linha, sizeof(linha), arquivo) && total_produtos < MAX_PRODUTOS) {
        int i = 0, j = 0;
        // Código
        int codigo = 0;
        while (linha[i] != ';' && linha[i] != '\0') {
            codigo = codigo * 10 + (linha[i] - '0');
            i++;
        }
        estoque[total_produtos].codigo = codigo;
        i++;  // pula ';'
        // Nome
        j = 0;
        while (linha[i] != ';' && linha[i] != '\0' && j < 49) {
            estoque[total_produtos].nome[j++] = linha[i++];
        }
        estoque[total_produtos].nome[j] = '\0';
        i++;
        // Quantidade
        int quantidade = 0;
        while (linha[i] != ';' && linha[i] != '\0') {
            quantidade = quantidade * 10 + (linha[i] - '0');
            i++;
        }
        estoque[total_produtos].quantidade = quantidade;
        i++;
        // Preço
        float preco = 0, frac = 0.1f;
        int decimal = 0;
        while (linha[i] != ';' && linha[i] != '\0') {
            if (linha[i] == '.') {
                decimal = 1;
                i++;
                continue;
            }
            if (!decimal)
                preco = preco * 10 + (linha[i] - '0');
            else {
                preco += (linha[i] - '0') * frac;
                frac *= 0.1f;
            }
            i++;
        }
        estoque[total_produtos].preco = preco;
        i++;
        // Data validade
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
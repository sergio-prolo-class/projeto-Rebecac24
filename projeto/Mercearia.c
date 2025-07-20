#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MAX_PRODUTOS 500
#define DATA_LENGTH 11

typedef struct {
    int codigo;
    char nome[50];
    int quantidade;
    float preco;
    char data_validade[DATA_LENGTH];
} Produto;

Produto estoque[MAX_PRODUTOS];
int total_produtos = 0;

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

    estoque[total_produtos].codigo = codigo;
    copiar_string(estoque[total_produtos].nome, nome, 49);
    estoque[total_produtos].quantidade = quantidade;
    estoque[total_produtos].preco = preco;
    copiar_string(estoque[total_produtos].data_validade, data_validade, DATA_LENGTH - 1);

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

int comparar_strings(const char *s1, const char *s2)
{
    while (*s1 && (*s1 == *s2)) {
        s1++;
        s2++;
    }
    return *(const unsigned char *)s1 - *(const unsigned char *)s2;
}

int verificar_validade(char *resultados[], int *total_resultados)
{
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    char data_atual[DATA_LENGTH];
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

int main()
{
    return 0;
}
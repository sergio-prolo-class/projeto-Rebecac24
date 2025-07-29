#include "estoque.h"
#include <string.h>
#include <stdio.h>

// Função de componentes da data
int extrair_data_componentes(const char *data_str, int *dia, int *mes, int *ano)
{
    int d = 0, m = 0, a = 0;
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
                a = a * 10 + (data_str[i] - '0');
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
    if (*num_produtos >= MAX) {
        return 0;  // Falha: estoque está cheio
    }
    if (novo_produto.quantidade < 0 || novo_produto.preco <= 0.0) {
        return 0;  // Falha: dados inválidos
    }
    estoque[*num_produtos] = novo_produto;
    (*num_produtos)++;
    return 1;  // Sucesso
}

void verificar_validade(const Produto estoque[], int num_produtos, const char *data_hoje, char **vencidos_nomes, int *num_vencidos) {}

int atualizar_estoque(Produto *produto_alvo, int operacao, float valor)
{
    switch (operacao) {
        case 1:  // Adicionar quantidade
            if (valor > 0) {
                produto_alvo->quantidade += (int)valor;
                return 1;
            }
            break;
        case 2:  // Remover quantidade
            if (valor > 0 && produto_alvo->quantidade >= (int)valor) {
                produto_alvo->quantidade -= (int)valor;
                return 1;
            }
            break;
        case 3:  // Alterar preço
            if (valor > 0.0) {
                produto_alvo->preco = valor;
                return 1;
            }
            break;
    }
    return 0;  // Falha na operação
}

// Busca um produto no estoque (apenas lógica, sem printf/scanf)
int buscar_produto_por_nome(const Produto estoque[], int num_produtos, const char *nome_busca)
{
    for (int i = 0; i < num_produtos; i++) {
        if (strcmp(estoque[i].nome, nome_busca) == 0) {
            return i;  // Retorna o índice se encontrado
        }
    }
    return -1;  // Não encontrado
}

int aplicar_desconto(Produto *produto_alvo, float porcentagem_desconto)
{
    if (porcentagem_desconto >= 0 && porcentagem_desconto <= 100) {
        produto_alvo->preco = produto_alvo->preco * (1 - (porcentagem_desconto / 100.0));
        return 1;  // Sucesso
    }
    return 0;  // Porcentagem inválida
}

// para acessar campos de ums estrutura usa o operador ponto: nome.campo
// strcmp da biblioteca string.h  determina se duas sequências de caracteres são idênticas ou qual delas vem primeiro em ordem alfabética.

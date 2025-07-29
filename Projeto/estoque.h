#ifndef ESTOQUE_H
#define ESTOQUE_H

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

int extrair_data_componentes(const char *data_str, int *dia, int *mes, int *ano);

#endif

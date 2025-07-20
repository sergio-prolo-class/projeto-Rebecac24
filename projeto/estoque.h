#ifndef ESTOQUE_H
#define ESTOQUE_H

#define MAX_PRODUTOS 100
#define DATA_TAMANHO 11

typedef struct {
    int codigo;
    char nome[50];
    int quantidade;
    float preco;
    char data_validade[DATA_TAMANHO];
} Produto;

extern Produto estoque[MAX_PRODUTOS];
extern int total_produtos;

int adicionar_produto(int codigo, const char *nome, int quantidade, float preco, const char *data_validade);
int atualizar_estoque(int opcao, int codigo, int quantidade, float novo_preco);
int verificar_validade(char *resultados[], int *total_resultados);
void gerar_relatorio(char *relatorio);
int buscar_produto(int opcao, const char *termo, char *resultados[], int *total_resultados);
int salvar_estoque();
int carregar_estoque();
int validar_data(const char *data);

#endif
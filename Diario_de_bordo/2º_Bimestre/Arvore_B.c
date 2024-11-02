// Nome: Heberth Santos de Assis  Turma: CC6N

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define T 3 // Ordem mínima da árvore B (cada nó pode ter até 2*T-1 chaves)

// Estrutura para o nó da árvore B
typedef struct btree_no {
    int chaves[2*T - 1];
    struct btree_no* filhos[2*T];
    int n; // Número de chaves atuais no nó
    bool folha; // boolano que verifica se o nó é uma folha
} btree_no;

// Função para criar um novo nó da árvore B
btree_no* criar_no(bool folha) {
    btree_no* no = (btree_no*)malloc(sizeof(btree_no));
    no->folha = folha;
    no->n = 0;
    for (int i = 0; i < 2*T; i++)
        no->filhos[i] = NULL;
    return no;
}

// Função para buscar uma chave na árvore B
btree_no* pesquisa(btree_no* raiz, int chave) {
    int i = 0;
    while (i < raiz->n && chave > raiz->chaves[i])
        i++;
    
    if (i < raiz->n && raiz->chaves[i] == chave)
        return raiz;

    if (raiz->folha)
        return NULL;

    return pesquisa(raiz->filhos[i], chave);
}

// Função para dividir o filho y de um nó
void dividir_filho(btree_no* pai, int i, btree_no* y) {
    btree_no* z = criar_no(y->folha);
    z->n = T - 1;

    for (int j = 0; j < T - 1; j++)
        z->chaves[j] = y->chaves[j + T];

    if (!y->folha) {
        for (int j = 0; j < T; j++)
            z->filhos[j] = y->filhos[j + T];
    }

    y->n = T - 1;

    for (int j = pai->n; j >= i + 1; j--)
        pai->filhos[j + 1] = pai->filhos[j];
    
    pai->filhos[i + 1] = z;

    for (int j = pai->n - 1; j >= i; j--)
        pai->chaves[j + 1] = pai->chaves[j];

    pai->chaves[i] = y->chaves[T - 1];
    pai->n++;
}

// Função para inserir uma chave em um nó não cheio
void inserir_nao_cheio(btree_no* no, int chave) {
    int i = no->n - 1;

    if (no->folha) {
        while (i >= 0 && chave < no->chaves[i]) {
            no->chaves[i + 1] = no->chaves[i];
            i--;
        }
        no->chaves[i + 1] = chave;
        no->n++;
    } else {
        while (i >= 0 && chave < no->chaves[i])
            i--;

        i++;
        if (no->filhos[i]->n == 2*T - 1) {
            dividir_filho(no, i, no->filhos[i]);
            if (chave > no->chaves[i])
                i++;
        }
        inserir_nao_cheio(no->filhos[i], chave);
    }
}

// Função para inserir uma chave na árvore B
void inserir(btree_no** raiz, int chave) {
    if ((*raiz)->n == 2*T - 1) {
        btree_no* nova_raiz = criar_no(false);
        nova_raiz->filhos[0] = *raiz;
        dividir_filho(nova_raiz, 0, *raiz);
        int i = (chave > nova_raiz->chaves[0]) ? 1 : 0;
        inserir_nao_cheio(nova_raiz->filhos[i], chave);
        *raiz = nova_raiz;
    } else {
        inserir_nao_cheio(*raiz, chave);
    }
}

// Função auxiliar para obter o antecessorecessor de uma chave
int get_antecessor(btree_no* no) {
    while (!no->folha)
        no = no->filhos[no->n];
    return no->chaves[no->n - 1];
}

// Função auxiliar para obter o sucessor de uma chave
int get_sucessor(btree_no* no) {
    while (!no->folha)
        no = no->filhos[0];
    return no->chaves[0];
}

// Função para remover uma chave da árvore B
void remove_chave(btree_no* no, int chave) {
    int indice = 0;
    while (indice < no->n && no->chaves[indice] < chave)
        indice++;

    if (indice < no->n && no->chaves[indice] == chave) {
        if (no->folha) {
            for (int i = indice + 1; i < no->n; i++)
                no->chaves[i - 1] = no->chaves[i];
            no->n--;
        } else if (no->filhos[indice]->n >= T) {
            int antecessor = get_antecessor(no->filhos[indice]);
            no->chaves[indice] = antecessor;
            remove_chave(no->filhos[indice], antecessor);
        } else if (no->filhos[indice + 1]->n >= T) {
            int sucessor = get_sucessor(no->filhos[indice + 1]);
            no->chaves[indice] = sucessor;
            remove_chave(no->filhos[indice + 1], sucessor);
        } else {
            btree_no* filho = no->filhos[indice];
            btree_no* irmao = no->filhos[indice + 1];
            filho->chaves[T - 1] = no->chaves[indice];
            for (int i = 0; i < irmao->n; i++)
                filho->chaves[i + T] = irmao->chaves[i];
            if (!filho->folha) {
                for (int i = 0; i <= irmao->n; i++)
                    filho->filhos[i + T] = irmao->filhos[i];
            }
            for (int i = indice + 1; i < no->n; i++)
                no->chaves[i - 1] = no->chaves[i];
            for (int i = indice + 2; i <= no->n; i++)
                no->filhos[i - 1] = no->filhos[i];
            filho->n += irmao->n + 1;
            no->n--;
            free(irmao);
            remove_chave(filho, chave);
        }
    } else {
        if (no->folha) {
            printf("A chave %d não está na árvore\n", chave);
            return;
        }
        bool flag = (indice == no->n);
        if (no->filhos[indice]->n < T)
            if (flag && indice > no->n)
                remove_chave(no->filhos[indice - 1], chave);
            else
                remove_chave(no->filhos[indice], chave);
    }
}

// Função para exibir a árvore B
void print_tree(btree_no* no, int level) {
    if (no != NULL) {
        for (int i = 0; i < no->n; i++) {
            print_tree(no->filhos[i], level + 1);
            printf("(%d) ", no->chaves[i]);
        }
        print_tree(no->filhos[no->n], level + 1);
    }
}

int main() {
    btree_no* raiz = criar_no(true);
    int opcao, chave;

    while (1) {
        printf("\n--- Menu ---\n");
        printf("1. Inserir\n");
        printf("2. Buscar\n");
        printf("3. Remover\n");
        printf("4. Exibir Árvore\n");
        printf("5. Sair\n");
        printf("Escolha uma opção: ");
        scanf("%d", &opcao);

        switch (opcao) {
            case 1:
                printf("Digite a chave a ser inserida: ");
                scanf("%d", &chave);
                inserir(&raiz, chave);
                printf("Chave %d inserida.\n", chave);
                break;
            case 2:
                printf("Digite a chave a ser buscada: ");
                scanf("%d", &chave);
                if (pesquisa(raiz, chave))
                    printf("Chave %d encontrada.\n", chave);
                else
                    printf("Chave %d não encontrada.\n", chave);
                break;
            case 3:
                printf("Digite a chave a ser removida: ");
                scanf("%d", &chave);
                remove_chave(raiz, chave);
                printf("Chave %d removida.\n", chave);
                break;
            case 4:
                printf("Árvore B:\n");
                print_tree(raiz, 0);
                printf("\n");
                break;
            case 5:
                printf("FIM\n");
                return 0;
            default:
                printf("Opção inválida. Tente novamente.\n");
        }
    }
}

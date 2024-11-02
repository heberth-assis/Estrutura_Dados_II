//NOME: HEBERTH SANTOS DE ASSIS  TURMA: CC6N

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

typedef struct treap_no {
    int chave;
    int prioridade;
    struct treap_no *esquerdo, *direito;
} treap_no;

typedef treap_no* Treap;

// Função para criar um novo nó
treap_no* criar_no(int chave) {
    treap_no* no = (treap_no*)malloc(sizeof(treap_no));
    no->chave = chave;
    no->prioridade = rand() % 100; // Prioridade aleatória
    no->esquerdo = no->direito = NULL;
    return no;
}

// Rotação à direita
treap_no* rotacionar_direita(treap_no* y) {
    treap_no* x = y->esquerdo;
    y->esquerdo = x->direito;
    x->direito = y;
    return x;
}

// Rotação à esquerda
treap_no* rotacionar_esquerda(treap_no* x) {
    treap_no* y = x->direito;
    x->direito = y->esquerdo;
    y->esquerdo = x;
    return y;
}

// Função de inserção
treap_no* inserir(treap_no* raiz, int chave) {
    if (raiz == NULL) return criar_no(chave);

    if (chave < raiz->chave) {
        raiz->esquerdo = inserir(raiz->esquerdo, chave);
        if (raiz->esquerdo->prioridade > raiz->prioridade)
            raiz = rotacionar_direita(raiz);
    } else if (chave > raiz->chave) {
        raiz->direito = inserir(raiz->direito, chave);
        if (raiz->direito->prioridade > raiz->prioridade)
            raiz = rotacionar_esquerda(raiz);
    }
    return raiz;
}

// Função de busca
treap_no* busca(treap_no* raiz, int chave) {
    if (raiz == NULL || raiz->chave == chave)
        return raiz;

    if (chave < raiz->chave)
        return busca(raiz->esquerdo, chave);
    else
        return busca(raiz->direito, chave);
}

// Função de remoção
treap_no* deletar(treap_no* raiz, int chave) {
    if (raiz == NULL) return raiz;

    if (chave < raiz->chave) {
        raiz->esquerdo = deletar(raiz->esquerdo, chave);
    } else if (chave > raiz->chave) {
        raiz->direito = deletar(raiz->direito, chave);
    } else {
        if (raiz->esquerdo == NULL) {
            treap_no* temp = raiz->direito;
            free(raiz);
            return temp;
        } else if (raiz->direito == NULL) {
            treap_no* temp = raiz->esquerdo;
            free(raiz);
            return temp;
        }

        if (raiz->esquerdo->prioridade < raiz->direito->prioridade) {
            raiz = rotacionar_esquerda(raiz);
            raiz->esquerdo = deletar(raiz->esquerdo, chave);
        } else {
            raiz = rotacionar_direita(raiz);
            raiz->direito = deletar(raiz->direito, chave);
        }
    }
    return raiz;
}

// Função para exibir a treap em ordem
void em_ordem(treap_no* raiz) {
    if (raiz != NULL) {
        em_ordem(raiz->esquerdo);
        printf("Chave: %d, Prioridade: %d\n", raiz->chave, raiz->prioridade);
        em_ordem(raiz->direito);
    }
}

int main() {
    srand(time(0)); // Inicializa com números aleatórios

    Treap treap = NULL;

    // Inserindo elementos
    treap = inserir(treap, 50);
    treap = inserir(treap, 30);
    treap = inserir(treap, 20);
    treap = inserir(treap, 40);
    treap = inserir(treap, 70);
    treap = inserir(treap, 60);
    treap = inserir(treap, 80);

    printf("Treap em ordem:\n");
    em_ordem(treap);

    // Buscando elemento
    int chave = 40;
    treap_no* resultado = busca(treap, chave);
    if (resultado)
        printf("\nElemento %d encontrado com prioridade %d\n", chave, resultado->prioridade);
    else
        printf("\nElemento %d não encontrado\n", chave);

    // Removendo elemento
    printf("\nRemovendo o elemento 50\n");
    treap = deletar(treap, 50);
    printf("Treap em ordem após remoção:\n");
    em_ordem(treap);

    return 0;
}

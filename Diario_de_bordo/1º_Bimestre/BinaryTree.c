// ALUNO: HEBERTH SANTOS DE ASSIS    TURMA: CC6N 

#include <stdio.h>
#include <stdlib.h>

// Defino uma estrutura para representar um nó na árvore binária
typedef struct no {
	int conteudo; // Este campo armazena o valor do nó
	struct no *esquerdo, *direito; // Ponteiros para os nós filhos (esquerdo e direito)
} No;

// Defino uma estrutura para representar a árvore binária
typedef struct {
	No *raiz; // Ponteiro para o nó raiz da árvore
} arv_bin;

// Função para inserir um valor na subárvore esquerda
void inserir_esquerda(No *no, int valor) {
	// Verifico se o filho esquerdo está vazio
	if(no->esquerdo == NULL) {
		// Se estiver vazio, crio um novo nó e o insiro como filho esquerdo
		No *novo = (No*)malloc(sizeof(No));
		novo->conteudo = valor;
		novo->esquerdo = NULL;
		novo->direito = NULL;
		no->esquerdo = novo;
	} else {
		// Se já houver um nó à esquerda, verifico onde o novo valor deve ser inserido
		if(valor < no->esquerdo->conteudo) {
			inserir_esquerda(no->esquerdo, valor); // Insiro à esquerda
		}
		if(valor > no->esquerdo->conteudo) {
			inserir_direita(no->esquerdo, valor); // Insiro à direita
		}
	}
}

// Função para inserir um valor na subárvore direita (igual da inserção a esquerda)
void inserir_direita(No *no, int valor) {
	if(no->direito == NULL) {
		No *novo = (No*)malloc(sizeof(No));
		novo->conteudo = valor;
		novo->esquerdo = NULL;
		novo->direito = NULL;
		no->direito = novo;
	} else {
		if(valor > no->direito->conteudo) {
			inserir_direita(no->direito, valor);
		}
		if(valor < no->direito->conteudo) {
			inserir_esquerda(no->direito, valor);
		}
	}
}

// Função para inserir um valor na árvore binária
void inserir(arv_bin *arvore, int valor) {
	// Verifico se a árvore está vazia, se não tem raiz)
	if (arvore->raiz == NULL) {
		// Se estiver vazia, crio um novo nó e o defino como raiz
		No *novo = (No*)malloc(sizeof(No));
		novo->conteudo = valor;
		novo->esquerdo = NULL;
		novo->direito = NULL;
		arvore->raiz = novo;
	} else {
		// Se já houver uma raiz, verifico onde o valor deve ser inserido
		if (valor < arvore->raiz->conteudo) {
			inserir_esquerda(arvore->raiz, valor); // Insiro à esquerda
		}
		if(valor > arvore->raiz->conteudo) {
			inserir_direita(arvore->raiz, valor); // Insiro à direita
		}
	}
}

// Função para imprimir a árvore em ordem (esquerda, raiz, direita)
void imprimir(No *raiz) {
	if(raiz != NULL) {
		imprimir(raiz->esquerdo); // Imprime a subárvore esquerda
		printf("%d ", raiz->conteudo); // Imprime o conteúdo do nó
		imprimir(raiz->direito); // Imprime a subárvore direita
	}
}

// Função para buscar um valor na árvore
int buscar(No *raiz, int chave) {
	if(raiz == NULL) {
	    // Se o nó for nulo, o valor não foi encontrado
	    printf("Valor não encontrado\n");
	    return 0; // Retorno 0 para indicar "não encontrado"
	} else {
		// Se o valor do nó for igual à chave, o valor foi encontrado
		if(raiz->conteudo == chave) {
		    printf("O valor está presente na árvore\n");
			return 1; // Retorno 1 para indicar "encontrado"
		} else {
			// Se a chave for menor que o valor do nó, busco na subárvore esquerda
			if(chave < raiz->conteudo) {
				return buscar(raiz->esquerdo, chave);
			} else {
				// Se a chave for maior, busco na subárvore direita
				return buscar(raiz->direito, chave);
			}
		}
	}
}

// Função para remover um nó da árvore
No* remover(No *raiz, int valor) {
	if(raiz == NULL) {
		// Se o nó for nulo, não há nada para remover
		return NULL;
	}

	// Se o valor a ser removido for menor que o valor do nó, continuo na subárvore esquerda
	if(valor < raiz->conteudo) {
		raiz->esquerdo = remover(raiz->esquerdo, valor);
	} 
	// Se o valor a ser removido for maior que o valor do nó, continuo na subárvore direita
	else if(valor > raiz->conteudo) {
		raiz->direito = remover(raiz->direito, valor);
	} 
	// Se o valor for igual ao valor do nó, encontrei o nó a ser removido
	else {
		// Caso 1: Nó sem filhos (nó folha)
		if(raiz->esquerdo == NULL && raiz->direito == NULL) {
			free(raiz); // Libero a memória do nó
			return NULL; // Retorno NULL porque o nó foi removido
		}
		// Caso 2: Nó com apenas um filho (esquerdo ou direito)
		else if(raiz->esquerdo == NULL) {
			No *temp = raiz->direito; // Salvo o filho direito
			free(raiz); // Libero a memória do nó
			return temp; // Retorno o filho direito para substituir o nó removido
		} 
		else if(raiz->direito == NULL) {
			No *temp = raiz->esquerdo; // Salvo o filho esquerdo
			free(raiz); // Libero a memória do nó
			return temp; // Retorno o filho esquerdo para substituir o nó removido
		} 
		// Caso 3: Nó com dois filhos
		else {
			No *temp = raiz->direito;
			// Encontro o menor valor na subárvore direita (sucessor in-order)
			while(temp->esquerdo != NULL) {
				temp = temp->esquerdo;
			}
			// Substituo o valor do nó pelo valor do sucessor in-order
			raiz->conteudo = temp->conteudo;
			// Removo o nó sucessor in-order da subárvore direita
			raiz->direito = remover(raiz->direito, temp->conteudo);
		}
	}

	return raiz; // Retorno a nova raiz após a remoção
}

// Função principal
int main(void) {
	int opcao, valor; // Variáveis para a opção do menu e o valor a ser inserido/buscado/removido
	arv_bin arvore; // Crio uma árvore binária
	arvore.raiz = NULL; // Inicializo a raiz da árvore como NULL (árvore vazia)

	do {
		// Exibo o menu de opções para o usuário
		printf("\n0 - SAIR\n1 - INSERIR\n2 - IMPRIMIR ÁRVORE\n3 - BUSCAR\n4 - REMOVER\n");
		scanf("%d", &opcao);

		switch(opcao) {
		case 0:
			printf("\nFim"); // Opção para sair do programa
			break;

		case 1:
			printf("Digite um valor:");
			scanf("%d", &valor);
			inserir(&arvore, valor); // Insiro o valor na árvore
			break;

		case 2:
			printf("\nArvore impressa: \n");
			imprimir(arvore.raiz); // Imprimo a árvore em ordem
			break;

		case 3:
			printf("Qual valor quer buscar? ");
			scanf("%d", &valor);
			printf("Resultado da busca: %d\n", buscar(arvore.raiz, valor)); // Busco o valor na árvore
			break;
	    
	    case 4: // Novo caso para remover
        	printf("Digite o valor que deseja remover: ");
        	scanf("%d", &valor);
        	arvore.raiz = remover(arvore.raiz, valor); // Removo o valor da árvore
        	printf("Valor removido.\n");
	        break;

		default:
			printf("\nOpção invalida\n"); // Mensagem de erro para opção inválida
		}

	} while (opcao != 0); // Repito até o usuário escolher a opção de sair

	return 0; // Retorno 0 para indicar que o programa terminou corretamente
}

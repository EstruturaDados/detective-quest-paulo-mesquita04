#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* ==========================
   ESTRUTURA: SALAS (ÁRVORE)
   ========================== */

typedef struct Sala {
    char nome[40];
    char pista[40];  /* "" se não tiver pista */
    struct Sala *esq;
    struct Sala *dir;
} Sala;

Sala* criarSala(/* sem const para compatibilidade máxima */ char *nome, char *pista) {
    Sala *s;
    s = (Sala*) malloc(sizeof(Sala));
    if (s == NULL) {
        printf("Erro: memoria insuficiente\n");
        exit(1);
    }
    strcpy(s->nome, nome);
    strcpy(s->pista, pista);
    s->esq = NULL;
    s->dir = NULL;
    return s;
}

/* ==========================
   ESTRUTURA: BST DE PISTAS
   ========================== */

typedef struct NodoBST {
    char pista[40];
    struct NodoBST *esq;
    struct NodoBST *dir;
} NodoBST;

NodoBST* criarNodoPista(char *p) {
    NodoBST *novo;
    novo = (NodoBST*) malloc(sizeof(NodoBST));
    if (novo == NULL) {
        printf("Erro: memoria insuficiente\n");
        exit(1);
    }
    strcpy(novo->pista, p);
    novo->esq = NULL;
    novo->dir = NULL;
    return novo;
}

NodoBST* inserirPista(NodoBST *raiz, char *p) {
    int cmp;
    if (raiz == NULL) {
        return criarNodoPista(p);
    }

    cmp = strcmp(p, raiz->pista);
    if (cmp < 0) {
        raiz->esq = inserirPista(raiz->esq, p);
    } else if (cmp > 0) {
        raiz->dir = inserirPista(raiz->dir, p);
    }
    /* se igual, não insere duplicata */
    return raiz;
}

void listarPistas(NodoBST *raiz) {
    if (raiz == NULL) return;
    listarPistas(raiz->esq);
    printf(" - %s\n", raiz->pista);
    listarPistas(raiz->dir);
}

/* ==========================
   EXPLORAÇÃO DA MANSÃO
   ========================== */

void explorarSalas(Sala *atual, NodoBST **arvorePistas) {
    char opcao;
    /* declarações no início (C89) */
    while (1) {
        printf("\nVoce entrou em: %s\n", atual->nome);

        if (atual->pista[0] != '\0') {
            printf("Voce encontrou uma pista: %s\n", atual->pista);
            *arvorePistas = inserirPista(*arvorePistas, atual->pista);
        }

        if (atual->esq == NULL && atual->dir == NULL) {
            printf("\nFim do caminho! Cômodo sem saídas.\n");
            return;
        }

        printf("\nPara onde deseja ir?\n");
        if (atual->esq != NULL) printf(" (e) Esquerda -> %s\n", atual->esq->nome);
        if (atual->dir != NULL) printf(" (d) Direita  -> %s\n", atual->dir->nome);
        printf(" (s) Sair da exploracao\n");
        printf("Escolha: ");

        /* ler primeiro caractere não branco */
        do {
            if (scanf(" %c", &opcao) != 1) {
                int ch;
                while ((ch = getchar()) != '\n' && ch != EOF) {}
                opcao = '\0';
            }
        } while (opcao == '\n' || opcao == '\r');

        if (opcao == 'e' && atual->esq != NULL) {
            atual = atual->esq;
        } else if (opcao == 'd' && atual->dir != NULL) {
            atual = atual->dir;
        } else if (opcao == 's') {
            return;
        } else {
            printf("Opcao invalida ou caminho inexistente.\n");
        }
    }
}

/* ==========================
             MAIN
   ========================== */

int main() {
    Sala *hall;
    Sala *biblioteca;
    Sala *cozinha;
    Sala *porao;
    Sala *jardim;
    NodoBST *pistas;
    int opcao;

    /* criar salas (strings literais convertidas para char* locais) */
    hall = criarSala("Hall de Entrada", "");
    biblioteca = criarSala("Biblioteca", "Pagina Rasgada");
    cozinha = criarSala("Cozinha", "");
    porao = criarSala("Porao", "Chave Enferrujada");
    jardim = criarSala("Jardim", "Pegadas Misteriosas");

    /* montar arvore */
    hall->esq = biblioteca;
    hall->dir = cozinha;
    biblioteca->esq = porao;
    biblioteca->dir = jardim;
    /* porao, jardim e cozinha sao folhas */

    pistas = NULL;

    do {
        printf("\n========= DETECTIVE QUEST – NÍVEL AVENTUREIRO =========\n");
        printf("1 - Explorar a mansao\n");
        printf("2 - Listar pistas encontradas\n");
        printf("0 - Sair\n");
        printf("Escolha: ");
        if (scanf("%d", &opcao) != 1) {
            int ch;
            while ((ch = getchar()) != '\n' && ch != EOF) {}
            opcao = -1;
        }

        if (opcao == 1) {
            explorarSalas(hall, &pistas);
        } else if (opcao == 2) {
            printf("\n=== Pistas Encontradas (em ordem alfabetica) ===\n");
            if (pistas == NULL) {
                printf("Nenhuma pista encontrada ainda.\n");
            } else {
                listarPistas(pistas);
            }
        } else if (opcao != 0) {
            printf("Opcao invalida!\n");
        }

    } while (opcao != 0);

    /* liberar memoria (opcjonal) */
    /* Nota: para simplicidade nao liberei todos os nodos alocados aqui. */

    return 0;
}


#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MAX 5 // Tamanho fixo da fila de peças futuras

// Estrutura que representa uma peça de Tetris
typedef struct {
    char nome; // 'I', 'O', 'T', 'L'
    int id;    // Identificador numérico único
} Peca;

// Estrutura da Fila Circular
typedef struct {
    Peca itens[MAX];
    int frente;
    int tras;
    int tamanho;
} FilaCircular;

// Contador global para garantir que cada peça tenha um ID único e sequencial
int proximo_id = 0;

// Função que gera automaticamente uma peça aleatória
Peca gerarPeca() {
    Peca novaPeca;
    char tipos[] = {'I', 'O', 'T', 'L'};
    
    // Sorteia um dos 4 tipos de peças
    novaPeca.nome = tipos[rand() % 4];
    novaPeca.id = proximo_id++; // Atribui o ID atual e incrementa para a próxima
    
    return novaPeca;
}

// Inicializa a fila definindo os índices e o tamanho inicial
void inicializarFila(FilaCircular *f) {
    f->frente = 0;
    f->tras = -1;
    f->tamanho = 0;
}

// Verifica se a fila está cheia
int estaCheia(FilaCircular *f) {
    return f->tamanho == MAX;
}

// Verifica se a fila está vazia
int estaVazia(FilaCircular *f) {
    return f->tamanho == 0;
}

// Insere uma nova peça no final da fila (Enqueue)
void enfileirar(FilaCircular *f) {
    if (estaCheia(f)) {
        printf("\n[AVISO] A fila de pecas futuras esta cheia! Jogue uma peca primeiro.\n");
        return;
    }
    
    // Avança o índice 'tras' de forma circular
    f->tras = (f->tras + 1) % MAX;
    
    // Gera a peça automaticamente e insere na fila
    f->itens[f->tras] = gerarPeca();
    f->tamanho++;
}

// Remove a peça da frente da fila para o jogador usar (Dequeue)
void desenfileirar(FilaCircular *f) {
    if (estaVazia(f)) {
        printf("\n[AVISO] Nao ha pecas na fila para jogar!\n");
        return;
    }
    
    Peca pecaJogada = f->itens[f->frente];
    printf("\nPeca [%c %d] jogada com sucesso!\n", pecaJogada.nome, pecaJogada.id);
    
    // Avança o índice 'frente' de forma circular
    f->frente = (f->frente + 1) % MAX;
    f->tamanho--;
}

// Exibe o estado atual da fila de peças
void exibirFila(FilaCircular *f) {
    printf("\nFila de pecas: ");
    if (estaVazia(f)) {
        printf("[Vazia]");
    } else {
        int i = f->frente;
        //Passa por todos elementos que estão atualmente na fila
        for (int c = 0; c < f->tamanho; c++) {
            printf("[%c %d] ", f->itens[i].nome, f->itens[i].id);
            i = (i + 1) % MAX; // Caminha de forma circular pelo vetor
        }
    }
    printf("\n");
}

int main() {
    // Inicializa o gerador de números aleatórios com base no tempo atual
    srand(time(NULL));
    
    FilaCircular filaControle;
    inicializarFila(&filaControle);
    
    // Inicializa a fila com as 5 peças padrão conforme os requisitos
    for (int i = 0; i < MAX; i++) {
        enfileirar(&filaControle);
    }
    
    int opcao = -1;
    
    // Loop principal do Menu do Jogo
    while (opcao != 0) {
        exibirFila(&filaControle);
        
        printf("\nOpcoes de acao:\n");
        printf("Codigo | Acao\n");
        printf("  1    | Jogar peca (dequeue)\n");
        printf("  2    | Inserir nova peca (enqueue)\n");
        printf("  0    | Sair\n");
        printf("Escolha uma opcao: ");
        
        // Validação simples de entrada
        if (scanf("%d", &opcao) != 1) {
            printf("\nEntrada invalida! Digite um numero.\n");
            while (getchar() != '\n'); // Limpa o buffer do teclado
            continue;
        }
        
        switch (opcao) {
            case 1:
                desenfileirar(&filaControle);
                break;
            case 2:
                enfileirar(&filaControle);
                break;
            case 0:
                printf("\nEncerrando o Modulo Tetris Stack. *** Ate mais!*** \n");

                break;

            default:
                printf("\nOpcao invalida! Tente novamente.\n");
        }
        
        printf("-----------------------------------------\n");
    }
    
    return 0;
}
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MAX_FILA 5    // Tamanho fixo da fila de peças futuras
#define MAX_PILHA 3   // Capacidade máxima da pilha de reserva

// Estrutura que representa uma peça de Tetris
typedef struct {
    char nome; // 'I', 'O', 'T', 'L'
    int id;    // Identificador numérico único
} Peca;

// Estrutura da Fila Circular
typedef struct {
    Peca itens[MAX_FILA];
    int frente;
    int tras;
    int tamanho;
} FilaCircular;

// Estrutura da Pilha Linear (Reserva)
typedef struct {
    Peca itens[MAX_PILHA];
    int topo;
} PilhaReserva;

// Contador global para IDs únicos e sequenciais
int proximo_id = 0;

// Função que gera automaticamente uma peça aleatória
Peca gerarPeca() {
    Peca novaPeca;
    char tipos[] = {'I', 'O', 'T', 'L'};
    novaPeca.nome = tipos[rand() % 4];
    novaPeca.id = proximo_id++;
    return novaPeca;
}

// ==========================================
// FUNÇÕES DA FILA CIRCULAR
// ==========================================

void inicializarFila(FilaCircular *f) {
    f->frente = 0;
    f->tras = -1;
    f->tamanho = 0;
}

int filaCheia(FilaCircular *f) {
    return f->tamanho == MAX_FILA;
}

int filaVazia(FilaCircular *f) {
    return f->tamanho == 0;
}

// Insere uma nova peça gerada automaticamente no fim da fila
void enfileirar(FilaCircular *f) {
    if (filaCheia(f)) return;
    
    f->tras = (f->tras + 1) % MAX_FILA;
    f->itens[f->tras] = gerarPeca();
    f->tamanho++;
}

// Remove a peça da frente da fila e a retorna
Peca desenfileirar(FilaCircular *f) {
    Peca pecaRemovida = f->itens[f->frente];
    f->frente = (f->frente + 1) % MAX_FILA;
    f->tamanho--;
    
    // Regra: Sempre que sai uma peça, uma nova entra no fim da fila
    enfileirar(f);
    
    return pecaRemovida;
}

// ==========================================
// FUNÇÕES DA PILHA DE RESERVA
// ==========================================

void inicializarPilha(PilhaReserva *p) {
    p->topo = -1; // Pilha vazia
}

int pilhaCheia(PilhaReserva *p) {
    return p->topo == MAX_PILHA - 1;
}

int pilhaVazia(PilhaReserva *p) {
    return p->topo == -1;
}

// Empilha uma peça na reserva (Push)
int empilhar(PilhaReserva *p, Peca peca) {
    if (pilhaCheia(p)) {
        return 0; // Falha: pilha cheia
    }
    p->topo++;
    p->itens[p->topo] = peca;
    return 1; // Sucesso
}

// Desempilha uma peça da reserva (Pop)
Peca desempilhar(PilhaReserva *p) {
    Peca pecaRemovida = p->itens[p->topo];
    p->topo--;
    return pecaRemovida;
}

// ==========================================
// FUNÇÕES DE EXIBIÇÃO E INTERFACE
// ==========================================

void exibirEstadoAtual(FilaCircular *f, PilhaReserva *p) {
    printf("\nEstado atual:\n");
    
    // 1. Exibição da Fila
    printf("Fila de pecas: ");
    int idx = f->frente;
    for (int c = 0; c < f->tamanho; c++) {
        printf("[%c %d] ", f->itens[idx].nome, f->itens[idx].id);
        idx = (idx + 1) % MAX_FILA;
    }
    printf("\n");
    
    // 2. Exibição da Pilha (Do Topo para a Base)
    printf("Pilha de reserva (Topo -> Base): ");
    if (pilhaVazia(p)) {
        printf("[Vazia]");
    } else {
        for (int i = p->topo; i >= 0; i--) {
            printf("[%c %d] ", p->itens[i].nome, p->itens[i].id);
        }
    }
    printf("\n");
}

// ==========================================
// PROGRAMA PRINCIPAL
// ==========================================

int main() {
    srand(time(NULL));
    
    FilaCircular filaControle;
    PilhaReserva pilhaControle;
    
    inicializarFila(&filaControle);
    inicializarPilha(&pilhaControle);
    
    // Preenche a fila inicial com 5 elementos
    for (int i = 0; i < MAX_FILA; i++) {
        enfileirar(&filaControle);
    }
    
    int opcao = -1;
    
    while (opcao != 0) {
        exibirEstadoAtual(&filaControle, &pilhaControle);
        
        printf("\nOpcoes de Acao:\n");
        printf("Codigo | Acao\n");
        printf("  1    | Jogar peca\n");
        printf("  2    | Reservar peca\n");
        printf("  3    | Usar peca reservada\n");
        printf("  0    | Sair\n");
        printf("Opcao: ");
        
        if (scanf("%d", &opcao) != 1) {
            printf("\nEntrada invalida! Digite um numero.\n");
            while (getchar() != '\n'); 
            continue;
        }
        
        switch (opcao) {
            case 1: {
                // Remove a peça da frente e descarta (simula o uso no tabuleiro)
                Peca jogada = desenfileirar(&filaControle);
                printf("\n[JOGADA] Peca [%c %d] foi para o tabuleiro!\n", jogada.nome, jogada.id);
                break;
            }
            case 2: {
                // Tenta mover da fila para a pilha
                if (pilhaCheia(&pilhaControle)) {
                    printf("\n[AVISO] A pilha de reserva esta cheia! Nao eh possivel reservar.\n");
                } else {
                    Peca paraReserva = desenfileirar(&filaControle);
                    empilhar(&pilhaControle, paraReserva);
                    printf("\n[RESERVA] Peca [%c %d] guardada na reserva.\n", paraReserva.nome, paraReserva.id);
                }
                break;
            }
            case 3: {
                // Tenta remover o topo da pilha
                if (pilhaVazia(&pilhaControle)) {
                    printf("\n[AVISO] Nao ha pecas na reserva para usar!\n");
                } else {
                    Peca deReserva = desempilhar(&pilhaControle);
                    printf("\n[JOGADA] Peca da reserva [%c %d] foi usada!\n", deReserva.nome, deReserva.id);
                }
                break;
            }
            case 0:
                printf("\nEncerrando o modulo Tetris Stack. Ate logo!\n");
                break;
            default:
                printf("\nOpcao invalida! Tente novamente.\n");
        }
        printf("-----------------------------------------\n");
    }
    
    return 0;
}
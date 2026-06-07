#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MAX_FILA 5  // Tamanho fixo da fila de peças futuras
#define MAX_PILHA 3 // Capacidade máxima da pilha de reserva

// Estrutura que representa uma peça de Tetris
typedef struct
{
    char nome; // 'I', 'O', 'T', 'L'
    int id;    // Identificador numérico único
} Peca;

// Estrutura da Fila Circular
typedef struct
{
    Peca itens[MAX_FILA];
    int frente;
    int tras;
    int tamanho;
} FilaCircular;

// Estrutura da Pilha Linear (Reserva)
typedef struct
{
    Peca itens[MAX_PILHA];
    int topo;
} PilhaReserva;

// Contador global para IDs únicos e sequenciais
int proximo_id = 0;

// Função que gera automaticamente uma peça aleatória
Peca gerarPeca()
{
    Peca novaPeca;
    char tipos[] = {'I', 'O', 'T', 'L'};
    novaPeca.nome = tipos[rand() % 4];
    novaPeca.id = proximo_id++;
    return novaPeca;
}

// ==========================================
// FUNÇÕES DA FILA CIRCULAR
// ==========================================

void inicializarFila(FilaCircular *f)
{
    f->frente = 0;
    f->tras = -1;
    f->tamanho = 0;
}

int filaCheia(FilaCircular *f)
{
    return f->tamanho == MAX_FILA;
}

int filaVazia(FilaCircular *f)
{
    return f->tamanho == 0;
}

// Insere uma nova peça gerada automaticamente no fim da fila
void enfileirar(FilaCircular *f)
{
    if (filaCheia(f))
        return;

    f->tras = (f->tras + 1) % MAX_FILA;
    f->itens[f->tras] = gerarPeca();
    f->tamanho++;
}

// Remove a peça da frente da fila e a retorna (fazendo reposição imediata)
Peca desenfileirar(FilaCircular *f)
{
    Peca pecaRemovida = f->itens[f->frente];
    f->frente = (f->frente + 1) % MAX_FILA;
    f->tamanho--;

    // Auto-reposição para manter a fila sempre com 5
    enfileirar(f);

    return pecaRemovida;
}

// ==========================================
// FUNÇÕES DA PILHA DE RESERVA
// ==========================================

void inicializarPilha(PilhaReserva *p)
{
    p->topo = -1;
}

int pCheia(PilhaReserva *p)
{
    return p->topo == MAX_PILHA - 1;
}

int pVazia(PilhaReserva *p)
{
    return p->topo == -1;
}

void empilhar(PilhaReserva *p, Peca peca)
{
    if (pCheia(p))
        return;
    p->topo++;
    p->itens[p->topo] = peca;
}

Peca desempilhar(PilhaReserva *p)
{
    Peca pecaRemovida = p->itens[p->topo];
    p->topo--;
    return pecaRemovida;
}

// ==========================================
// MECÂNICAS AVANÇADAS DE TROCA
// ==========================================

// Opção 4: Substitui a peça da frente da fila com o topo da pilha
void trocarPecaAtual(FilaCircular *f, PilhaReserva *p)
{
    if (filaVazia(f) || pVazia(p))
    {
        printf("\n[AVISO] Operacao impossivel. A fila ou a pilha estao vazias!\n");
        return;
    }

    // Troca direta de valores entre os índices mapeados
    Peca aux = f->itens[f->frente];
    f->itens[f->frente] = p->itens[p->topo];
    p->itens[p->topo] = aux;

    printf("\n[TROCA] Peca da frente da fila trocada com o topo da pilha!\n");
}

// Opção 5: Alterna em bloco os 3 primeiros da fila com os 3 da pilha
void trocaMultipla(FilaCircular *f, PilhaReserva *p)
{
    // Validação: Ambas precisam ter pelo menos 3 peças. Como a fila sempre tem 5, checamos a pilha.
    if (p->topo < 2)
    {
        printf("\n[AVISO] A pilha precisa ter exatamente 3 pecas para realizar a troca multipla!\n");
        return;
    }

    // Vetores temporários para segurar as peças durante a transição
    Peca temporarioFila[3];
    Peca temporarioPilha[3];

    // 1. Coleta os 3 primeiros elementos da Fila Circular sem alterar a estrutura dela ainda
    int idx = f->frente;
    for (int i = 0; i < 3; i++)
    {
        temporarioFila[i] = f->itens[idx];
        idx = (idx + 1) % MAX_FILA;
    }

    // 2. Coleta os elementos da Pilha (do Topo para a Base)
    // Ex: Topo idx 2, depois 1, depois 0
    int t = p->topo;
    for (int i = 0; i < 3; i++)
    {
        temporarioPilha[i] = p->itens[t];
        t--;
    }

    // 3. Aplica os novos valores na Fila Circular (mantendo a ordem original do topo no começo)
    idx = f->frente;
    for (int i = 0; i < 3; i++)
    {
        f->itens[idx] = temporarioPilha[i];
        idx = (idx + 1) % MAX_FILA;
    }

    // 4. Aplica os novos valores na Pilha (invertendo para o comportamento LIFO)
    // O primeiro da fila vira a base da pilha, o terceiro vira o topo.
    p->itens[0] = temporarioFila[0];
    p->itens[1] = temporarioFila[1];
    p->itens[2] = temporarioFila[2];

    printf("\nAcao: troca realizada entre os 3 primeiros da fila e os 3 da pilha.\n");
}

// ==========================================
// EXIBIÇÃO DO ESTADO
// ==========================================

void exibirEstadoAtual(FilaCircular *f, PilhaReserva *p)
{
    printf("\nEstado atual:\n");

    // Exibição da Fila
    printf("Fila de pecas: ");
    int idx = f->frente;
    for (int c = 0; c < f->tamanho; c++)
    {
        printf("[%c %d] ", f->itens[idx].nome, f->itens[idx].id);
        idx = (idx + 1) % MAX_FILA;
    }
    printf("\n");

    // Exibição da Pilha
    printf("Pilha de reserva (Topo -> base): ");
    if (pVazia(p))
    {
        printf("[Vazia]");
    }
    else
    {
        for (int i = p->topo; i >= 0; i--)
        {
            printf("[%c %d] ", p->itens[i].nome, p->itens[i].id);
        }
    }
    printf("\n");
}

// ==========================================
// PROGRAMA PRINCIPAL
// ==========================================

int main()
{
    srand(time(NULL));

    FilaCircular filaControle;
    PilhaReserva pilhaControle;

    inicializarFila(&filaControle);
    inicializarPilha(&pilhaControle);

    // Inicializa a fila cheia (5 peças)
    for (int i = 0; i < MAX_FILA; i++)
    {
        enfileirar(&filaControle);
    }

    int opcao = -1;

    while (opcao != 0)
    {
        exibirEstadoAtual(&filaControle, &pilhaControle);

        printf("\nOpcoes disponiveis:\n");
        printf("Codigo | Acao\n");
        printf("  1    | Jogar peca da frente da fila\n");
        printf("  2    | Enviar peca da fila para a pilha de reserva\n");
        printf("  3    | Usar peca da pilha de reserva\n");
        printf("  4    | Trocar peca da frente da fila com o topo da pilha\n");
        printf("  5    | Trocar os 3 primeiros da fila com as 3 pecas da pilha\n");
        printf("  0    | Sair\n");
        printf("Opcao escolhida: ");

        if (scanf("%d", &opcao) != 1)
        {
            printf("\nEntrada invalida! Digite um numero.\n");
            while (getchar() != '\n')
                ;
            continue;
        }

        switch (opcao)
        {
        case 1:
        {
            Peca jogada = desenfileirar(&filaControle);
            printf("\n[JOGADA] Peca [%c %d] enviada para o tabuleiro!\n", jogada.nome, jogada.id);
            break;
        }
        case 2:
        {
            if (pCheia(&pilhaControle))
            {
                printf("\n[AVISO] Pilha de reserva cheia!\n");
            }
            else
            {
                Peca paraReserva = desenfileirar(&filaControle);
                empilhar(&pilhaControle, paraReserva);
                printf("\n[RESERVA] Peca [%c %d] movida para a reserva.\n", paraReserva.nome, paraReserva.id);
            }
            break;
        }
        case 3:
        {
            if (pVazia(&pilhaControle))
            {
                printf("\n[AVISO] Pilha de reserva vazia!\n");
            }
            else
            {
                Peca deReserva = desempilhar(&pilhaControle);
                printf("\n[USO] Peca da reserva [%c %d] utilizada!\n", deReserva.nome, deReserva.id);
            }
            break;
        }
        case 4:
            trocarPecaAtual(&filaControle, &pilhaControle);
            break;
        case 5:
            trocaMultipla(&filaControle, &pilhaControle);
            break;
        case 0:
            printf("\nEncerrando o Modulo Tetris Stack Avancado. *** Ate a proxima!*** \n");
            break;
        default:
            printf("\nOpcao invalida! Tente novamente.\n");
        }
        printf("----------------------------------------------------------------\n");
    }

    return 0;
}
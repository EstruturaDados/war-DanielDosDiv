#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

/*
    Estrutura Territorio:
    - nome: nome do território (até 29 chars + \0)
    - cor: cor do exército/dono (até 9 chars + \0)
    - tropas: quantidade de tropas no território
*/
typedef struct {
    char nome[30];
    char cor[10];
    int tropas;
} Territorio;

/* ---------- Assinaturas das funções (modularização) ---------- */
Territorio* cadastrarTerritorios(int quantidade);
void exibirMapa(Territorio* mapa, int quantidade);
void atacar(Territorio* atacante, Territorio* defensor);
void liberarMemoria(Territorio* mapa);
void limparBufferEntrada();
void lerString(const char* prompt, char* destino, int tamanho);

/* -------------------- Implementação -------------------- */

int main() {
    srand((unsigned int) time(NULL)); // Garante aleatoriedade nas rolagens de dados

    printf("=== WAR Estruturado (Nível Aventureiro) ===\n");

    int n;
    do {
        printf("Digite o número de territórios (mínimo 2): ");
        if (scanf("%d", &n) != 1) {
            // leitura inválida: limpa buffer e pede novamente
            limparBufferEntrada();
            n = 0;
            printf("Entrada inválida. Tente novamente.\n");
            continue;
        }
        limparBufferEntrada();
        if (n < 2) {
            printf("É necessário pelo menos 2 territórios para brincar. Tente novamente.\n");
        }
    } while (n < 2);

    // Alocação dinâmica com calloc (inicializa com zeros)
    Territorio* mapa = cadastrarTerritorios(n);
    if (mapa == NULL) {
        fprintf(stderr, "Erro ao alocar memória. Encerrando.\n");
        return 1;
    }

    // Exibe mapa inicial
    exibirMapa(mapa, n);

    // Loop principal para executar ataques
    while (1) {
        printf("\n--- Menu ---\n");
        printf("1. Realizar ataque\n");
        printf("2. Exibir mapa\n");
        printf("3. Sair\n");
        printf("Escolha uma opção: ");

        int opc;
        if (scanf("%d", &opc) != 1) {
            limparBufferEntrada();
            printf("Opção inválida. Tente novamente.\n");
            continue;
        }
        limparBufferEntrada();

        if (opc == 1) {
            exibirMapa(mapa, n);
            int idxAtac, idxDef;
            printf("\nSelecione o índice do território ATACANTE (0 a %d): ", n - 1);
            if (scanf("%d", &idxAtac) != 1) { limparBufferEntrada(); printf("Entrada inválida.\n"); continue; }
            limparBufferEntrada();
            printf("Selecione o índice do território DEFENSOR (0 a %d): ", n - 1);
            if (scanf("%d", &idxDef) != 1) { limparBufferEntrada(); printf("Entrada inválida.\n"); continue; }
            limparBufferEntrada();

            // Valida índices
            if (idxAtac < 0 || idxAtac >= n || idxDef < 0 || idxDef >= n) {
                printf("Índices fora do intervalo. Tente novamente.\n");
                continue;
            }
            if (idxAtac == idxDef) {
                printf("Um território não pode atacar a si mesmo. Tente novamente.\n");
                continue;
            }

            Territorio* atacante = &mapa[idxAtac];
            Territorio* defensor = &mapa[idxDef];

            // valida que não atacar próprio exército
            if (strcmp(atacante->cor, defensor->cor) == 0) {
                printf("Ataque inválido: ambos os territórios pertencem à mesma cor ('%s').\n", atacante->cor);
                continue;
            }
            // valida tropas mínimas
            if (atacante->tropas <= 0) {
                printf("Ataque impossível: território atacante '%s' não tem tropas.\n", atacante->nome);
                continue;
            }

            // Realiza ataque (a função modifica as estruturas via ponteiro)
            atacar(atacante, defensor);

            // Exibe resultado atualizado
            printf("\nResultado do ataque:\n");
            exibirMapa(mapa, n);
        }
        else if (opc == 2) {
            exibirMapa(mapa, n);
        }
        else if (opc == 3) {
            printf("Encerrando o jogo. Liberando memória...\n");
            break;
        }
        else {
            printf("Opção inválida. Tente novamente.\n");
        }
    }

    liberarMemoria(mapa);
    return 0;
}

/* Lê e cadastra 'quantidade' territórios, retorna ponteiro para vetor alocado */
Territorio* cadastrarTerritorios(int quantidade) {
    // Aloca dinamicamente o vetor de Territorio
    Territorio* mapa = (Territorio*) calloc(quantidade, sizeof(Territorio));
    if (mapa == NULL) return NULL;

    for (int i = 0; i < quantidade; i++) {
        printf("\n--- Cadastro do Território %d ---\n", i);
        lerString("Nome do território: ", mapa[i].nome, sizeof(mapa[i].nome));
        lerString("Cor do exército: ", mapa[i].cor, sizeof(mapa[i].cor));

        // Entrada de tropas usando scanf (validação simples)
        int t;
        do {
            printf("Quantidade de tropas (>= 0): ");
            if (scanf("%d", &t) != 1) {
                limparBufferEntrada();
                t = -1;
                printf("Entrada inválida. Tente novamente.\n");
                continue;
            }
            limparBufferEntrada();
            if (t < 0) {
                printf("Tropas não podem ser negativas. Tente novamente.\n");
            }
        } while (t < 0);
        mapa[i].tropas = t;
    }

    return mapa;
}

/* Exibe todos os territórios em formato claro e legível */
void exibirMapa(Territorio* mapa, int quantidade) {
    printf("\n=== Mapa de Territórios ===\n");
    for (int i = 0; i < quantidade; i++) {
        printf("[%d] Nome: %s | Cor: %s | Tropas: %d\n",
               i, mapa[i].nome, mapa[i].cor, mapa[i].tropas);
    }
}


void atacar(Territorio* atacante, Territorio* defensor) {
    if (atacante == NULL || defensor == NULL) return;

    int rollAtac = (rand() % 6) + 1; // 1..6
    int rollDef  = (rand() % 6) + 1; // 1..6

    printf("\nRolagem de dados: %s (Atacante) -> %d x %d <- %s (Defensor)\n",
           atacante->nome, rollAtac, rollDef, defensor->nome);

    if (rollAtac > rollDef) {
        // Atacante vence: transfere cor e metade das tropas
        int transferencia = atacante->tropas / 2; // metade (inteiro)
        if (transferencia <= 0) {
            // Se atacante tem apenas 1 tropa, transferência resulta em 0;
            // decidimos que defensor fica com 0 tropas mas muda de cor
            transferencia = 0;
        }

        // Atualiza defensor via ponteiro
        strncpy(defensor->cor, atacante->cor, sizeof(defensor->cor) - 1);
        defensor->cor[sizeof(defensor->cor) - 1] = '\0'; // garante terminação

        defensor->tropas = transferencia;

        // Atualiza atacante
        atacante->tropas -= transferencia;
        if (atacante->tropas < 0) atacante->tropas = 0;

        printf("Atacante venceu! '%s' conquista '%s'.\n", atacante->nome, defensor->nome);
        printf("Foram transferidas %d tropas para o defensor.\n", transferencia);
    } else {
        // Atacante perde (ou empata): perde 1 tropa (mas não fica negativa)
        if (atacante->tropas > 0) {
            atacante->tropas -= 1;
        }
        printf("Atacante perdeu ou houve empate. '%s' perde 1 tropa.\n", atacante->nome);
    }
}

/* Libera a memória alocada pelo mapa de territórios */
void liberarMemoria(Territorio* mapa) {
    if (mapa != NULL) {
        free(mapa);
    }
}

/* Função utilitária para limpar caracteres remanescentes no buffer de entrada */
void limparBufferEntrada() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF) { }
}

/* Lê uma linha de texto com fgets e remove newline; usa prompt para o usuário */
void lerString(const char* prompt, char* destino, int tamanho) {
    printf("%s", prompt);
    if (fgets(destino, tamanho, stdin) == NULL) {
        // Em caso de falha, garante string vazia
        destino[0] = '\0';
        return;
    }
    // Remove newline final, se presente
    size_t len = strlen(destino);
    if (len > 0 && destino[len - 1] == '\n') destino[len - 1] = '\0';
}

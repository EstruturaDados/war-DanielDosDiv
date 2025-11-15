#include <stdio.h>
#include <string.h>


typedef struct {
    char nome[30];
    char cor[10];
    int tropas;
} Territorio;

int main() {
    // Vetor de 5 territórios
    Territorio territorios[5];

    printf("=== Cadastro de Territórios ===\n");

    // Entrada de dados com laço for
    for (int i = 0; i < 5; i++) {
        printf("\n--- Território %d ---\n", i + 1);

        printf("Digite o nome do território: ");
        scanf("%s", territorios[i].nome);  // Lê string sem espaços

        printf("Digite a cor do exército: ");
        scanf("%s", territorios[i].cor);

        printf("Digite a quantidade de tropas: ");
        scanf("%d", &territorios[i].tropas);
    }

    // Exibição dos dados
    printf("\n\n=== Territórios Registrados ===\n");

    for (int i = 0; i < 5; i++) {
        printf("\nTerritório %d:\n", i + 1);
        printf("Nome: %s\n", territorios[i].nome);
        printf("Cor do Exército: %s\n", territorios[i].cor);
        printf("Tropas: %d\n", territorios[i].tropas);
    }

    return 0;
}

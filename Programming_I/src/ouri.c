#include<stdio.h>
#include<stdlib.h>
#include"function.h"
int main(int argc, char *argv[]) {
    int player[2][6] = {{4, 4, 4, 4, 4, 4}, {4, 4, 4, 4, 4, 4}}; // Initial setup for the stones in houses
    int deposito[2] = {0, 0}; // Initial setup for the deposit (store) of each player
    if (argc > 1) {
        load_game_state(argv[1], player, deposito);
    }
    printf("Jogo do Ouri\n");
    printf("Bem vindo ao Jogo do Ouri!\n");
    printf("Escolha um oponente:\n");
    printf("1. Jogue contra outro player\n");
    printf("2. Jogue contra a CPU\n");
    while (1)
    {
    int opponentChoice;
    printf("Introduza a sua escolha (1 ou 2): ");
    scanf("%d", &opponentChoice);
    if (opponentChoice == 1) {
        // Play against another human
        playGame(player, deposito);
        break;
    } else if (opponentChoice == 2) {
        // Play against the CPU
        playAgainstCPU(player, deposito);
        break;
    } else {
        printf("Escolha inválida. Por favor insira 1 ou 2.\n");
        while (getchar() != '\n'); // Clear input buffer
    }

    }
    return 0;
}
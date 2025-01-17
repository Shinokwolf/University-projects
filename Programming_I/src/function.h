// Protótipos das funções
int getInput();
int house_rules(int player[2][6], int selectedHouse, int currentPlayer);
void Board(int player[2][6], int deposit[2]);
void movement(int player[2][6], int deposito[2], int selectedHouse, int currentPlayer);
int findHouseWithMaxStones(int player[2][6], int currentPlayer);
void save_game_state(int player[2][6], int deposit[2]);
void load_game_state(char *filename, int player[2][6], int deposit[2]);
void playGame(int player[2][6], int deposito[2]);
void playAgainstCPU(int player[2][6], int deposito[2]); 
// Função para exibir o tabuleiro
void Board(int player[2][6], int deposit[2]) {
    printf("|---|--|--|--|--|--|--|---|\n");
    printf("|   |%2d|%2d|%2d|%2d|%2d|%2d|   |\n", player[1][5], player[1][4], player[1][3], player[1][2], player[1][1], player[1][0]);
    printf("| %2d|-----------------| %2d|\n", deposit[0], deposit[1]);
    printf("|   |%2d|%2d|%2d|%2d|%2d|%2d|   |\n", player[0][0], player[0][1], player[0][2], player[0][3], player[0][4], player[0][5]);
    printf("|---|--|--|--|--|--|--|---|\n");
}
// Função para movimentar as pedras
void movement(int player[2][6], int deposito[2], int selectedHouse, int currentPlayer) {
    int currentHouse = selectedHouse;
    int stones = player[currentPlayer][currentHouse];
    player[currentPlayer][currentHouse] = 0;
    int initialPlayer = currentPlayer;
    int initialHouse = currentHouse;
    int Verificador_Player = currentPlayer;

    while (stones > 0) {
        if ((currentHouse == 5) && stones > 0) {
            currentHouse = 0;
            currentPlayer = 1 - currentPlayer;
            player[currentPlayer][currentHouse]++;
            stones--;
        } else {
            currentHouse = (currentHouse + 1) % 6;
            player[currentPlayer][currentHouse]++;
            --stones;
        }

        if ((currentPlayer == initialPlayer) && (currentHouse == initialHouse)) {
            player[initialPlayer][initialHouse] = 0;
            ++stones;
        }
    }

    if (Verificador_Player != currentPlayer && (player[currentPlayer][(currentHouse)] == 2 || player[currentPlayer][(currentHouse)] == 3)) {
        if (currentHouse == 0) {
            int mine_2 = player[currentPlayer][currentHouse];
            deposito[1 - currentPlayer] += mine_2;
            player[currentPlayer][currentHouse] = 0;
        } else {
            int mine = player[currentPlayer][currentHouse];
            deposito[1 - currentPlayer] += mine;
            player[currentPlayer][currentHouse] = 0;

            currentHouse = currentHouse - 1;
            while (player[currentPlayer][(currentHouse)] == 2 || player[currentPlayer][(currentHouse)] == 3) {
                mine = player[currentPlayer][(currentHouse)];
                deposito[1 - currentPlayer] += mine;
                player[currentPlayer][(currentHouse)] = 0;
                --currentHouse;
            }
        }
    }

    int verify = 0;
    int current_atual = currentPlayer;

    while (1) {
        for (int i = 0; i < 6; i++) {
            if (player[currentPlayer][i] == 0) {
                verify++;
            }
        }

        if (verify == 6) {
            currentPlayer = 1 - currentPlayer;
            Board(player, deposito);
            printf("Reponha as pedras no lado adversário\n");
            int x, z;
            x = getInput();
            z = house_rules(player, x, currentPlayer);
            movement(player, deposito, z, currentPlayer);
        } else if (current_atual != currentPlayer) {
            break;
        } else {
            verify = 0;
            currentPlayer = 1 - currentPlayer;
        }
    }
}
// Função para encontrar a casa com o máximo de pedras para a jogada da CPU
int findHouseWithMaxStones(int player[2][6], int currentPlayer) {
    int maxStones = 0;
    int houseIndex = -1;

    for (int i = 0; i < 6; ++i) {
        if (player[currentPlayer][i] > maxStones) {
            maxStones = player[currentPlayer][i];
            houseIndex = i;
        }
    }

    return houseIndex;
}

// Função para a estratégia de jogada da CPU
int getCPUMove(int player[2][6], int deposito[2], int currentPlayer) {
    int cpuSelectedHouse = findHouseWithMaxStones(player, currentPlayer);

    // Garante que a casa selecionada é válida e não está vazia
    while (cpuSelectedHouse == -1 || player[currentPlayer][cpuSelectedHouse] == 0) {
        // Se a casa escolhida for inválida ou vazia, tenta a próxima casa com menos pedras
        cpuSelectedHouse = findHouseWithMaxStones(player, currentPlayer);
    }

    return cpuSelectedHouse;
}

// Função para obter a entrada do jogador
int getInput() {
    int user_input;
    while (1) {
        printf("Introduza a posição (entre 1 e 6): \n");
        if (scanf("%d", &user_input) != 1) {
            printf("Entrada inválida. Por favor, introduza um número.\n");
            while (getchar() != '\n');
        } else if (user_input >= 1 && user_input <= 6) {
            return user_input - 1;
        } else {
            printf("Input inválido: introduza valores entre 1 e 6.\n");
        }
    }
}
// Função para aplicar as regras da casa
int house_rules(int player[2][6], int selectedHouse, int currentPlayer) {
    int pedras = player[currentPlayer][selectedHouse];
    int count = 0;

    for (int i = 0; i < 6; i++) {
        int stones = player[currentPlayer][i];
        if (stones > pedras) {
            count++;
        }
    }

    if ((pedras == 1 && count >= 1) || (pedras == 0)) {
        printf("Por favor selecione uma casa que tenha mais de uma pedra.\n");
        selectedHouse = getInput();
        return house_rules(player, selectedHouse, currentPlayer);
    } else {
        return selectedHouse;
    }
}
// Função para salvar o estado do jogo em um file
void save_game_state(int player[2][6], int deposit[2]) {
    char filename[100];
    printf("Digite o nome do arquivo: ");
    scanf("%s", filename);

    FILE *file = fopen(filename, "w");
    if (file == NULL) {
        printf("Não foi possível abrir o arquivo.\n");
        return;
    }

    for (int i = 0; i < 2; i++) {
        fprintf(file, "%d\n", deposit[i]);
        for (int j = 0; j < 6; j++) {
            fprintf(file, "%d ", player[i][j]);
        }
        fprintf(file, "\n");
    }

    fclose(file);
    printf("Estado do jogo salvo com sucesso no arquivo %s.\n", filename);
    exit(0);
}
// Função para carregar o estado do jogo de um file
void load_game_state(char *filename, int player[2][6], int deposit[2]) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        printf("Não foi possível abrir o arquivo.\n");
        return;
    }
    for (int i = 0; i < 2; i++) {
        fscanf(file, "%d", &deposit[i]);
        for (int j = 0; j < 6; j++) {
            fscanf(file, "%d ", &player[i][j]);
        }
    }


    fclose(file);
}
// Função playgame vs outro player
void playGame(int player[2][6], int deposito[2]) {
    int currentPlayer = 0;

    printf("Jogo do Ouri\n");

    while (1) {
        Board(player, deposito);

        printf("Player %d turn:\n", currentPlayer + 1);
        int selectedHouse = getInput();
        selectedHouse = house_rules(player, selectedHouse, currentPlayer);
        movement(player, deposito, selectedHouse, currentPlayer);

        int count = 0;
        for (int i = 0; i < 6; i++) {
            if (player[currentPlayer][i] == 0) {
                count++;
            }
        }

        if (count == 6) {
            currentPlayer = 1 - currentPlayer;
            printf("Reponha as pedras no lado adversário.\n");
            int x = getInput();
            x = house_rules(player, x, currentPlayer);
            movement(player, deposito, x, currentPlayer);
        }

        int victory = 0;
        for (int i = 0; i < 2; i++) {
            if (deposito[i] >= 25) {
                printf("O jogador %d venceu com %d\n", i + 1, deposito[i]);
                victory = 1;
                break;
            }
        }
        if (victory) {
            break;
        }

        currentPlayer = 1 - currentPlayer;
    }
}
// Função para jogar contra o CPU
void playAgainstCPU(int player[2][6], int deposito[2]) {
    int currentPlayer = 0;
    int isPlayerOneHuman = 1; // Uma Flag para indicar se o player 1 é humano ou CPU

    printf("Game against CPU\n");

    while (1) {
        Board(player, deposito);

        printf("Player %d turn:\n", currentPlayer + 1);

        if ((currentPlayer == 0 && isPlayerOneHuman) || (currentPlayer == 1 && !isPlayerOneHuman)) {
            int selectedHouse = getInput();
            selectedHouse = house_rules(player, selectedHouse, currentPlayer);
            movement(player, deposito, selectedHouse, currentPlayer);
        } else {
            // Vez dO CPU (Player 2)
            int cpuSelectedHouse = getCPUMove(player, deposito, currentPlayer);
            movement(player, deposito, cpuSelectedHouse, currentPlayer);
        }

       
        int count = 0;
        for (int i = 0; i < 6; i++) {
            if (player[currentPlayer][i] == 0) {
                count++;
            }
        }

        if (count == 6) {
            currentPlayer = 1 - currentPlayer;
            printf("Reponha as pedras no lado adversário.\n");
            int x = getInput();
            x = house_rules(player, x, currentPlayer);
            movement(player, deposito, x, currentPlayer);
        }

        int victory = 0;
        for (int i = 0; i < 2; i++) {
            if (deposito[0] >= 25) {
                printf("O jogador %d venceu com %d\n", i + 1, deposito[0]);
                victory = 1;
                break;
            }
              if (deposito[1] >= 25) {
                printf("O computador %d venceu com %d\n", i + 1, deposito[1]);
                victory = 1;
                break;
            }
        }
        if (victory) {
            break;
        }

        currentPlayer = 1 - currentPlayer;
    }
}
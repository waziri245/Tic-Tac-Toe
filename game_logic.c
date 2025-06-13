#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

char board[3][3];
char user_symbol;
char bot_symbol;
int current_player;

int check_winner(int row, int column);
int check_draw(void);


void init_game(void) {
    srand(time(NULL));

    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            board[i][j] = ' ';
        }
    }

    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            printf("%c", board[i][j]);
            if (j < 2) {
                printf("|"); // Print a separator between columns
            }
        }
        printf("\n");
        if (i < 2) {
            printf("-----\n"); // Print a separator between rows
        }
    }

    int random_symbol = rand() % 2;  // 0 = O, 1 = X
    
    if (random_symbol == 0) {
        user_symbol = 'O';
        bot_symbol = 'X';

        printf("%c User\n", user_symbol);
        printf("%c Bot\n", bot_symbol);
    }

    else {
        user_symbol = 'X';
        bot_symbol = 'O';
        printf("%c User\n", user_symbol);
        printf("%c Bot\n", bot_symbol);
    }

    int random_player = rand() % 2; // 0 = user, 1 = Bot

    if (random_player == 0) {
        current_player = 0;
        printf("User %d\n", current_player);
    }

    else {
        current_player = 1;
        printf("Bot %d\n", current_player);
    }

}

int make_move(int row, int column) {
    if (board[row][column] != ' ') {
        return -1;
    }
    else {
        if (current_player == 0) {
                board[row][column] = user_symbol;
        }
        else {
                board[row][column] = bot_symbol;  
        }

        if (check_winner(row, column)) {
            return 1;
        }
        if (check_draw()) {
            return 2;
        }
        // switch player
        current_player = 1 - current_player;
        return 0;
    }
}

int check_winner(int row, int column) {
    char symbol = (current_player == 0) ? user_symbol : bot_symbol;

    // Check row
    int match = 0;
    for (int i = 0; i < 3; i++) {
        if (board[row][i] == symbol) {
            match++;
        }
    }
    if (match == 3) return 1;

    // Check column
    match = 0;
    for (int i = 0; i < 3; i++) {
        if (board[i][column] == symbol) {
            match++;
        }
    }
    if (match == 3) return 1;

    // Check main diagonal
    if (row == column) {
        match = 0;
        for (int i = 0; i < 3; i++) {
            if (board[i][i] == symbol) {
                match++;
            }
        }
        if (match == 3) return 1;
    }

    // Check anti-diagonal
    if (row + column == 2) {
        match = 0;
        for (int i = 0; i < 3; i++) {
            if (board[i][2 - i] == symbol) {
                match++;
            }
        }
        if (match == 3) return 1;
    }

    // No win found
    return 0;
}

int check_draw() {
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            if (board[i][j] == ' ') {
                return 0;  // Still empty spots → not a draw
            }
        }
    }
    return 1;  // No empty spots → draw
}


int main(void) {
    init_game();
    return 0;
}


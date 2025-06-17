#include <stdio.h>
#include <stdlib.h>
#include <time.h>

char board[3][3];
char user_symbol;
char bot_symbol;
int current_player;
int difficulty_mode;

void hard_mode(int *row, int *column);
void easy_mode(int *row, int *column);
int check_winner(int row, int column);
int check_draw(void);

char get_user_symbol() { return user_symbol; }
char get_bot_symbol() { return bot_symbol; }
int get_current_player() { return current_player; } // 0=user, 1=bot

void init_game(void) {
    srand(time(NULL));

    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            board[i][j] = ' ';
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
    if (current_player == 0) {
        // User's move
        if (board[row][column] != ' ') return -1;
        board[row][column] = user_symbol;
    } else {
        // Bot's move
        if (difficulty_mode == 0) {
            // Random move
            easy_mode(&row, &column);
            board[row][column] = bot_symbol;
        } else {
            // Minimax
            hard_mode(&row, &column);
            board[row][column] = bot_symbol;
        }
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

void set_difficulty(int mode){
    difficulty_mode = mode;
}

void easy_mode(int *row, int *column) {
    int empty[9][2];
    int count = 0;  

    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            if (board[i][j] == ' ') {
                empty[count][0] = i;
                empty[count][1] = j;
                count ++;
            }
        }
    }

    int r = rand() % count;
    *row = empty[r][0];
    *column = empty[r][1];
}

void hard_mode(int *row, int *column) {

}

int main() {
    srand(time(NULL));  // seed randomness once
    // Pretend some cells are filled
    board[0][0] = 'X';
    board[1][1] = 'O';

    int row, col;
    easy_mode(&row, &col);
    printf("Bot picks: (%d, %d)\n", row, col);

    return 0;
}

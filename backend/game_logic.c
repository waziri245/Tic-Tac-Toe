#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// === GLOBAL GAME STATE === //
char board[3][3];
char user_symbol;
char bot_symbol;
int current_player;
int difficulty_mode;

// Store last bot move for Python to know which button to update
int last_bot_row;
int last_bot_col;

// === FUNCTION DECLARATIONS === //
void hard_mode(int *row, int *column);
void easy_mode(int *row, int *column);
int check_winner(int row, int column);
int check_draw(void);
int has_winner(char symbol);
int minimax(int is_maximizing);

// === GETTERS FOR PYTHON === //
char get_user_symbol() { return user_symbol; }
char get_bot_symbol() { return bot_symbol; }
int get_current_player() { return current_player; }
int get_last_bot_row() { return last_bot_row; }
int get_last_bot_col() { return last_bot_col; }

// === INITIALIZE GAME === //
void init_game(void) {
    srand(time(NULL));

    // Clear the board
    for (int i = 0; i < 3; i++)
        for (int j = 0; j < 3; j++)
            board[i][j] = ' ';

    // Randomly assign X/O
    int random_symbol = rand() % 2;  // 0 = O, 1 = X
    if (random_symbol == 0) {
        user_symbol = 'O';
        bot_symbol = 'X';
    } else {
        user_symbol = 'X';
        bot_symbol = 'O';
    }

    // Randomly decide who starts
    current_player = rand() % 2;

    // Print info for debug
    printf("User: %c | Bot: %c | Starts: %s\n",
           user_symbol, bot_symbol, (current_player == 0) ? "User" : "Bot");
}

// === MAKE A MOVE === //
int make_move(int row, int column) {
    if (current_player == 0) {
        // User's turn: must be valid
        if (board[row][column] != ' ')
            return -1;
        board[row][column] = user_symbol;
    } else {
        // Bot's turn: row/col chosen by AI
        if (difficulty_mode == 0) {
            easy_mode(&row, &column);
        } else {
            hard_mode(&row, &column);
        }
        board[row][column] = bot_symbol;

        // Store where the bot moved (for Python to read!)
        last_bot_row = row;
        last_bot_col = column;
    }

    // Check win/draw for the actual move
    if (check_winner(row, column)) return 1;
    if (check_draw()) return 2;

    // Switch to other player
    current_player = 1 - current_player;
    return 0; // game continues
}

// === CHECK WIN CONDITION === //
int check_winner(int row, int column) {
    char symbol = (current_player == 0) ? user_symbol : bot_symbol;

    // Row
    int match = 0;
    for (int i = 0; i < 3; i++)
        if (board[row][i] == symbol) match++;
    if (match == 3) return 1;

    // Column
    match = 0;
    for (int i = 0; i < 3; i++)
        if (board[i][column] == symbol) match++;
    if (match == 3) return 1;

    // Main diagonal
    if (row == column) {
        match = 0;
        for (int i = 0; i < 3; i++)
            if (board[i][i] == symbol) match++;
        if (match == 3) return 1;
    }

    // Anti-diagonal
    if (row + column == 2) {
        match = 0;
        for (int i = 0; i < 3; i++)
            if (board[i][2 - i] == symbol) match++;
        if (match == 3) return 1;
    }

    return 0; // no win
}

// === CHECK FOR DRAW === //
int check_draw() {
    for (int i = 0; i < 3; i++)
        for (int j = 0; j < 3; j++)
            if (board[i][j] == ' ')
                return 0;
    return 1; // no empty spots
}

// === SET DIFFICULTY === //
void set_difficulty(int mode) {
    difficulty_mode = mode;
}

// === EASY MODE === //
void easy_mode(int *row, int *column) {
    int empty[9][2];
    int count = 0;
    for (int i = 0; i < 3; i++)
        for (int j = 0; j < 3; j++)
            if (board[i][j] == ' ') {
                empty[count][0] = i;
                empty[count][1] = j;
                count++;
            }
    int r = rand() % count;
    *row = empty[r][0];
    *column = empty[r][1];
}

// === HARD MODE (MINIMAX) === //
void hard_mode(int *row, int *column) {
    int best_score = -999;
    int best_row = -1;
    int best_column = -1;

    for (int i = 0; i < 3; i++)
        for (int j = 0; j < 3; j++)
            if (board[i][j] == ' ') {
                board[i][j] = bot_symbol;
                int score = minimax(0);
                board[i][j] = ' ';
                if (score > best_score) {
                    best_score = score;
                    best_row = i;
                    best_column = j;
                }
            }
    *row = best_row;
    *column = best_column;
}

// === MINIMAX ALGORITHM === //
int minimax(int is_maximizing) {
    if (has_winner(bot_symbol)) return +1;
    if (has_winner(user_symbol)) return -1;
    if (check_draw()) return 0;

    if (is_maximizing) {
        int best = -999;
        for (int i = 0; i < 3; i++)
            for (int j = 0; j < 3; j++)
                if (board[i][j] == ' ') {
                    board[i][j] = bot_symbol;
                    int score = minimax(0);
                    board[i][j] = ' ';
                    if (score > best) best = score;
                }
        return best;
    } else {
        int best = 999;
        for (int i = 0; i < 3; i++)
            for (int j = 0; j < 3; j++)
                if (board[i][j] == ' ') {
                    board[i][j] = user_symbol;
                    int score = minimax(1);
                    board[i][j] = ' ';
                    if (score < best) best = score;
                }
        return best;
    }
}

// === SIMPLE WIN CHECK === //
int has_winner(char symbol) {
    // Rows & columns
    for (int i = 0; i < 3; i++) {
        int match_row = 0, match_col = 0;
        for (int j = 0; j < 3; j++) {
            if (board[i][j] == symbol) match_row++;
            if (board[j][i] == symbol) match_col++;
        }
        if (match_row == 3 || match_col == 3) return 1;
    }
    // Diagonals
    int match = 0;
    for (int i = 0; i < 3; i++) if (board[i][i] == symbol) match++;
    if (match == 3) return 1;
    match = 0;
    for (int i = 0; i < 3; i++) if (board[i][2 - i] == symbol) match++;
    if (match == 3) return 1;
    return 0;
}

// === MAIN TEST DRIVER === //
int main() {
    
    return 0;
}

from tkinter import *
from tkinter import messagebox, filedialog
import random
from ctypes import CDLL, c_int

lib = CDLL('./libgame_logic.so')

root = Tk()
root.geometry("700x450")
root.title("Tic-Tac-Toe")
root.resizable(False, False)

difficulty_mode = None
user_symbol = None
bot_symbol = None
current_player = None
buttons = []


def gui():
    play_btn = Button(root, text="Play", font=("Segoe UI", 14, "bold"), width=25, height=2, command=clear_window)
    play_btn.grid()

def clear_window():
    for widget in root.winfo_children():
        widget.destroy()
    easy_btn = Button(root, text="Easy Mode", font=("Segoe UI", 14, "bold"), width=25, height=2, command=lambda: difficulty_level(0)).grid()
    hard_btn = Button(root, text="Hard Mode", font=("Segoe UI", 14, "bold"), width=25, height=2, command=lambda: difficulty_level(1)).grid()

def difficulty_level(mode):
    global difficulty_mode
    difficulty_mode = mode
    lib.set_difficulty(mode)  # C must have this too
    start_game()

def start_game():
    lib.init_game()
    global user_symbol, bot_symbol, current_player
    user_symbol = lib.get_user_symbol()
    bot_symbol = lib.get_bot_symbol()
    current_player = lib.get_current_player()
    draw_board()
    if current_player == 1:
        bot_turn()



def draw_board():
    global buttons
    buttons = []
    for i in range(3):
        row = []
        for j in range(3):
            btn = Button(root, width=5, height=2, font=("Segoe UI", 20, "bold"),
                         command=lambda x=i, y=j: on_button_click(x, y))
            btn.grid(row=i, column=j)
            row.append(btn)
        buttons.append(row)
    messagebox.showinfo(
    "Game Start",
    f"You are '{chr(user_symbol)}'\nBot is '{chr(bot_symbol)}'\n"
    + ("You start!" if current_player == 0 else "Bot starts!")
    )
    if current_player == 1:
        bot_turn()


def on_button_click(x, y):
    global current_player, user_symbol, bot_symbol, buttons

    if current_player != 0:
        return  # ignore clicks when it's not user's turn

    result = lib.make_move(x, y)
    if result == -1:
        messagebox.showwarning("Invalid", "This cell is already taken!")
        return

    buttons[x][y].config(text=chr(user_symbol))

    if result == 1:
        messagebox.showinfo("Game Over", "You win!")
    elif result == 2:
        messagebox.showinfo("Game Over", "It's a Draw!")
    else:
        current_player = 1  # switch to bot
        bot_turn()

                    

def bot_turn():
    global current_player, bot_symbol, buttons

    # Choose move (for now random, later AI)
    empty_cells = [(i, j) for i in range(3) for j in range(3) if buttons[i][j]['text'] == '']
    x, y = random.choice(empty_cells)
    result = lib.make_move(x, y)
    buttons[x][y].config(text=chr(bot_symbol))

    if result == 1:
        messagebox.showinfo("Game Over", "Bot wins!")
    elif result == 2:
        messagebox.showinfo("Game Over", "It's a Draw!")
    else:
        current_player = 0  # switch back to user

if __name__ == "__main__":
    gui()
    root.mainloop()
    
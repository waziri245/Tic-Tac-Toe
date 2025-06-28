from tkinter import *
from tkinter import messagebox
from ctypes import CDLL, c_int
import os
import platform

# Load the compiled C logic library
current_dir = os.path.dirname(os.path.abspath(__file__))
if platform.system() == "Windows":
    lib_path = os.path.join(current_dir, "..", "shared", "libgame_logic.dll")
else:
    lib_path = os.path.join(current_dir, "..", "shared", "libgame_logic.so")

lib = CDLL(lib_path)
lib.get_last_bot_row.restype = c_int
lib.get_last_bot_col.restype = c_int

# Initialize main window
root = Tk()
root.geometry("700x450")
root.title("Tic-Tac-Toe")
root.resizable(False, False)

# Game state
difficulty_mode = None
user_symbol = None
bot_symbol = None
current_player = None
buttons = []
board_frame = None

# 🎨 Dark mode colors & fonts
BG_COLOR = "#121212"   # dark background
BTN_COLOR = "#1F1B24"  # card color
BTN_TEXT_COLOR = "#E0E0E0"  # light text
ACCENT_COLOR = "#BB86FC"  # purple accent for highlights
USER_MOVE_COLOR = "#03DAC6"  # teal for user
BOT_MOVE_COLOR = "#CF6679"  # pink/red for bot
FONT_BIG = ("Segoe UI", 18, "bold")
FONT_BTN = ("Segoe UI", 26, "bold")

root.configure(bg=BG_COLOR)


def gui():
    """Show main menu"""
    clear_window()

    Label(root,
          text="Tic-Tac-Toe",
          font=("Segoe UI", 30, "bold"),
          fg=ACCENT_COLOR,
          bg=BG_COLOR).pack(pady=40)

    Button(root,
           text="Play",
           font=FONT_BIG,
           width=20,
           height=2,
           bg=BTN_COLOR,
           fg=BTN_TEXT_COLOR,
           activebackground=ACCENT_COLOR,
           activeforeground="black",
           bd=0,
           highlightthickness=0,
           command=show_difficulty).pack(pady=20)


def clear_window():
    """Remove all widgets"""
    for widget in root.winfo_children():
        widget.destroy()


def show_difficulty():
    """Show difficulty options"""
    clear_window()

    Label(root,
          text="Select Difficulty",
          font=("Segoe UI", 24, "bold"),
          fg=ACCENT_COLOR,
          bg=BG_COLOR).pack(pady=40)

    Button(root,
           text="Easy Mode",
           font=FONT_BIG,
           width=20,
           height=2,
           bg="#3700B3",
           fg="white",
           activebackground="#6200EE",
           activeforeground="white",
           bd=0,
           highlightthickness=0,
           command=lambda: difficulty_level(0)).pack(pady=20)

    Button(root,
           text="Hard Mode",
           font=FONT_BIG,
           width=20,
           height=2,
           bg="#B00020",
           fg="white",
           activebackground="#CF6679",
           activeforeground="white",
           bd=0,
           highlightthickness=0,
           command=lambda: difficulty_level(1)).pack(pady=20)


def difficulty_level(mode):
    """Set difficulty & start game"""
    global difficulty_mode
    difficulty_mode = mode
    lib.set_difficulty(mode)
    start_game()


def start_game():
    """Init C backend & draw the board"""
    global user_symbol, bot_symbol, current_player

    lib.init_game()
    user_symbol = lib.get_user_symbol()
    bot_symbol = lib.get_bot_symbol()
    current_player = lib.get_current_player()

    draw_board()

    messagebox.showinfo(
        "Game Start",
        f"You are '{chr(user_symbol)}'\nBot is '{chr(bot_symbol)}'\n" +
        ("You start!" if current_player == 0 else "Bot starts!")
    )

    if current_player == 1:
        bot_turn()


def draw_board():
    """Create the 3x3 game grid"""
    global buttons, board_frame

    clear_window()

    board_frame = Frame(root, bg=BG_COLOR)
    board_frame.pack(pady=30)

    buttons = []
    for i in range(3):
        row = []
        for j in range(3):
            btn = Button(board_frame,
                         text="",
                         width=4,
                         height=2,
                         font=FONT_BTN,
                         bg=BTN_COLOR,
                         fg=BTN_TEXT_COLOR,
                         activebackground=ACCENT_COLOR,
                         activeforeground="black",
                         bd=0,
                         highlightthickness=0,
                         command=lambda x=i, y=j: on_button_click(x, y))
            btn.grid(row=i, column=j, padx=20, pady=20)
            row.append(btn)
        buttons.append(row)


def on_button_click(x, y):
    """Handle user click"""
    global current_player

    if current_player != 0:
        return

    result = lib.make_move(x, y)

    if result == -1:
        messagebox.showwarning("Invalid", "Cell already occupied!")
        return

    buttons[x][y].config(text=chr(user_symbol), fg=USER_MOVE_COLOR)

    if result == 1:
        end_game("You win!")
    elif result == 2:
        end_game("It's a Draw!")
    else:
        current_player = 1
        bot_turn()


def bot_turn():
    """Bot makes move"""
    global current_player

    result = lib.make_move(0, 0)
    x = lib.get_last_bot_row()
    y = lib.get_last_bot_col()

    buttons[x][y].config(text=chr(bot_symbol), fg=BOT_MOVE_COLOR)

    if result == 1:
        end_game("Bot wins!")
    elif result == 2:
        end_game("It's a Draw!")
    else:
        current_player = 0


def end_game(msg):
    """Show result and go back to main menu"""
    messagebox.showinfo("Game Over", msg)
    gui()


# Run the app
if __name__ == "__main__":
    gui()
    root.mainloop()
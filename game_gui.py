from tkinter import *
from tkinter import messagebox, filedialog
import random

root = Tk()
root.geometry("700x450")
root.title("Tic-Tac-Toe")
root.resizable(False, False)

current_player = None
current_symbol = None

def clear_window():
    for widget in root.winfo_children():
        widget.destroy()

    draw_board()

play_btn = Button(root, text="Play", font=("Segoe UI", 14, "bold"), width=25, height=2, command=clear_window)
play_btn.grid()

def draw_board():
    for i in range(3):
        for j in range(3):
            Button(root, command=lambda x=i, y=j: on_button_click(x, y)).grid(row=i, column=j)

    current_player = random.choice(["User", "Bot"])

    current_symbol = random.choice(["X", "O"])

    print(current_player, current_symbol)

def on_button_click(x, y):
    print(x, y)

if __name__ == "__main__":
    root.mainloop()
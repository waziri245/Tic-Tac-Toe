import pytest
from ctypes import CDLL, c_int, byref

# Load the compiled C shared library
lib = CDLL('./libgame_logic.so')

# Set return types for getter functions
lib.get_user_symbol.restype = c_int
lib.get_bot_symbol.restype = c_int
lib.get_current_player.restype = c_int
lib.get_last_bot_row.restype = c_int
lib.get_last_bot_col.restype = c_int

def setup_function():
    lib.init_game()

def test_symbols_are_different():
    user = lib.get_user_symbol()
    bot = lib.get_bot_symbol()
    assert user != bot, "User and bot should not have the same symbol"

def test_starting_player_is_valid():
    current = lib.get_current_player()
    assert current in (0, 1), "Current player must be 0 (user) or 1 (bot)"

def test_easy_mode_returns_valid_cell():
    row = c_int()
    col = c_int()
    lib.set_difficulty(0)
    lib.easy_mode(byref(row), byref(col))
    assert 0 <= row.value <= 2 and 0 <= col.value <= 2

def test_make_move_valid_user_move():
    lib.set_difficulty(0)
    lib.init_game()
    lib.make_move(0, 0)  # user plays
    result = lib.make_move(0, 0)  # bot (should overwrite)
    assert result in (0, 1, 2), "Bot move should return a valid result"

def test_bot_move_registered():
    lib.set_difficulty(1)
    lib.init_game()
    current = lib.get_current_player()
    if current == 1:
        lib.make_move(0, 0)  # dummy move, bot plays
        row = lib.get_last_bot_row()
        col = lib.get_last_bot_col()
        assert 0 <= row <= 2 and 0 <= col <= 2, "Bot should have picked a valid cell"




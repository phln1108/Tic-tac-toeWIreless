#include "tictactoe.h"

Tictactoe::Tictactoe() {
    for (u_int8_t i = 0; i < 3; i++) {
        for (u_int8_t j = 0; j < 3; j++) {
            map[i][j] = 0;
        }
    }
    Tictactoe::result = 0;
    Tictactoe::printMap();
}

uint8_t Tictactoe::checkWinner() {
    for (int i = 0; i < 3; ++i) {
        // Check rows
        if (map[i][0] == map[i][1] && map[i][1] == map[i][2] && map[i][0] != 0)
            return map[i][0];

        // Check columns
        if (map[0][i] == map[1][i] && map[1][i] == map[2][i] && map[0][i] != 0)
            return map[0][i];
    }

    // Check diagonals
    if ((map[0][0] == map[1][1] && map[1][1] == map[2][2] && map[0][0] != 0) || (map[0][2] == map[1][1] && map[1][1] == map[2][0] && map[0][2] != 0))
        return map[1][1];

    // Check if has empty spaces
    for (int i = 0; i < 3; ++i)
        for (int j = 0; j < 3; ++j)
            if (map[i][j] == 0)
                return 0;

    // it's a draw
    return 3;
}

uint8_t Tictactoe::play(uint8_t cell, uint8_t player) {
    if (cell > 9 && player < 3)
        return 4;

    if (turn == player)
        return 5;

    turn = player;
    uint8_t x = cell / 3;
    uint8_t y = cell % 3;

    if (map[x][y] != 0)
        return 4;

    map[x][y] = player;

    Tictactoe::result = Tictactoe::checkWinner();
    Tictactoe::printMap();
    return Tictactoe::result;
}

void Tictactoe::restart() {
    for (u_int8_t i = 0; i < 3; i++) {
        for (u_int8_t j = 0; j < 3; j++) {
            map[i][j] = 0;
        }
    }
    Tictactoe::result = 0;
    Tictactoe::printMap();
}

void Tictactoe::printMap() {
    for (uint8_t i = 0; i < 3; i++) {
        Serial.printf("%d|", map[i][0]);
        Serial.printf("%d|", map[i][1]);
        Serial.printf("%d\n", map[i][2]);
    }
    String msg = "";
    switch (Tictactoe::result) {
        case 0:
            msg = "The game is still running";
            break;
        case 1:
            msg = "You won! :D";
            break;
        case 2:
            msg = "They won! :(";
            break;
        case 3:
            msg = "It's a  draw (-_-)";
            break;
    }
    Serial.println(msg);
}

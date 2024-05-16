#pragma once

#include <Arduino.h>

/// @brief handle a tic-tac-toe board and game
class Tictactoe {
    private:
        uint8_t map[3][3] = {0};
        uint8_t teste;
        uint8_t result = 0;
        uint8_t checkWinner();
        uint8_t turn;

        void printMap();
    public:
    /// @brief set the map
    Tictactoe();
    // ~tictactoe();
    
    /// @brief Handle's the play made by a player
    /// @param cell [0 : 8] Cells to play
    /// @param player Player who made the play
    /// @return - 0: The game is still playable
    /// @return - 1: Player 1 won
    /// @return - 2: Player 2 won
    /// @return - 3: The game ended in a draw
    /// @return - 4: The play was not possible (eg: cell already assigned or cell not in range)
    /// @return - 5: Not your turn 
    uint8_t play(uint8_t cell, uint8_t player);
    
    /// @brief Restart the board
    void restart();
};



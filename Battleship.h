/**
 * @file Battleship.h
 * @brief Header file for the Battleship game.
 */

#ifndef BATTLESHIP_H
#define BATTLESHIP_H

#include <string>

// -----------------------------------------------------------------------------
//  Constants
// -----------------------------------------------------------------------------

const int BOARD_SIZE = 10;

const char EMPTY = '~';
// NOTE: SHIP ('S') constant removed - it was never written to the board and
// caused hit/overlap detection bugs. Ships use their own symbols: C B D U P
const char HIT   = 'X';
const char MISS  = 'O';

const int NUM_SHIPS = 5;

const int HIT_BONUS = 50;   // Bonus points for every successful hit

// -----------------------------------------------------------------------------
//  Structs
// -----------------------------------------------------------------------------

struct Ship {
    std::string name;     // Ship name (e.g., "Carrier")
    char        symbol;   // Single character shown on the board (C, B, D, U, P)
    int         size;     // Number of cells the ship occupies
    int         hits;     // Number of times this ship has been hit
    bool        sunk;     // True when hits == size
    int         points;   // Bonus points awarded when this ship is fully sunk
};

struct Player {
    char board[BOARD_SIZE][BOARD_SIZE];         // Own board (ships visible)
    char trackingBoard[BOARD_SIZE][BOARD_SIZE]; // Shots fired at opponent
    int  shipIndex[BOARD_SIZE][BOARD_SIZE];     // Index (0-4) of ship at each cell, or -1
    Ship ships[NUM_SHIPS];                      // Fleet
    int  shotsGuessed[BOARD_SIZE][BOARD_SIZE];  // AI: tracks already-guessed cells
    int  shipsRemaining;                        // Ships still afloat
    int  score;                                 // Cumulative points earned this game

    // AI hunt mode state
    int  firstHitRow;   // Row of first hit on current target ship (-1 if none)
    int  firstHitCol;   // Col of first hit on current target ship (-1 if none)
    int  lastHitRow;    // Row of most recent hit (-1 if none)
    int  lastHitCol;    // Col of most recent hit (-1 if none)
    bool hunting;       // True when AI has an active hit to follow up
    int  huntAxis;      // 0 = unknown, 1 = horizontal, 2 = vertical
    int  huntDir;       // +1 or -1: current direction along the axis
};

// -----------------------------------------------------------------------------
//  Function Prototypes
// -----------------------------------------------------------------------------

/* --- board.cpp --- */
void initBoard(char board[BOARD_SIZE][BOARD_SIZE]);
void initShipIndex(int idx[BOARD_SIZE][BOARD_SIZE]);
void initShotTracker(int tracker[BOARD_SIZE][BOARD_SIZE]);
void printBoard(const char board[BOARD_SIZE][BOARD_SIZE], const std::string& title, bool hideShips = false, const int shipIdx[BOARD_SIZE][BOARD_SIZE] = nullptr);
void printBothBoards(const Player& human, const Player& ai);

/* --- ships.cpp --- */
void initShips(Ship ships[NUM_SHIPS]);
bool placeShip(char board[BOARD_SIZE][BOARD_SIZE],
               int  idx[BOARD_SIZE][BOARD_SIZE],
               int row, int col, int size, bool horizontal, int shipIdx, char symbol);
void placeShipsRandom(char board[BOARD_SIZE][BOARD_SIZE],
                      int  idx[BOARD_SIZE][BOARD_SIZE],
                      Ship ships[NUM_SHIPS]);
void placeShipsManual(char board[BOARD_SIZE][BOARD_SIZE],
                      int  idx[BOARD_SIZE][BOARD_SIZE],
                      Ship ships[NUM_SHIPS]);

/* --- gameplay.cpp --- */
int  fireShot(char opponentBoard[BOARD_SIZE][BOARD_SIZE],
              char trackingBoard[BOARD_SIZE][BOARD_SIZE],
              int  shipIndex[BOARD_SIZE][BOARD_SIZE],
              Ship opponentShips[NUM_SHIPS],
              int& shipsRemaining,
              int row, int col);
bool aiFireShot(Player& ai, Player& human);
bool checkWin(int shipsRemaining);

/* --- input.cpp --- */
bool getPlayerShot(int& row, int& col);
char getYesNo(const std::string& prompt);
int  getMenuChoice(int min, int max);

/* --- score.cpp --- */
void loadHighScore(int& highScore);
void saveHighScore(int score);
void displayHighScore();
void displayScoreboard(const Player& human, const Player& ai, int turnScore);

/* --- utils.cpp --- */
void clearScreen();
void displayTitle();
void displayRules();

#endif // BATTLESHIP_H

/**
 * @file ships.cpp
 * @brief Ship initialisation and placement logic.
 *
 * Fixes:
 *  1. placeShip() overlap check uses (!= EMPTY) instead of (== SHIP).
 *  2. Removed the extra cin.ignore() before cin.get() at the end of
 *     placeShipsManual() that was silently swallowing the ENTER keypress.
 */

#include "Battleship.h"
#include "colors.h"
#include "input.cpp"
#include <iostream>
#include <cstdlib>
using namespace std;

// Point values per ship (awarded on sinking).
// Fleet: Carrier(5) Battleship(4) Destroyer(3) Submarine(3) Patrol Boat(2)
static const int SHIP_POINTS[NUM_SHIPS] = {500, 300, 200, 200, 100};

void initShips(Ship ships[NUM_SHIPS])
{
    const char *names[] = {"Carrier", "Battleship", "Destroyer",
                           "Submarine", "Patrol Boat"};
    const char symbols[] = {'C', 'B', 'D', 'U', 'P'};
    const int sizes[] = {5, 4, 3, 3, 2};

    for (int i = 0; i < NUM_SHIPS; ++i)
    {
        ships[i].name = names[i];
        ships[i].symbol = symbols[i];
        ships[i].size = sizes[i];
        ships[i].hits = 0;
        ships[i].sunk = false;
        ships[i].points = SHIP_POINTS[i];
    }
}

bool placeShip(char board[BOARD_SIZE][BOARD_SIZE],
               int idx[BOARD_SIZE][BOARD_SIZE],
               int row, int col, int size, bool horizontal, int shipIdx, char symbol)
{
    if (horizontal)
    {
        if (col + size > BOARD_SIZE)
            return false;
    }
    else
    {
        if (row + size > BOARD_SIZE)
            return false;
    }

    for (int i = 0; i < size; ++i)
    {
        int r = row + (horizontal ? 0 : i);
        int c = col + (horizontal ? i : 0);
        if (board[r][c] != EMPTY)
            return false;
    }

    for (int i = 0; i < size; ++i)
    {
        int r = row + (horizontal ? 0 : i);
        int c = col + (horizontal ? i : 0);
        board[r][c] = symbol;
        idx[r][c] = shipIdx;
    }

    return true;
}

void placeShipsRandom(char board[BOARD_SIZE][BOARD_SIZE],
                      int idx[BOARD_SIZE][BOARD_SIZE],
                      Ship ships[NUM_SHIPS])
{
    for (int i = 0; i < NUM_SHIPS; ++i)
    {
        bool placed = false;
        while (!placed)
        {
            int row = rand() % BOARD_SIZE;
            int col = rand() % BOARD_SIZE;
            bool horizontal = (rand() % 2 == 0);
            placed = placeShip(board, idx, row, col, ships[i].size,
                               horizontal, i, ships[i].symbol);
        }
    }
}

void placeShipsManual(char board[BOARD_SIZE][BOARD_SIZE],
                      int idx[BOARD_SIZE][BOARD_SIZE],
                      Ship ships[NUM_SHIPS])
{
    cout << "\n=== PLACE YOUR FLEET ===\n";
    cout << "Columns: A-J   Rows: 1-10\n";
    cout << "Direction: H = horizontal   V = vertical\n\n";

    for (int i = 0; i < NUM_SHIPS; ++i)
    {
        printBoard(board, "Your Current Fleet");

        cout << "\nPlacing: " << ships[i].name
             << "  (size " << ships[i].size
             << ", worth " << ships[i].points << " pts)\n";

        bool placed = false;
        while (!placed)
        {
            cout << "  Enter column (A-J): ";
            char colChar;
            cin >> colChar;
            if (cin.fail())
            {
                cin.clear();
                cin.ignore(1000, '\n');
                continue;
            }
            colChar = toupper(colChar);
            if (colChar < 'A' || colChar > 'J')
            {
                cout << "  Invalid column. Use A-J.\n";
                continue;
            }

            cout << "  Enter row    (1-10): ";
            int rowNum;
            cin >> rowNum;
            if (cin.fail() || rowNum < 1 || rowNum > 10)
            {
                cin.clear();
                cin.ignore(1000, '\n');
                cout << "  Invalid row. Use 1-10.\n";
                continue;
            }

            cout << "  Direction (H/V): ";
            char dir;
            cin >> dir;
            if (cin.fail())
            {
                cin.clear();
                cin.ignore(1000, '\n');
                continue;
            }
            dir = toupper(dir);
            if (dir != 'H' && dir != 'V')
            {
                cout << "  Enter H or V.\n";
                cin.ignore(1000, '\n');
                continue;
            }
            cin.ignore(1000, '\n'); // consume newline after direction input

            int col = colChar - 'A';
            int row = rowNum - 1;
            bool horizontal = (dir == 'H');

            if (placeShip(board, idx, row, col, ships[i].size,
                          horizontal, i, ships[i].symbol))
            {
                placed = true;
                cout << CLR_SUCCESS << "  " << ships[i].name << " placed!" << CLR_RESET << "\n";
            }
            else
            {
                cout << CLR_ERROR << "  Invalid position (out of bounds or overlap). Try again." << CLR_RESET << "\n";
            }
        }
    }

    printBoard(board, "Your Final Fleet");
    // FIX: removed the extra cin.ignore() here that was eating the ENTER press.
    cout << "\nAll ships placed! Press ENTER to start the battle...";
    cin.get();
}

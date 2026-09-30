/**
 * @file gameplay.cpp
 * @brief Core game mechanics: firing shots, AI opponent, and win detection.
 *        Enhanced with ANSI color output.
 */

#include "Battleship.h"
#include "colors.h"
#include <iostream>
#include <cstdlib>
using namespace std;

// -----------------------------------------------------------------------------
//  fireShot
// -----------------------------------------------------------------------------
int fireShot(char opponentBoard[BOARD_SIZE][BOARD_SIZE],
             char trackingBoard[BOARD_SIZE][BOARD_SIZE],
             int  shipIndex[BOARD_SIZE][BOARD_SIZE],
             Ship opponentShips[NUM_SHIPS],
             int& shipsRemaining,
             int row, int col)
{
    if (opponentBoard[row][col] == HIT || opponentBoard[row][col] == MISS)
        return 0;

    if (opponentBoard[row][col] != EMPTY) {
        opponentBoard[row][col] = HIT;
        trackingBoard[row][col] = HIT;

        int si = shipIndex[row][col];
        Ship& s = opponentShips[si];

        cout << CLR_HIT << "  *** HIT" << CLR_RESET
             << CLR_INFO << " at " << (char)('A' + col) << (row + 1) << "! "
             << CLR_HIT << "***" << CLR_RESET << "\n";
        cout << CLR_BONUS << "  [+" << HIT_BONUS << " pts"
             << CLR_INFO << " - hit on " << shipColor(si) << s.name << CLR_RESET
             << CLR_INFO << "]" << CLR_RESET << "\n";

        int earned = HIT_BONUS;
        s.hits++;

        if (s.hits == s.size) {
            s.sunk = true;
            shipsRemaining--;
            earned += s.points;
            cout << CLR_BOLD << CLR_BRED << "  *** " << s.name << " has been SUNK! ***" << CLR_RESET << "\n";
            cout << CLR_BONUS << "  [+" << s.points << " pts"
                 << CLR_INFO << " - " << shipColor(si) << s.name << CLR_RESET
                 << CLR_INFO << " destroyed!]" << CLR_RESET << "\n";
        }

        return earned;

    } else {
        opponentBoard[row][col] = MISS;
        trackingBoard[row][col] = MISS;
        cout << CLR_MISS << "  Miss" << CLR_RESET
             << CLR_INFO << " at " << (char)('A' + col) << (row + 1)
             << ". [+0 pts]" << CLR_RESET << "\n";
        return 0;
    }
}

// -----------------------------------------------------------------------------
//  resetHunt
// -----------------------------------------------------------------------------
static void resetHunt(Player& ai) {
    ai.hunting      = false;
    ai.firstHitRow  = -1;
    ai.firstHitCol  = -1;
    ai.lastHitRow   = -1;
    ai.lastHitCol   = -1;
    ai.huntAxis     = 0;
    ai.huntDir      = 1;
}

// -----------------------------------------------------------------------------
//  aiFireShot
// -----------------------------------------------------------------------------
bool aiFireShot(Player& ai, Player& human) {
    int row = -1, col = -1;

    if (!ai.hunting) {
        do {
            row = rand() % BOARD_SIZE;
            col = rand() % BOARD_SIZE;
        } while (ai.shotsGuessed[row][col] == 1);

    } else if (ai.huntAxis == 0) {
        const int dr[] = {-1, 0, 1,  0};
        const int dc[] = { 0, 1, 0, -1};
        bool found = false;
        for (int d = 0; d < 4 && !found; ++d) {
            int nr = ai.firstHitRow + dr[d];
            int nc = ai.firstHitCol + dc[d];
            if (nr >= 0 && nr < BOARD_SIZE &&
                nc >= 0 && nc < BOARD_SIZE &&
                ai.shotsGuessed[nr][nc] == 0) {
                row = nr; col = nc; found = true;
            }
        }
        if (!found) {
            do {
                row = rand() % BOARD_SIZE;
                col = rand() % BOARD_SIZE;
            } while (ai.shotsGuessed[row][col] == 1);
        }

    } else {
        bool found = false;
        int nr = ai.lastHitRow + (ai.huntAxis == 2 ? ai.huntDir : 0);
        int nc = ai.lastHitCol + (ai.huntAxis == 1 ? ai.huntDir : 0);
        if (nr >= 0 && nr < BOARD_SIZE && nc >= 0 && nc < BOARD_SIZE &&
            ai.shotsGuessed[nr][nc] == 0) {
            row = nr; col = nc; found = true;
        }
        if (!found) {
            ai.huntDir = -ai.huntDir;
            nr = ai.firstHitRow + (ai.huntAxis == 2 ? ai.huntDir : 0);
            nc = ai.firstHitCol + (ai.huntAxis == 1 ? ai.huntDir : 0);
            if (nr >= 0 && nr < BOARD_SIZE && nc >= 0 && nc < BOARD_SIZE &&
                ai.shotsGuessed[nr][nc] == 0) {
                row = nr; col = nc;
                ai.lastHitRow = ai.firstHitRow;
                ai.lastHitCol = ai.firstHitCol;
                found = true;
            }
        }
        if (!found) {
            do {
                row = rand() % BOARD_SIZE;
                col = rand() % BOARD_SIZE;
            } while (ai.shotsGuessed[row][col] == 1);
        }
    }

    ai.shotsGuessed[row][col] = 1;
    cout << "\n  " << CLR_ENEMY_SCORE << "Enemy fires at "
         << (char)('A' + col) << (row + 1) << "..." << CLR_RESET << "\n";

    char before = human.board[row][col];

    int earned = fireShot(human.board, ai.trackingBoard,
                          human.shipIndex, human.ships,
                          human.shipsRemaining, row, col);
    ai.score += earned;

    bool wasHit = (before != EMPTY && before != HIT && before != MISS);
    if (wasHit) {
        int si = human.shipIndex[row][col];
        if (si >= 0 && human.ships[si].sunk) {
            resetHunt(ai);
        } else {
            if (!ai.hunting) {
                ai.hunting     = true;
                ai.firstHitRow = row; ai.firstHitCol = col;
                ai.lastHitRow  = row; ai.lastHitCol  = col;
                ai.huntAxis    = 0;   ai.huntDir      = 1;
            } else if (ai.huntAxis == 0) {
                ai.huntAxis = (row == ai.firstHitRow) ? 1 : 2;
                ai.huntDir  = (ai.huntAxis == 1)
                                ? (col > ai.firstHitCol ? 1 : -1)
                                : (row > ai.firstHitRow ? 1 : -1);
                ai.lastHitRow = row; ai.lastHitCol = col;
            } else {
                ai.lastHitRow = row; ai.lastHitCol = col;
            }
        }
    }

    return true;
}

bool checkWin(int shipsRemaining) {
    return shipsRemaining == 0;
}

/**
 * @file board.cpp
 * @brief Functions for initialising and rendering the game boards.
 *        Enhanced with ANSI color output.
 *
 * Alignment (all measurements in VISIBLE characters, 0-based positions):
 *
 *   Data row:   setw(2) num + " |" + " " + 10×"X " + "|"
 *               pos 0-1=num, 2=' ', 3='|', 4=' ', 5-24=cells, 25='|'
 *
 *   Grid border: "   +" + 21 dashes + "+"
 *               3 spaces → '+' at pos 3, matches '|' at pos 3 in data rows
 *               21 dashes → '+' at pos 25, matches '|' at pos 25 in data rows
 *
 *   Col labels:  5 spaces → 'A' at pos 5, sits above first cell char
 *
 *   Title box:   "   +" + OUTER_W dashes + "+"
 *               Same 3-space prefix as grid border (left edges align).
 *               OUTER_W = max(21, title.size() + 1) so title never clips.
 *               Right edge may extend beyond grid when title is long — that is fine.
 */

#include "Battleship.h"
#include "colors.h"
#include <iostream>
#include <iomanip>
#include <string>
using namespace std;

// ── Board initialisers ────────────────────────────────────────────────────────

void initBoard(char board[BOARD_SIZE][BOARD_SIZE])
{
    for (int r = 0; r < BOARD_SIZE; ++r)
        for (int c = 0; c < BOARD_SIZE; ++c)
            board[r][c] = EMPTY;
}

void initShipIndex(int idx[BOARD_SIZE][BOARD_SIZE])
{
    for (int r = 0; r < BOARD_SIZE; ++r)
        for (int c = 0; c < BOARD_SIZE; ++c)
            idx[r][c] = -1;
}

void initShotTracker(int tracker[BOARD_SIZE][BOARD_SIZE])
{
    for (int r = 0; r < BOARD_SIZE; ++r)
        for (int c = 0; c < BOARD_SIZE; ++c)
            tracker[r][c] = 0;
}

// ── Helpers ───────────────────────────────────────────────────────────────────

static bool isShipCell(char c)
{
    return c != EMPTY && c != HIT && c != MISS;
}

static const char *getCellColor(char cell,
                                const int idx[BOARD_SIZE][BOARD_SIZE],
                                int r, int c)
{
    switch (cell)
    {
    case '~':
        return CLR_WATER;
    case 'X':
        return CLR_HIT;
    case 'O':
        return CLR_MISS;
    default:
        if (idx && idx[r][c] >= 0)
            return shipColor(idx[r][c]);
        return CLR_RESET;
    }
}

// Emit character ch exactly n times
static void rep(char ch, int n)
{
    for (int i = 0; i < n; ++i)
        cout << ch;
}

// ── printBoard ────────────────────────────────────────────────────────────────

void printBoard(const char board[BOARD_SIZE][BOARD_SIZE], const string &title,
                bool hideShips,
                const int shipIdx[BOARD_SIZE][BOARD_SIZE])
{
    // ── Fixed measurements ────────────────────────────────────────────────────
    // GRID_W = dashes between the grid's "+" corners = 21
    //   Derivation: 1 inner space + 10 cells × 2 chars = 21
    //   Grid: "   +" + 21 dashes + "+"  → left '+' at col 3, right '+' at col 25
    //   Data: " 1 |" (4 chars) + " " + 10×"X " + "|"
    //                                   ^col 4    col 5-24   col 25
    //   The '|' walls of data rows are at col 3 and col 25 — match grid corners ✓
    const int GRID_W = 21;

    // OUTER_W = inner width (dashes) of the title box.
    //   +2 gives at least 1-space margin each side of the title.
    //   Must also be >= GRID_W so the box is never narrower than the grid.
    const int OUTER_W = ((int)title.size() + 2 > GRID_W)
                            ? (int)title.size() + 2
                            : GRID_W;

    // ── Title box top ─────────────────────────────────────────────────────────
    cout << "\n"
         << CLR_BORDER << "   +";
    rep('-', OUTER_W);
    cout << "+" << CLR_RESET << "\n";

    // ── Title row — centered with equal margins ───────────────────────────────
    {
        int pad_total = OUTER_W - (int)title.size();
        int lpad = pad_total / 2;
        int rpad = pad_total - lpad;
        cout << CLR_BORDER << "   |" << CLR_RESET;
        rep(' ', lpad);
        cout << CLR_HEADER << title << CLR_RESET;
        rep(' ', rpad);
        cout << CLR_BORDER << "|" << CLR_RESET << "\n";
    }

    // ── Title box bottom ──────────────────────────────────────────────────────
    cout << CLR_BORDER << "   +";
    rep('-', OUTER_W);
    cout << "+" << CLR_RESET << "\n";

    // ── Column labels ─────────────────────────────────────────────────────────
    // 5-space indent: col 0-1 = row-num area (2), col 2 = ' ', col 3 = '|', col 4 = ' '
    // 'A' must sit at col 5 (above the first cell character)
    cout << "     ";
    for (int c = 0; c < BOARD_SIZE; ++c)
        cout << CLR_LABEL << (char)('A' + c) << CLR_RESET << " ";
    cout << "\n";

    // ── Grid top border ───────────────────────────────────────────────────────
    // "   +" → '+' at col 3 (aligns with '|' in data rows)
    cout << CLR_BORDER << "   +";
    rep('-', GRID_W);
    cout << "+" << CLR_RESET << "\n";

    // ── Data rows ─────────────────────────────────────────────────────────────
    for (int r = 0; r < BOARD_SIZE; ++r)
    {
        // setw(2) right-aligns row number in 2 chars → cols 0-1
        // " |" → col 2 = ' ', col 3 = '|'
        // " "  → col 4 = leading space inside grid
        cout << CLR_LABEL << setw(2) << (r + 1) << CLR_RESET
             << CLR_BORDER << " |" << CLR_RESET
             << " ";

        for (int c = 0; c < BOARD_SIZE; ++c)
        {
            char cell = board[r][c];
            if (hideShips && isShipCell(cell))
            {
                cout << CLR_WATER << "~ " << CLR_RESET;
            }
            else
            {
                const char *color = getCellColor(cell, shipIdx, r, c);
                cout << color << cell << " " << CLR_RESET;
            }
        }

        // closing '|' at col 25
        cout << CLR_BORDER << "|" << CLR_RESET << "\n";
    }

    // ── Grid bottom border ────────────────────────────────────────────────────
    cout << CLR_BORDER << "   +";
    rep('-', GRID_W);
    cout << "+" << CLR_RESET << "\n";
}

// ── printBothBoards ───────────────────────────────────────────────────────────

void printBothBoards(const Player &human, const Player &ai)
{
    (void)ai;
    cout << "\n";
    printBoard(human.trackingBoard, "YOUR SHOTS  (Enemy Waters)", true, nullptr);
    printBoard(human.board, "YOUR FLEET", false, human.shipIndex);
}

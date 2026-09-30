/**
 * @file utils.cpp
 * @brief Utility / presentation helpers.
 *        Colors via ANSI, plain ASCII only (no box-drawing or emoji).
 *
 * Alignment fix: title box padding computed from plain string lengths only.
 */

#include "Battleship.h"
#include "colors.h"
#include <iostream>
#include <string>
using namespace std;

void clearScreen()
{
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

// ── Title screen ──────────────────────────────────────────────────────────────
//
// Box: "  +======================================+"  → 38 '=' chars inside
// Row: "  |" + 38 visible chars + "|"
//
void displayTitle()
{
    clearScreen();
    const int BOX = 38;

    auto centeredRow = [&](const string &text, const char *color)
    {
        int pad = BOX - (int)text.size();
        int lpad = pad / 2;
        int rpad = pad - lpad;
        cout << CLR_BORDER << "  |" << CLR_RESET << color;
        for (int i = 0; i < lpad; i++)
            cout << ' ';
        cout << text;
        for (int i = 0; i < rpad; i++)
            cout << ' ';
        cout << CLR_RESET << CLR_BORDER << "|\n"
             << CLR_RESET;
    };

    cout << "\n";
    cout << CLR_BORDER << "  +";
    for (int i = 0; i < BOX; i++)
        cout << '=';
    cout << "+" << CLR_RESET << "\n";
    centeredRow("** BATTLESHIP **", CLR_TITLE);
    centeredRow("Human  vs.  AI", CLR_INFO);
    cout << CLR_BORDER << "  +";
    for (int i = 0; i < BOX; i++)
        cout << '=';
    cout << "+" << CLR_RESET << "\n";
    cout << "\n";
}

// ── Rules / help screen ───────────────────────────────────────────────────────

void displayRules()
{
    cout << "\n"
         << CLR_HEADER << "======================== HOW TO PLAY ========================" << CLR_RESET << "\n\n";
    cout << CLR_INFO << "  1. Place your 5 ships on a 10x10 grid.\n";
    cout << "  2. Take turns firing at the enemy grid using coordinates.\n";
    cout << "     Example: column B, row 4  ->  enter B then 4\n";
    cout << "  3. Sink all enemy ships before they sink yours!\n"
         << CLR_RESET;
    cout << "\n"
         << CLR_HEADER << "=============================================================" << CLR_RESET << "\n\n";

    cout << CLR_HEADER << "============ FLEET & POINT VALUES ============" << CLR_RESET << "\n";
    cout << CLR_LABEL << "  Ship            Size   Sink pts   Hit bonus\n";
    cout << CLR_BORDER << "  -------------------------------------------\n"
         << CLR_RESET;

    const char *labels[] = {"Carrier", "Battleship", "Destroyer", "Submarine", "Patrol Boat"};
    const int sizes[] = {5, 4, 3, 3, 2};
    const int spts[] = {500, 300, 200, 200, 100};
    for (int i = 0; i < 5; ++i)
    {
        cout << "  " << shipColor(i) << labels[i] << CLR_RESET;
        int pad = 16 - (int)string(labels[i]).size();
        for (int p = 0; p < pad; ++p)
            cout << " ";
        cout << CLR_INFO << sizes[i] << "       "
             << CLR_SCORE << spts[i] << "        +50/hit\n"
             << CLR_RESET;
    }
    cout << CLR_HEADER << "==============================================" << CLR_RESET << "\n";

    cout << "\n"
         << CLR_HEADER << "=============== SCORING ===============" << CLR_RESET << "\n";
    cout << CLR_BONUS << "  +50 points " << CLR_INFO << "for every hit\n";
    cout << CLR_BONUS << "  +Sink bonus " << CLR_INFO << "when a ship is destroyed\n";
    cout << CLR_WARNING << "  High score is only recorded on a WIN\n"
         << CLR_RESET;
    cout << CLR_HEADER << "=======================================" << CLR_RESET << "\n\n";

    cout << CLR_HEADER << "======== LEGEND ========" << CLR_RESET << "\n";
    cout << CLR_WATER << "  ~" << CLR_RESET << CLR_INFO << "  Water (unexplored)\n"
         << CLR_RESET;
    cout << shipColor(0) << "  C" << CLR_RESET << CLR_INFO << "  Carrier\n"
         << CLR_RESET;
    cout << shipColor(1) << "  B" << CLR_RESET << CLR_INFO << "  Battleship\n"
         << CLR_RESET;
    cout << shipColor(2) << "  D" << CLR_RESET << CLR_INFO << "  Destroyer\n"
         << CLR_RESET;
    cout << shipColor(3) << "  U" << CLR_RESET << CLR_INFO << "  Submarine\n"
         << CLR_RESET;
    cout << shipColor(4) << "  P" << CLR_RESET << CLR_INFO << "  Patrol Boat\n"
         << CLR_RESET;
    cout << CLR_HIT << "  X" << CLR_RESET << CLR_INFO << "  HIT\n"
         << CLR_RESET;
    cout << CLR_MISS << "  O" << CLR_RESET << CLR_INFO << "  MISS\n"
         << CLR_RESET;
    cout << CLR_HEADER << "========================" << CLR_RESET << "\n";
}

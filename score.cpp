/**
 * @file score.cpp
 * @brief High-score persistence and scoreboard display.
 *
 * Alignment fix: setw() is never applied to strings that are preceded by ANSI
 * codes.  Instead every column is padded with explicit spaces so that the
 * visible character count is always exact.
 */

#include "Battleship.h"
#include "colors.h"
#include <iostream>
#include <fstream>
#include <iomanip>
#include <string>
using namespace std;

static const string SCORE_FILE = "highscore.txt";

// ── High-score I/O ────────────────────────────────────────────────────────────

void loadHighScore(int &highScore)
{
    ifstream file(SCORE_FILE);
    if (file.is_open())
    {
        file >> highScore;
        if (file.fail())
            highScore = 0;
        file.close();
    }
    else
    {
        highScore = 0;
    }
}

void saveHighScore(int score)
{
    ofstream file(SCORE_FILE);
    if (file.is_open())
    {
        file << score;
        file.close();
        cout << "\n  " << CLR_TITLE << "** NEW HIGH SCORE: " << score << " points! **" << CLR_RESET << "\n";
    }
    else
    {
        cout << "\n  " << CLR_ERROR << "Warning: could not save high score to file." << CLR_RESET << "\n";
    }
}

void displayHighScore()
{
    int best;
    loadHighScore(best);
    if (best == 0)
        cout << CLR_DIM << "  No high score yet - be the first to set one!\n"
             << CLR_RESET;
    else
        cout << CLR_TITLE << "       ** Best score: " << best << " points **\n\n"
             << CLR_RESET;
}

// ── Helpers ───────────────────────────────────────────────────────────────────

// Left-pad a string with spaces to exactly `width` visible characters.
static string padRight(const string &s, int width)
{
    string out = s;
    while ((int)out.size() < width)
        out += ' ';
    return out;
}

static string padLeft(const string &s, int width)
{
    string out = s;
    while ((int)out.size() < width)
        out = ' ' + out;
    return out;
}

// ── Fleet status table ────────────────────────────────────────────────────────
//
// Column visible widths (between the | separators, including one space each side):
//   Ship    : 14  ("  " + name(12) + " ")  but we print "| " + 12 + " |"
//   Size    :  6  ("  " + digit + "   ")
//   Hits    :  8  ("  " + hits/size + "   ")   max "10/10" = 5 chars → pad to 6
//   Status  : 10  ("  " + 8 chars + " ")
//   Points  :  8  ("  " + 5 chars + "  ")
//
// Border line:  "  +--------------+------+--------+----------+--------+"
//                     14 dashes    6 dashes 8 dashes  10 dashes  8 dashes
//
static void printFleetStatus(const Ship ships[], const string &label, bool isEnemy)
{
    const char *labelColor = isEnemy ? CLR_ENEMY_SCORE : CLR_SCORE;

    cout << "\n  " << labelColor << CLR_BOLD << label << CLR_RESET << "\n";

    // Border / header
    const char *DIV = "  +--------------+------+--------+----------+--------+\n";
    cout << CLR_BORDER << DIV << CLR_RESET;
    cout << CLR_LABEL
         << "  | " << padRight("Ship", 12) << " "
         << "| " << padRight("Size", 4) << " "
         << "| " << padRight("Hits", 6) << " "
         << "| " << padRight("Status", 8) << " "
         << "| " << padRight("Points", 6) << " |\n"
         << CLR_RESET;
    cout << CLR_BORDER << DIV << CLR_RESET;

    for (int i = 0; i < NUM_SHIPS; ++i)
    {
        const Ship &s = ships[i];

        // Status string (8 visible chars for the column content)
        const char *statusColor;
        string statusStr;
        if (s.sunk)
        {
            statusColor = CLR_SUNK;
            statusStr = padRight("SUNK", 8);
        }
        else if (s.hits > 0)
        {
            statusColor = CLR_HIT_STATUS;
            statusStr = padRight("HIT", 8);
        }
        else
        {
            statusColor = CLR_AFLOAT;
            statusStr = padRight("Afloat", 8);
        }

        // Hits string e.g. "3/5"
        string hitsStr = to_string(s.hits) + "/" + to_string(s.size);

        // Points string (or "---" when sunk)
        string ptsStr = s.sunk ? "---" : to_string(s.points);

        // Print the row — each field padded to exact visible width, color outside
        cout << CLR_BORDER << "  | " << CLR_RESET;
        cout << shipColor(i) << padRight(s.name, 12) << CLR_RESET;
        cout << CLR_BORDER << " | " << CLR_RESET;
        cout << CLR_INFO << padLeft(to_string(s.size), 4) << CLR_RESET;
        cout << CLR_BORDER << " | " << CLR_RESET;
        cout << CLR_INFO << padLeft(hitsStr, 6) << CLR_RESET;
        cout << CLR_BORDER << " | " << CLR_RESET;
        cout << statusColor << statusStr << CLR_RESET;
        cout << CLR_BORDER << " | " << CLR_RESET;
        cout << CLR_SCORE << padLeft(ptsStr, 6) << CLR_RESET;
        cout << CLR_BORDER << " |" << CLR_RESET << "\n";
    }

    cout << CLR_BORDER << DIV << CLR_RESET;
}

// ── Scoreboard box ────────────────────────────────────────────────────────────
//
// Box width: "  +====================================+" → 36 '=' chars inside
// Inner visible width = 36
// Each score row:  "  | YOUR score :     0              |"
//                    2+1  14 chars  6 chars  rest padding  1
//
void displayScoreboard(const Player &human, const Player &ai, int turnScore)
{
    const int BOX = 36; // inner width (number of '=' / '-' chars)

    // Top
    cout << "\n"
         << CLR_BORDER << "  +";
    for (int i = 0; i < BOX; i++)
        cout << '=';
    cout << "+" << CLR_RESET << "\n";

    // Title row — "  |" + centered "SCOREBOARD" + "|"
    {
        const string title = "SCOREBOARD";
        int pad = BOX - (int)title.size();
        int lpad = pad / 2;
        int rpad = pad - lpad;
        cout << CLR_BORDER << "  |" << CLR_RESET
             << CLR_HEADER;
        for (int i = 0; i < lpad; i++)
            cout << ' ';
        cout << title;
        for (int i = 0; i < rpad; i++)
            cout << ' ';
        cout << CLR_RESET
             << CLR_BORDER << "|" << CLR_RESET << "\n";
    }

    cout << CLR_BORDER << "  +";
    for (int i = 0; i < BOX; i++)
        cout << '-';
    cout << "+" << CLR_RESET << "\n";

    // Score rows
    // Format: "  | LABEL : " + right-aligned score (6 chars) + bonus + padding + "|"
    // Visible: "  | " = 4, label = 14 ("YOUR score : " / "Enemy score: "), score = 6 → 24 used
    // BOX = 36, so 12 chars remain for bonus/padding
    {
        // Human score row
        string scoreStr = to_string(human.score);
        string bonusStr = "";
        if (turnScore > 0)
            bonusStr = "(+" + to_string(turnScore) + " this turn)";

        // Row content (visible): " YOUR score :  " + score(6) + " " + bonus
        string rowContent = " YOUR score :  " + padLeft(scoreStr, 6);
        if (!bonusStr.empty())
            rowContent += "  " + bonusStr;
        int rpad = BOX - (int)rowContent.size();
        if (rpad < 0)
            rpad = 0;

        cout << CLR_BORDER << "  |" << CLR_RESET
             << CLR_SCORE << " YOUR score :  " << padLeft(scoreStr, 6) << CLR_RESET;
        if (!bonusStr.empty())
            cout << CLR_BONUS << "  " << bonusStr << CLR_RESET;
        for (int i = 0; i < rpad; i++)
            cout << ' ';
        cout << CLR_BORDER << "|" << CLR_RESET << "\n";
    }
    {
        // Enemy score row
        string scoreStr = to_string(ai.score);
        string rowContent = " Enemy score:  " + padLeft(scoreStr, 6);
        int rpad = BOX - (int)rowContent.size();
        if (rpad < 0)
            rpad = 0;

        cout << CLR_BORDER << "  |" << CLR_RESET
             << CLR_ENEMY_SCORE << " Enemy score:  " << padLeft(scoreStr, 6) << CLR_RESET;
        for (int i = 0; i < rpad; i++)
            cout << ' ';
        cout << CLR_BORDER << "|" << CLR_RESET << "\n";
    }

    // Bottom
    cout << CLR_BORDER << "  +";
    for (int i = 0; i < BOX; i++)
        cout << '=';
    cout << "+" << CLR_RESET << "\n";

    printFleetStatus(ai.ships, "ENEMY FLEET STATUS", true);
    printFleetStatus(human.ships, "YOUR FLEET STATUS", false);
}

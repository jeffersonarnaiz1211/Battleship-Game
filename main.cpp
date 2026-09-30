/**
 * @file main.cpp
 * @brief Entry point for the Battleship game (Human vs. AI).
 *
 */

#include "Battleship.h"
#include "colors.h"
#include "utils.cpp"
#include "board.cpp"
#include "ships.cpp"
#include "gameplay.cpp"
#include "score.cpp"

#include <iostream>
#include <cstdlib>
#include <ctime>
using namespace std;

static void initPlayer(Player &p, bool isAI)
{
    initBoard(p.board);
    initBoard(p.trackingBoard);
    initShipIndex(p.shipIndex);
    initShips(p.ships);
    p.shipsRemaining = NUM_SHIPS;
    p.score = 0;

    // AI-only fields
    if (isAI)
    {
        initShotTracker(p.shotsGuessed);
        p.hunting = false;
        p.firstHitRow = -1;
        p.firstHitCol = -1;
        p.lastHitRow = -1;
        p.lastHitCol = -1;
        p.huntAxis = 0;
        p.huntDir = 1;
    }
    else
    {
        // Zero out shotsGuessed for human too (unused but keeps memory clean)
        initShotTracker(p.shotsGuessed);
        p.hunting = false;
        p.firstHitRow = -1;
        p.firstHitCol = -1;
        p.lastHitRow = -1;
        p.lastHitCol = -1;
        p.huntAxis = 0;
        p.huntDir = 1;
    }
}

static int runGame()
{
    Player human, ai;
    initPlayer(human, false);
    initPlayer(ai, true);

    // ── Ship placement ───────────────────────────────────────────────────────
    clearScreen();
    cout << "\n=== SHIP PLACEMENT ===\n";
    cout << "  1. Place ships manually\n";
    cout << "  2. Place ships randomly\n";
    int choice = getMenuChoice(1, 2);

    if (choice == 1)
        placeShipsManual(human.board, human.shipIndex, human.ships);
    else
        placeShipsRandom(human.board, human.shipIndex, human.ships);

    placeShipsRandom(ai.board, ai.shipIndex, ai.ships);

    // ── Main game loop ───────────────────────────────────────────────────────
    int totalShots = 0;
    bool gameOver = false;
    bool playerWon = false;

    while (!gameOver)
    {
        clearScreen();
        printBothBoards(human, ai);
        displayScoreboard(human, ai, 0);

        cout << "\n"
             << CLR_SECTION << "--------------------- YOUR TURN ---------------------" << CLR_RESET << "\n";
        cout << CLR_SCORE << "  Ships remaining: You " << human.shipsRemaining << CLR_RESET
             << CLR_INFO << "  |  "
             << CLR_ENEMY_SCORE << "Enemy " << ai.shipsRemaining << CLR_RESET << "\n\n";

        // ── Human fires ──────────────────────────────────────────────────────
        int row = -1, col = -1;
        bool validShot = false;
        while (!validShot)
        {
            if (!getPlayerShot(row, col))
                continue;
            if (human.trackingBoard[row][col] == HIT ||
                human.trackingBoard[row][col] == MISS)
            {
                cout << CLR_WARNING << "  You already fired there! Choose another cell." << CLR_RESET << "\n";
                continue;
            }
            validShot = true;
        }

        int turnPoints = fireShot(ai.board, human.trackingBoard,
                                  ai.shipIndex, ai.ships, ai.shipsRemaining,
                                  row, col);
        human.score += turnPoints;
        totalShots++;

        displayScoreboard(human, ai, turnPoints);

        // ── Win check ────────────────────────────────────────────────────────
        if (checkWin(ai.shipsRemaining))
        {
            gameOver = true;
            playerWon = true;
            // FIX: pause so player can read the sinking message before wipe
            cout << CLR_PROMPT << "\nPress ENTER to see results..." << CLR_RESET;
            cin.get();
            break;
        }

        // ── AI fires ─────────────────────────────────────────────────────────
        cout << "\n"
             << CLR_ERROR << "--- ENEMY TURN ------------------------------------------" << CLR_RESET << "\n";
        aiFireShot(ai, human);
        displayScoreboard(human, ai, 0);

        // ── Lose check ───────────────────────────────────────────────────────
        if (checkWin(human.shipsRemaining))
        {
            gameOver = true;
            playerWon = false;
            // FIX: pause so player can read the AI's final shot before wipe
            cout << CLR_PROMPT << "\nPress ENTER to see results..." << CLR_RESET;
            cin.get();
            break;
        }

        cout << CLR_PROMPT << "\nPress ENTER to continue..." << CLR_RESET;
        cin.get();
    }

    // ── End-of-game display ──────────────────────────────────────────────────
    clearScreen();
    printBothBoards(human, ai);
    displayScoreboard(human, ai, 0);

    if (playerWon)
    {
        cout << "\n  " << CLR_SCORE << "+==============================+" << CLR_RESET << "\n";
        cout << "  " << CLR_SCORE << "|" << CLR_RESET << CLR_TITLE << "         ** YOU WIN! **       " << CLR_RESET << CLR_SCORE << "|" << CLR_RESET << "\n";
        cout << "  " << CLR_SCORE << "+==============================+" << CLR_RESET << "\n";
        cout << CLR_INFO << "\n  You sank all enemy ships in " << totalShots << " shots!\n";
        cout << CLR_SCORE << "  Final score: " << human.score << " points\n"
             << CLR_RESET;
    }
    else
    {
        cout << "\n  " << CLR_SUNK << "+==============================+" << CLR_RESET << "\n";
        cout << "  " << CLR_SUNK << "|" << CLR_RESET << CLR_ENEMY_SCORE << "           -- DEFEAT --       " << CLR_RESET << CLR_SUNK << "|" << CLR_RESET << "\n";
        cout << "  " << CLR_SUNK << "+==============================+" << CLR_RESET << "\n";
        cout << CLR_INFO << "\n  The enemy sank your entire fleet!\n";
        cout << CLR_SCORE << "  Final score: " << human.score << " points\n";
        cout << CLR_DIM << "  (Score not saved - win the game to set a high score!)\n"
             << CLR_RESET;
    }

    // FIX: only save high score on a win
    if (playerWon)
        return human.score;
    else
        return -1; // sentinel: tells main() not to check high score
}

int main()
{
    srand(static_cast<unsigned int>(time(nullptr)));

    bool keepPlaying = true;

    while (keepPlaying)
    {
        displayTitle();
        displayHighScore();

        cout << "\n"
             << CLR_HEADER << "=== MAIN MENU ===" << CLR_RESET << "\n";
        cout << CLR_LABEL << "  1. " << CLR_BGREEN << "New Game\n"
             << CLR_RESET;
        cout << CLR_LABEL << "  2. " << CLR_BCYAN << "How to Play\n"
             << CLR_RESET;
        cout << CLR_LABEL << "  3. " << CLR_DIM << "Quit\n"
             << CLR_RESET;
        int choice = getMenuChoice(1, 3);

        switch (choice)
        {
        case 1:
        {
            int finalScore = runGame();

            // FIX: -1 means the player lost; skip high score check
            if (finalScore >= 0)
            {
                int best;
                loadHighScore(best);
                if (finalScore > best)
                    saveHighScore(finalScore);
                else
                    cout << CLR_INFO << "\n  Best score is still " << best << " points.\n"
                         << CLR_RESET;
            }

            char again = getYesNo("Play again?");
            if (again == 'N')
                keepPlaying = false;
            break;
        }
        case 2:
            clearScreen();
            displayRules();
            cout << CLR_PROMPT << "Press ENTER to return to menu..." << CLR_RESET;
            cin.get();
            break;
        case 3:
            keepPlaying = false;
            break;
        }
    }

    clearScreen();
    cout << "\n  " << CLR_TITLE << "Thanks for playing Battleship!" << CLR_RESET << "\n\n";
    return 0;
}

/**
 * @file colors.h
 * @brief ANSI escape code constants for terminal color output.
 */

#ifndef COLORS_H
#define COLORS_H

// Reset
#define CLR_RESET       "\033[0m"

// Text styles
#define CLR_BOLD        "\033[1m"
#define CLR_DIM         "\033[2m"

// Foreground colors
#define CLR_BLACK       "\033[30m"
#define CLR_RED         "\033[31m"
#define CLR_GREEN       "\033[32m"
#define CLR_YELLOW      "\033[33m"
#define CLR_BLUE        "\033[34m"
#define CLR_MAGENTA     "\033[35m"
#define CLR_CYAN        "\033[36m"
#define CLR_WHITE       "\033[37m"

// Bright foreground colors
#define CLR_BRED        "\033[91m"
#define CLR_BGREEN      "\033[92m"
#define CLR_BYELLOW     "\033[93m"
#define CLR_BBLUE       "\033[94m"
#define CLR_BMAGENTA    "\033[95m"
#define CLR_BCYAN       "\033[96m"
#define CLR_BWHITE      "\033[97m"

// Background colors
#define BG_RED          "\033[41m"
#define BG_GREEN        "\033[42m"
#define BG_YELLOW       "\033[43m"
#define BG_BLUE         "\033[44m"
#define BG_MAGENTA      "\033[45m"
#define BG_CYAN         "\033[46m"
#define BG_WHITE        "\033[47m"
#define BG_DARK         "\033[40m"

// Bright background colors
#define BG_BRED         "\033[101m"
#define BG_BGREEN       "\033[102m"
#define BG_BYELLOW      "\033[103m"
#define BG_BBLUE        "\033[104m"
#define BG_BMAGENTA     "\033[105m"
#define BG_BCYAN        "\033[106m"

// ── Game-specific semantic colors ────────────────────────────────────────────

// Cell symbols
#define CLR_WATER       CLR_BLUE            // ~ water
#define CLR_HIT         CLR_BOLD CLR_BRED   // X hit marker
#define CLR_MISS        CLR_DIM  CLR_WHITE  // O miss marker

// Ship colors (one per ship slot 0-4)
//   0 Carrier      - bright yellow
//   1 Battleship   - bright magenta
//   2 Destroyer    - bright cyan
//   3 Submarine    - bright green
//   4 Patrol Boat  - bright white
#define CLR_SHIP_0      CLR_BOLD CLR_BYELLOW
#define CLR_SHIP_1      CLR_BOLD CLR_BMAGENTA
#define CLR_SHIP_2      CLR_BOLD CLR_BCYAN
#define CLR_SHIP_3      CLR_BOLD CLR_BGREEN
#define CLR_SHIP_4      CLR_BOLD CLR_BWHITE

// UI chrome
#define CLR_HEADER      CLR_BOLD CLR_BCYAN
#define CLR_TITLE       CLR_BOLD CLR_BYELLOW
#define CLR_LABEL       CLR_BOLD CLR_WHITE
#define CLR_BORDER      CLR_DIM  CLR_CYAN
#define CLR_SCORE       CLR_BOLD CLR_BGREEN
#define CLR_ENEMY_SCORE CLR_BOLD CLR_BRED
#define CLR_SUNK        CLR_DIM  CLR_RED
#define CLR_AFLOAT      CLR_BWHITE
#define CLR_HIT_STATUS  CLR_BYELLOW
#define CLR_PROMPT      CLR_BOLD CLR_BWHITE
#define CLR_SUCCESS     CLR_BOLD CLR_BGREEN
#define CLR_WARNING     CLR_BOLD CLR_BYELLOW
#define CLR_ERROR       CLR_BOLD CLR_BRED
#define CLR_INFO        CLR_CYAN
#define CLR_BONUS       CLR_BGREEN
#define CLR_SECTION     CLR_BOLD CLR_BBLUE

// Returns the color string for a given ship index (0-4)
inline const char* shipColor(int idx) {
    switch (idx) {
        case 0: return CLR_SHIP_0;
        case 1: return CLR_SHIP_1;
        case 2: return CLR_SHIP_2;
        case 3: return CLR_SHIP_3;
        case 4: return CLR_SHIP_4;
        default: return CLR_RESET;
    }
}

// Returns the color string for a board cell character
// shipIdx is only used when the cell holds a ship symbol
inline const char* cellColor(char c, int shipIdx = -1) {
    switch (c) {
        case '~': return CLR_WATER;
        case 'X': return CLR_HIT;
        case 'O': return CLR_MISS;
        default:  return shipColor(shipIdx);
    }
}

#endif // COLORS_H

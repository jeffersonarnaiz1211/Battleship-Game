/**
 * @file input.cpp
 * @brief Input-handling and validation helpers.
 */

#ifndef INPUT_CPP
#define INPUT_CPP

#include "Battleship.h"
#include <iostream>
#include <limits>
#include <string>
using namespace std;

bool getPlayerShot(int& row, int& col) {
    cout << "  Enter column (A-J): ";
    char colChar;
    cin >> colChar;

    if (cin.fail()) {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "  Invalid input - please enter a letter.\n";
        return false;
    }

    colChar = toupper(colChar);
    if (colChar < 'A' || colChar > 'J') {
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "  Column must be A-J.\n";
        return false;
    }

    cout << "  Enter row    (1-10): ";
    int rowNum;
    cin >> rowNum;

    if (cin.fail() || rowNum < 1 || rowNum > 10) {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "  Row must be 1-10.\n";
        return false;
    }

    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    col = colChar - 'A';
    row = rowNum  - 1;
    return true;
}

char getYesNo(const string& prompt) {
    char ch = '\0';
    while (ch != 'Y' && ch != 'N') {
        cout << prompt << " (Y/N): ";
        cin >> ch;
        if (cin.fail()) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            ch = '\0';
            continue;
        }
        ch = toupper(ch);
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        if (ch != 'Y' && ch != 'N')
            cout << "  Please enter Y or N.\n";
    }
    return ch;
}

int getMenuChoice(int min, int max) {
    int choice = -1;
    while (choice < min || choice > max) {
        cout << "  Choice (" << min << "-" << max << "): ";
        cin >> choice;
        if (cin.fail() || choice < min || choice > max) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "  Please enter a number between "
                 << min << " and " << max << ".\n";
            choice = -1;
        } else {
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }
    }
    return choice;
}

#endif // INPUT_CPP

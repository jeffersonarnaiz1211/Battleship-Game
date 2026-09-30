# ──────────────────────────────────────────────────────────────────
#  Makefile – Battleship (Human vs. AI)
#  Usage:
#    make          → build release binary
#    make debug    → build with debug symbols
#    make clean    → remove compiled files
#    make run      → build and run immediately
# ──────────────────────────────────────────────────────────────────

CXX      = g++
CXXFLAGS = -std=c++17 -Wall -Wextra
TARGET   = battleship
SRC      = main.cpp

.PHONY: all debug clean run

all:
	$(CXX) $(CXXFLAGS) -O2 -o $(TARGET) $(SRC)
	@echo "Build successful → ./$(TARGET)"

debug:
	$(CXX) $(CXXFLAGS) -g -o $(TARGET)_debug $(SRC)
	@echo "Debug build → ./$(TARGET)_debug"

run: all
	./$(TARGET)

clean:
	rm -f $(TARGET) $(TARGET)_debug highscore.txt
	@echo "Cleaned."

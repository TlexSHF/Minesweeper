#include <iostream>
#include <time.h>
#include <vector>
#include <tuple>
#include "Game.h"

/*	MINESWEEPER
	
	setup:
		A 2D array of rdmly placed bombs
		Every other index is filled with a number indicating how many bombs are near
		(For 0, hide the number)
		A "revealed" array consisting of what indices are shown to the user

	mechanics:
		When an index is chosen to [Look at], that index is put in the revealed-array.
		If index is a bomb	-> Game over'
		If index is a 0		-> All adjacent 0 are revealed (and their adjacent, etc..)
		Else do nothing

	Documenting Problems:
		- Når man løper gjennom matrisen for å åpne alle hvite ((i, j) = 0)
			må man lage en liste over hvilke ruter som allerede er sjekket,
			slik at man ikke prøver å åpne samme rute to ganger, og havner
			i en uendelig loop. 
			I denne lista burde også de andre ulovlige stedene være lagret.
		- Regel: Hvis den du åpner er hvit skal ALLE nærliggende ruter åpnes!
			(unntak: index out of bounds)
*/

/*
	Number explanation:
	>= 0	: Indicating adjacent bombs
	-1		: Bomb
	-2		: Not revealed
*/

/* TO-BE MEMBER VARIABLES:*/
/*const size_t size{9};
const size_t mines{ 10 };
std::vector<std::tuple<int, int>> opened;
int unopened{ size * size };

void fillFog(int r[size][size]) {
	for (size_t i{ 0 }; i < size; ++i) {
		for (size_t j{ 0 }; j < size; ++j) {
			r[i][j] = { -2 };
		}
	}
}

void fillMatrix(int m[size][size], size_t firstX, size_t firstY) {
	for (size_t i{ 0 }; i < mines; ++i) {
		size_t rdm1{ 0 };
		size_t rdm2{ 0 };
		do {
			rdm1 = rand() % size;
			rdm2 = rand() % size ;
		} while (rdm1 == firstY && rdm2 == firstX);

		m[rdm1][rdm2] = -1; //bomb
	}
	for (size_t i{ 0 }; i < size; ++i) {
		for (size_t j{ 0 }; j < size; ++j) {
			int adjMines{ 0 };

			//Improvement: make adjMines++ function pointer and have this as function
			if (m[i][j] != -1) {
				if (i != 0 && j != 0			&& m[i-1][j-1] == -1) adjMines++; //<^
				if (i != 0						&& m[i-1][j] == -1) adjMines++;   // ^
				if (i != 0 && j != size-1		&& m[i-1][j+1] == -1) adjMines++; //^>
				if (j != size-1					&& m[i][j+1] == -1) adjMines++;   // >
				if (j != size-1 && i != size-1	&& m[i+1][j+1] == -1) adjMines++; // >v
				if (i != size-1					&& m[i+1][j] == -1) adjMines++;   // v
				if (i != size-1 && j != 0		&& m[i+1][j-1] == -1) adjMines++; //v<
				if (j != 0						&& m[i][j-1] == -1) adjMines++;   // <

				m[i][j] = { adjMines };
			}
		}
	}
}

void printMatrix(int m[size][size]) {
	/* margin *//*
	std::cout << "  ";
	for (size_t k{ 0 }; k < size; ++k) {
		std::cout << ' ' << k << ' ';
	}
	std::cout << std::endl;
	*//* matrix *//*
	for (size_t i{ 0 }; i < size; ++i) {
		std::cout << i << ' '; //margin
		for (size_t j{ 0 }; j < size; ++j) {
			if (m[i][j] == -2) std::cout << "[-]";
			else if (m[i][j] == 0) std::cout << "[ ]";
			else if (m[i][j] == -1) std::cout << "[X]";
			else std::cout << '[' << m[i][j] << ']';
		}
		std::cout << std::endl;
	}
	std::cout << std::endl;
}

bool open(int m[size][size], int r[size][size], int i, int j) {
	if (i == -1 || j == -1 || i == size || j == size) {
		return false;
	}
	opened.push_back(std::make_tuple(i, j));
	--unopened;

	if (m[i][j] == -1) {
		return false;
	}
	r[i][j] = m[i][j];
	if (r[i][j] == 0) {*/
		//Open all 0
		//refactor this later
		/*if (i != 0 && j != 0			&& m[i-1][j-1] == 0) open(m, r, i-1, j-1); //<^
		if (i != 0						&& m[i-1][j] == 0) open(m, r, i-1, j);     // ^
		if (i != 0 && j != size-1		&& m[i-1][j+1] == 0) open(m, r, i-1, j+1); //^>
		if (j != 0						&& m[i][j-1] == 0) open(m, r, i, j-1);     // <
		if (j != size-1					&& m[i][j+1] == 0) open(m, r, i, j+1);     // >
		if (j != size-1 && i != size-1	&& m[i+1][j+1] == 0) open(m, r, i+1, j+1); // >v
		if (i != size-1					&& m[i+1][j] == 0) open(m, r, i+1, j);     // v
		if (i != size-1 && j != 0		&& m[i+1][j-1] == 0) open(m, r, i+1, j-1); //v<
		
		*//*
		for (int k{ i - 1 }; k <= (i + 1); ++k) {
			for (int l{ j - 1 }; l <= (j + 1); ++l) {
				if (!(k==i && l==j)) { //You are here
					if (find(opened.begin(), opened.end(), std::make_tuple(k, l)) == opened.end()) { //If not found in opened vector
						open(m, r, k, l);
					}
				}
			}
		}
	}
	return true;
}

int menu() {
	std::cout << "Welcome to Minesweeper!" << std::endl;
	std::cout << "Press [1] for Game. \nPress[2] for Leaderboard. \nPress [0] to exit the game" << std::endl;
	int playerChoice{ 0 };
	std::cin >> playerChoice;
	return playerChoice;
}

void leaderboard() {
	std::cout << "This is the leaderboard!" << std::endl;
	std::cout << "Press any button to go Back." << std::endl;
	char playerChoice;
	std::cin >> playerChoice;
}

void game() {
	int matrix[size][size]{ 0 }; //-1 indicates bomb
	int revealed[size][size]{ 0 }; //-2 indicates fog

	*//* setup: *//*
	fillFog(revealed);
	printMatrix(revealed);
	bool first{ true };

	*//* mechanics: *//*
	int userCoord[2];
	int userRow;
	int userCol;
	while (true) {
		size_t i{ 0 };
		do {

			if (i == 0) std::cout << "x=";
			else if (i == 1) std::cout << "y=";
			std::cin >> userCoord[i];
			if (userCoord[i] < 0 || userCoord[i] >= size) {
				std::cout << "Index out of bounds" << std::endl;
			} else if (i != 1) {
				i = 1;
			} else {
				break;
			}
		} while (true);

		if (first) {
			first = false;
			fillMatrix(matrix, userCoord[0], userCoord[1]);
		}

		std::cout << "Opening (" << userCoord[0] << ',' << userCoord[1] << "):" << std::endl;
		opened.clear();
		if (!open(matrix, revealed, userCoord[1], userCoord[0])) {
			std::cout << "YOU LOST" << std::endl;
			printMatrix(matrix);
			break;
		}
		if (unopened == mines) {
			std::cout << "YOU WON THE GAME!" << std::endl;
			printMatrix(matrix);
			break;
		}
		printMatrix(revealed);
	}
}*/

/*
bool run{ true };
	while (run==true) {
		switch (menu()) {
			case 0:
				run = false;
				break;
			case 1:
				game();
				break;
			case 2:
				leaderboard();
				break;
		}
	}
*/


int main() {
	
	srand(time(NULL));
	const size_t sz{ 10 };
	size_t mines{ 10 };
	minesweeper::Game<sz> game = minesweeper::Game<sz>(mines);
	game.begin();
}
#pragma once
#ifndef GAME
#define GAME

#include <vector>

template <const size_t size>
class Game {
public:
	Game();
	void begin();

private:
	const size_t m_mines{ 10 };
	std::vector<std::tuple<int, int>> m_opened;
	int m_unopened{ size * size };


	int menu();
	void leaderboard();
	void game();
	void fillFog(int r[size][size]);
	void fillMatrix(int m[size][size], size_t firstX, size_t firstY);
	void printMatrix(int m[size][size]);
	bool open(int m[size][size], int r[size][size], int i, int j);
};

#endif // !GAME

template<size_t size>
Game<size>::Game() {}

template<size_t size>
void Game<size>::begin() {

	bool running{ true };
	while (running) {
		switch (menu()) {
		case 0: 
			running = false;
			break;
		case 1:
			game();
			break;
		case 2:
			leaderboard();
			break;
		default:
			throw "ERROR: Unknown case from menu()";
			running = false;
			break;
		}
	}
}

template<size_t size>
int Game<size>::menu() {
	std::cout << "Welcome to Minesweeper!" << std::endl;
	std::cout << "Press [1] for Game. \nPress[2] for Leaderboard. \nPress any other button to exit the game" << std::endl;
	int playerChoice{ 0 };
	std::cin >> playerChoice;
	if (playerChoice == 1 || playerChoice == 2) return playerChoice;
	else return 0;
}

template<size_t size>
void Game<size>::leaderboard() {
	std::cout << "This is the leaderboard!" << std::endl;
	std::cout << "Press any button to go Back." << std::endl;
	char playerChoice;
	std::cin >> playerChoice;
}

template<size_t size>
void Game<size>::game() {
	int matrix[size][size]{ 0 }; //-1 indicates bomb
	int revealed[size][size]{ 0 }; //-2 indicates fog

	/* setup: */
	fillFog(revealed);
	printMatrix(revealed);
	bool first{ true };

	/* mechanics: */
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
		m_opened.clear();
		if (!open(matrix, revealed, userCoord[1], userCoord[0])) {
			std::cout << "YOU LOST" << std::endl;
			printMatrix(matrix);
			break;
		}
		if (m_unopened == m_mines) {
			std::cout << "YOU WON THE GAME!" << std::endl;
			printMatrix(matrix);
			break;
		}
		printMatrix(revealed);
	}
}

template<size_t size>
void Game<size>::fillFog(int r[size][size]) {
	for (size_t i{ 0 }; i < size; ++i) {
		for (size_t j{ 0 }; j < size; ++j) {
			r[i][j] = { -2 };
		}
	}
}

template<size_t size>
void Game<size>::fillMatrix(int m[size][size], size_t firstX, size_t firstY) {
	for (size_t i{ 0 }; i < m_mines; ++i) {
		size_t rdm1{ 0 };
		size_t rdm2{ 0 };
		do {
			rdm1 = rand() % size;
			rdm2 = rand() % size;
		} while (rdm1 == firstY && rdm2 == firstX);

		m[rdm1][rdm2] = -1; //bomb
	}
	for (size_t i{ 0 }; i < size; ++i) {
		for (size_t j{ 0 }; j < size; ++j) {
			int adjMines{ 0 };

			//Improvement: make adjMines++ function pointer and have this as function
			if (m[i][j] != -1) {
				if (i != 0 && j != 0 && m[i - 1][j - 1] == -1) adjMines++; //<^
				if (i != 0 && m[i - 1][j] == -1) adjMines++;   // ^
				if (i != 0 && j != size - 1 && m[i - 1][j + 1] == -1) adjMines++; //^>
				if (j != size - 1 && m[i][j + 1] == -1) adjMines++;   // >
				if (j != size - 1 && i != size - 1 && m[i + 1][j + 1] == -1) adjMines++; // >v
				if (i != size - 1 && m[i + 1][j] == -1) adjMines++;   // v
				if (i != size - 1 && j != 0 && m[i + 1][j - 1] == -1) adjMines++; //v<
				if (j != 0 && m[i][j - 1] == -1) adjMines++;   // <

				m[i][j] = { adjMines };
			}
		}
	}
}

template<size_t size>
void Game<size>::printMatrix(int m[size][size]) {
	/* margin */
	std::cout << "  ";
	for (size_t k{ 0 }; k < size; ++k) {
		std::cout << ' ' << k << ' ';
	}
	std::cout << std::endl;
	/* matrix */
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

template<size_t size>
bool Game<size>::open(int m[size][size], int r[size][size], int i, int j) {
	if (i == -1 || j == -1 || i == size || j == size) {
		return false;
	}
	m_opened.push_back(std::make_tuple(i, j));
	--m_unopened;

	if (m[i][j] == -1) {
		return false;
	}
	r[i][j] = m[i][j];
	if (r[i][j] == 0) {
		//Open all 0
		for (int k{ i - 1 }; k <= (i + 1); ++k) {
			for (int l{ j - 1 }; l <= (j + 1); ++l) {
				if (!(k == i && l == j)) { //You are here
					if (find(	m_opened.begin(), 
								m_opened.end(), 
								std::make_tuple(k, l)) == m_opened.end()
						) { //If not found in opened vector
						open(m, r, k, l);
					}
				}
			}
		}
	}
	return true;
}

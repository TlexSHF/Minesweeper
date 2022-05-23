#ifndef GAME_H
#define GAME_H

#include <vector>
#include "Board.h"

namespace minesweeper {

	template <const size_t size>
	class Game {
	public:
		Game(size_t size);
		void begin();

	private:
		Board<size> m_board;
		int menu();
		void leaderboard();
		void game();
	};

	template<size_t size>
	Game<size>::Game(size_t mines) :
		board() {}

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

		matrix.print('r');
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
				initMatrix(userCoord[0], userCoord[1]);
			}

			std::cout << "Opening (" << userCoord[0] << ',' << userCoord[1] << "):" << std::endl;
			m_opened.clear();
			if (!open(matrix, revealed, userCoord[1], userCoord[0])) {
				std::cout << "YOU LOST" << std::endl;
				m_board.print('m');
				break;
			}
			if (m_unopened == m_mines) {
				std::cout << "YOU WON THE GAME!" << std::endl;
				m_board.print('m');
				break;
			}
			m_board.print('r');
		}
	}
}

#endif // !GAME
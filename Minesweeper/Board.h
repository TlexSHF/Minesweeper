#ifndef BOARD_H
#define BOARD_H

namespace minesweeper {
	template <const size_t size>
	class Board {
	public:
		Board(size_t mines);
		void fillMatrix(int r[size][size], int fillNum);
		void initMatrix(size_t firstX, size_t firstY);
		void print(char matrix);
		bool open(int m[size][size], int r[size][size], int i, int j);
	private:
		int m_matrix[size][size];
		int m_revealed[size][size];
		const size_t m_mines{ 10 };
		std::vector<std::tuple<int, int>> m_opened;
		int m_unopened{ size * size };
		void printMatrix(int m[size][size]);
	};

	template<size_t size>
	Board<size>::Board(size_t mines) :
		m_mines(mines) {
		fillMatrix(m_matrix, 0)
		fillMatrix(m_revealed, -2);
	}
	
	template<size_t size>
	void Board<size>::fillMatrix(int r[size][size], int fillNum) {
		for (size_t i{ 0 }; i < size; ++i) {
			for (size_t j{ 0 }; j < size; ++j) {
				r[i][j] = { fillNum };
			}
		}
	}

	template<size_t size>
	void Board<size>::initMatrix(size_t firstX, size_t firstY) {
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
	void Board<size>::print(char matrix) {
		if (matrix == 'm') printMatrix(m_matrix);
		else if (matrix == 'r') printMatrix(m_revealed);
		else throw "ERROR matrix not known in Board<size>::print(char matrix)";
	
	}

	template<size_t size>
	void Board<size>::printMatrix(int m[size][size]) {
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
	bool Board<size>::open(int m[size][size], int r[size][size], int i, int j) {
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
						if (find(m_opened.begin(),
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
}

#endif // !BOARD_H


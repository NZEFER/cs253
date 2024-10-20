#include <iostream>
#include <vector>
#include <algorithm>
#include <limits>
#include <stack>
#include <string>

using namespace std;

const int SIZE = 8; // Размер доски 8x8
enum Cell { EMPTY, BLACK, WHITE }; // Пустая клетка, черная, белая
char symbols[] = { '.', 'B', 'W' }; // Символы для отображения на доске
const int MAX_DEPTH = 6; // Глубина поиска Минимакс

// Оценка для различных позиций на доске (углы важнее всего)
const int positionWeights[SIZE][SIZE] = {
    {100, -20, 10,  5,  5, 10, -20, 100},
    {-20, -50, -2, -2, -2, -2, -50, -20},
    { 10,  -2,  5,  1,  1,  5,  -2,  10},
    {  5,  -2,  1,  0,  0,  1,  -2,   5},
    {  5,  -2,  1,  0,  0,  1,  -2,   5},
    { 10,  -2,  5,  1,  1,  5,  -2,  10},
    {-20, -50, -2, -2, -2, -2, -50, -20},
    {100, -20, 10,  5,  5, 10, -20, 100}
};

// Функция для отображения доски
void printBoard(const vector<vector<Cell>>& board) {
    cout << "  ";
    for (int i = 0; i < SIZE; ++i)
        cout << i << " ";
    cout << endl;

    for (int i = 0; i < SIZE; ++i) {
        cout << i << " ";
        for (int j = 0; j < SIZE; ++j) {
            cout << symbols[board[i][j]] << " ";
        }
        cout << endl;
    }
}

// Проверка, находится ли координата в пределах доски
bool inBounds(int x, int y) {
    return x >= 0 && x < SIZE && y >= 0 && y < SIZE;
}

// Начальная расстановка на доске
void initBoard(vector<vector<Cell>>& board) {
    for (int i = 0; i < SIZE; ++i)
        fill(board[i].begin(), board[i].end(), EMPTY);

    // Устанавливаем начальные 4 фишки
    board[SIZE / 2 - 1][SIZE / 2 - 1] = WHITE;
    board[SIZE / 2 - 1][SIZE / 2] = BLACK;
    board[SIZE / 2][SIZE / 2 - 1] = BLACK;
    board[SIZE / 2][SIZE / 2] = WHITE;
}

// Проверка, можно ли сделать ход
bool isValidMove(const vector<vector<Cell>>& board, int x, int y, Cell player) {
    if (!inBounds(x, y) || board[x][y] != EMPTY) return false; // Клетка должна быть пустой

    Cell opponent = (player == BLACK) ? WHITE : BLACK;

    // Восемь направлений для проверки
    const int dx[] = { -1, -1, -1, 0, 1, 1, 1, 0 };
    const int dy[] = { -1, 0, 1, 1, 1, 0, -1, -1 };

    // Проверяем все направления
    for (int dir = 0; dir < 8; ++dir) {
        int nx = x + dx[dir], ny = y + dy[dir];
        bool hasOpponent = false;

        while (inBounds(nx, ny) && board[nx][ny] == opponent) {
            hasOpponent = true;
            nx += dx[dir];
            ny += dy[dir];
        }

        if (hasOpponent && inBounds(nx, ny) && board[nx][ny] == player) {
            return true; // Найден валидный ход
        }
    }

    return false;
}

// Осуществляем ход, переворачиваем фишки
void makeMove(vector<vector<Cell>>& board, int x, int y, Cell player) {
    Cell opponent = (player == BLACK) ? WHITE : BLACK;
    board[x][y] = player; // Ставим фишку игрока

    const int dx[] = { -1, -1, -1, 0, 1, 1, 1, 0 };
    const int dy[] = { -1, 0, 1, 1, 1, 0, -1, -1 };

    // Переворачиваем фишки
    for (int dir = 0; dir < 8; ++dir) {
        int nx = x + dx[dir], ny = y + dy[dir];
        vector<pair<int, int>> toFlip;

        while (inBounds(nx, ny) && board[nx][ny] == opponent) {
            toFlip.push_back({ nx, ny });
            nx += dx[dir];
            ny += dy[dir];
        }

        if (inBounds(nx, ny) && board[nx][ny] == player) {
            for (auto& p : toFlip)
                board[p.first][p.second] = player; // Переворачиваем фишки
        }
    }
}

// Поиск всех доступных ходов
vector<pair<int, int>> getValidMoves(const vector<vector<Cell>>& board, Cell player) {
    vector<pair<int, int>> validMoves;
    for (int i = 0; i < SIZE; ++i) {
        for (int j = 0; j < SIZE; ++j) {
            if (isValidMove(board, i, j, player)) {
                validMoves.push_back({ i, j });
            }
        }
    }
    return validMoves;
}

// Оценочная функция для ИИ
int evaluateBoard(const vector<vector<Cell>>& board, Cell player) {
    int score = 0;
    Cell opponent = (player == BLACK) ? WHITE : BLACK;

    for (int i = 0; i < SIZE; ++i) {
        for (int j = 0; j < SIZE; ++j) {
            if (board[i][j] == player)
                score += positionWeights[i][j];
            else if (board[i][j] == opponent)
                score -= positionWeights[i][j];
        }
    }

    return score;
}

// Алгоритм Минимакс с альфа-бета отсечением
int minimax(vector<vector<Cell>>& board, int depth, int alpha, int beta, bool maximizingPlayer, Cell player) {
    Cell opponent = (player == BLACK) ? WHITE : BLACK;

    if (depth == 0) {
        return evaluateBoard(board, player);
    }

    vector<pair<int, int>> validMoves = getValidMoves(board, maximizingPlayer ? player : opponent);
    if (validMoves.empty()) {
        return evaluateBoard(board, player);
    }

    if (maximizingPlayer) {
        int maxEval = numeric_limits<int>::min();
        for (auto move : validMoves) {
            vector<vector<Cell>> tempBoard = board;
            makeMove(tempBoard, move.first, move.second, player);
            int eval = minimax(tempBoard, depth - 1, alpha, beta, false, player);
            maxEval = max(maxEval, eval);
            alpha = max(alpha, eval);
            if (beta <= alpha)
                break;
        }
        return maxEval;
    }
    else {
        int minEval = numeric_limits<int>::max();
        for (auto move : validMoves) {
            vector<vector<Cell>> tempBoard = board;
            makeMove(tempBoard, move.first, move.second, opponent);
            int eval = minimax(tempBoard, depth - 1, alpha, beta, true, player);
            minEval = min(minEval, eval);
            beta = min(beta, eval);
            if (beta <= alpha)
                break;
        }
        return minEval;
    }
}

// Функция для нахождения лучшего хода с использованием Минимакс
pair<int, int> getBestMove(vector<vector<Cell>>& board, Cell player) {
    int bestValue = numeric_limits<int>::min();
    pair<int, int> bestMove = { -1, -1 };
    vector<pair<int, int>> validMoves = getValidMoves(board, player);

    for (auto move : validMoves) {
        vector<vector<Cell>> tempBoard = board;
        makeMove(tempBoard, move.first, move.second, player);
        int boardValue = minimax(tempBoard, MAX_DEPTH, numeric_limits<int>::min(), numeric_limits<int>::max(), false, player);

        if (boardValue > bestValue) {
            bestValue = boardValue;
            bestMove = move;
        }
    }

    return bestMove;
}

int main() {
    setlocale(LC_ALL, "RU");
    srand(time(0)); // Для генерации случайных чисел

    vector<vector<Cell>> board(SIZE, vector<Cell>(SIZE)); // Инициализация доски
    initBoard(board); // Начальная расстановка

    Cell currentPlayer = BLACK; // Черные ходят первыми (игрок)

    stack<vector<vector<Cell>>> history; // Стек для хранения истории ходов
    history.push(board); // Сохраняем начальное состояние

    while (true) {
        printBoard(board);
        vector<pair<int, int>> validMoves = getValidMoves(board, currentPlayer);

        if (validMoves.empty()) {
            cout << "Нет доступных ходов для игрока " << symbols[currentPlayer] << endl;
            currentPlayer = (currentPlayer == BLACK) ? WHITE : BLACK; // Меняем игрока
            if (getValidMoves(board, currentPlayer).empty()) {
                cout << "Игра окончена!" << endl;
                break;
            }
            continue;
        }

        if (currentPlayer == BLACK) { // Ход игрока
            cout << "Вы ходите первый, играйте за Black\n";
            cout << "Ваш ход (формат: x y), или введите 'u' для отмены хода: ";
            string input;
            cin >> input;

            if (input == "u") {
                // Проверяем, можно ли отменить ход
                if (history.size() >= 2) {
                    history.pop(); // Удаляем текущее состояние
                    board = history.top(); // Возвращаемся к предыдущему состоянию
                    history.pop(); // Удаляем состояние перед ходом ИИ
                    if (!history.empty())
                        board = history.top(); // Возвращаемся к состоянию перед ходом игрока
                    else
                        initBoard(board); // Если история пуста, инициализируем доску заново
                    cout << "Ваш ход и ход ИИ отменены." << endl;
                    continue;
                }
                else {
                    cout << "Отмена невозможна." << endl;
                    continue;
                }
            }
            else {
                int x = stoi(input);
                int y;
                cin >> y;

                if (isValidMove(board, x, y, currentPlayer)) {
                    history.push(board); // Сохраняем состояние перед ходом
                    makeMove(board, x, y, currentPlayer);
                }
                else {
                    cout << "Неправильный ход. Попробуйте снова." << endl;
                    continue;
                }
            }
        }
        else { // Ход ИИ
            cout << "Ход ИИ..." << endl;
            auto bestMove = getBestMove(board, currentPlayer);
            cout << "ИИ ходит: " << bestMove.first << " " << bestMove.second << endl;
            history.push(board); // Сохраняем состояние перед ходом ИИ
            makeMove(board, bestMove.first, bestMove.second, currentPlayer);
        }

        // Меняем игрока
        currentPlayer = (currentPlayer == BLACK) ? WHITE : BLACK;
    }

    // Подсчет фишек для определения победителя
    int blackCount = 0, whiteCount = 0;
    for (int i = 0; i < SIZE; ++i) {
        for (int j = 0; j < SIZE; ++j) {
            if (board[i][j] == BLACK) blackCount++;
            else if (board[i][j] == WHITE) whiteCount++;
        }
    }

    cout << "Черные: " << blackCount << ", Белые: " << whiteCount << endl;
    if (blackCount > whiteCount)
        cout << "Победили черные!" << endl;
    else if (whiteCount > blackCount)
        cout << "Победили белые!" << endl;
    else
        cout << "Ничья!" << endl;

    return 0;
}

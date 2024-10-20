#include <iostream>
#include <vector>
#include <algorithm>
#include <limits>
#include <stack>
#include <string>

using namespace std;

const int SIZE = 8; // ������ ����� 8x8
enum Cell { EMPTY, BLACK, WHITE }; // ������ ������, ������, �����
char symbols[] = { '.', 'B', 'W' }; // ������� ��� ����������� �� �����
const int MAX_DEPTH = 6; // ������� ������ ��������

// ������ ��� ��������� ������� �� ����� (���� ������ �����)
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

// ������� ��� ����������� �����
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

// ��������, ��������� �� ���������� � �������� �����
bool inBounds(int x, int y) {
    return x >= 0 && x < SIZE && y >= 0 && y < SIZE;
}

// ��������� ����������� �� �����
void initBoard(vector<vector<Cell>>& board) {
    for (int i = 0; i < SIZE; ++i)
        fill(board[i].begin(), board[i].end(), EMPTY);

    // ������������� ��������� 4 �����
    board[SIZE / 2 - 1][SIZE / 2 - 1] = WHITE;
    board[SIZE / 2 - 1][SIZE / 2] = BLACK;
    board[SIZE / 2][SIZE / 2 - 1] = BLACK;
    board[SIZE / 2][SIZE / 2] = WHITE;
}

// ��������, ����� �� ������� ���
bool isValidMove(const vector<vector<Cell>>& board, int x, int y, Cell player) {
    if (!inBounds(x, y) || board[x][y] != EMPTY) return false; // ������ ������ ���� ������

    Cell opponent = (player == BLACK) ? WHITE : BLACK;

    // ������ ����������� ��� ��������
    const int dx[] = { -1, -1, -1, 0, 1, 1, 1, 0 };
    const int dy[] = { -1, 0, 1, 1, 1, 0, -1, -1 };

    // ��������� ��� �����������
    for (int dir = 0; dir < 8; ++dir) {
        int nx = x + dx[dir], ny = y + dy[dir];
        bool hasOpponent = false;

        while (inBounds(nx, ny) && board[nx][ny] == opponent) {
            hasOpponent = true;
            nx += dx[dir];
            ny += dy[dir];
        }

        if (hasOpponent && inBounds(nx, ny) && board[nx][ny] == player) {
            return true; // ������ �������� ���
        }
    }

    return false;
}

// ������������ ���, �������������� �����
void makeMove(vector<vector<Cell>>& board, int x, int y, Cell player) {
    Cell opponent = (player == BLACK) ? WHITE : BLACK;
    board[x][y] = player; // ������ ����� ������

    const int dx[] = { -1, -1, -1, 0, 1, 1, 1, 0 };
    const int dy[] = { -1, 0, 1, 1, 1, 0, -1, -1 };

    // �������������� �����
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
                board[p.first][p.second] = player; // �������������� �����
        }
    }
}

// ����� ���� ��������� �����
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

// ��������� ������� ��� ��
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

// �������� �������� � �����-���� ����������
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

// ������� ��� ���������� ������� ���� � �������������� ��������
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
    srand(time(0)); // ��� ��������� ��������� �����

    vector<vector<Cell>> board(SIZE, vector<Cell>(SIZE)); // ������������� �����
    initBoard(board); // ��������� �����������

    Cell currentPlayer = BLACK; // ������ ����� ������� (�����)

    stack<vector<vector<Cell>>> history; // ���� ��� �������� ������� �����
    history.push(board); // ��������� ��������� ���������

    while (true) {
        printBoard(board);
        vector<pair<int, int>> validMoves = getValidMoves(board, currentPlayer);

        if (validMoves.empty()) {
            cout << "��� ��������� ����� ��� ������ " << symbols[currentPlayer] << endl;
            currentPlayer = (currentPlayer == BLACK) ? WHITE : BLACK; // ������ ������
            if (getValidMoves(board, currentPlayer).empty()) {
                cout << "���� ��������!" << endl;
                break;
            }
            continue;
        }

        if (currentPlayer == BLACK) { // ��� ������
            cout << "�� ������ ������, ������� �� Black\n";
            cout << "��� ��� (������: x y), ��� ������� 'u' ��� ������ ����: ";
            string input;
            cin >> input;

            if (input == "u") {
                // ���������, ����� �� �������� ���
                if (history.size() >= 2) {
                    history.pop(); // ������� ������� ���������
                    board = history.top(); // ������������ � ����������� ���������
                    history.pop(); // ������� ��������� ����� ����� ��
                    if (!history.empty())
                        board = history.top(); // ������������ � ��������� ����� ����� ������
                    else
                        initBoard(board); // ���� ������� �����, �������������� ����� ������
                    cout << "��� ��� � ��� �� ��������." << endl;
                    continue;
                }
                else {
                    cout << "������ ����������." << endl;
                    continue;
                }
            }
            else {
                int x = stoi(input);
                int y;
                cin >> y;

                if (isValidMove(board, x, y, currentPlayer)) {
                    history.push(board); // ��������� ��������� ����� �����
                    makeMove(board, x, y, currentPlayer);
                }
                else {
                    cout << "������������ ���. ���������� �����." << endl;
                    continue;
                }
            }
        }
        else { // ��� ��
            cout << "��� ��..." << endl;
            auto bestMove = getBestMove(board, currentPlayer);
            cout << "�� �����: " << bestMove.first << " " << bestMove.second << endl;
            history.push(board); // ��������� ��������� ����� ����� ��
            makeMove(board, bestMove.first, bestMove.second, currentPlayer);
        }

        // ������ ������
        currentPlayer = (currentPlayer == BLACK) ? WHITE : BLACK;
    }

    // ������� ����� ��� ����������� ����������
    int blackCount = 0, whiteCount = 0;
    for (int i = 0; i < SIZE; ++i) {
        for (int j = 0; j < SIZE; ++j) {
            if (board[i][j] == BLACK) blackCount++;
            else if (board[i][j] == WHITE) whiteCount++;
        }
    }

    cout << "������: " << blackCount << ", �����: " << whiteCount << endl;
    if (blackCount > whiteCount)
        cout << "�������� ������!" << endl;
    else if (whiteCount > blackCount)
        cout << "�������� �����!" << endl;
    else
        cout << "�����!" << endl;

    return 0;
}

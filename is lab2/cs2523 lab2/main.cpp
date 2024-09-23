#include <iostream>
#include <vector>
#include <queue>
#include <unordered_set>
#include <algorithm>
#include <cmath>
#include <chrono>
using namespace std;

// Константы для пятнашек
const int N = 4;
const int SIZE = N * N;
const vector<pair<int, int>> moves = { {-1, 0}, {1, 0}, {0, -1}, {0, 1} }; // вверх, вниз, влево, вправо
const string GOAL = "123456789ABCDEF0";

// Функция для нахождения Манхэттенского расстояния
int manhattanDistance(const string& state) {
    int dist = 0;
    for (int i = 0; i < SIZE; ++i) {
        char tile = state[i];
        if (tile != '0') {
            int val = (tile >= 'A') ? (tile - 'A' + 10) : (tile - '0');
            int goalX = (val - 1) / N;
            int goalY = (val - 1) % N;
            int currX = i / N;
            int currY = i % N;
            dist += abs(goalX - currX) + abs(goalY - currY);
        }
    }
    return dist;
}

// Проверка на решаемость
bool isSolvable(const string& state) {
    int inversions = 0;
    int zeroRow = 0;

    for (int i = 0; i < SIZE; ++i) {
        if (state[i] == '0') {
            zeroRow = i / N; // номер ряда с пустой ячейкой
            continue;
        }
        for (int j = i + 1; j < SIZE; ++j) {
            if (state[j] != '0' && state[i] > state[j]) {
                inversions++;
            }
        }
    }
    // Если размер поля нечетный, количество инверсий должно быть четным
    // Если четный, то количество инверсий должно совпадать с четностью строки пустой клетки
    return (N % 2 == 1 && inversions % 2 == 0) || (N % 2 == 0 && (inversions + zeroRow) % 2 == 1);
}

// Функция для перемещения пустой клетки
string moveTile(const string& state, int zeroPos, int newPos) {
    string newState = state;
    swap(newState[zeroPos], newState[newPos]);
    return newState;
}

// Алгоритм A* с Манхэттенской эвристикой
struct Node {
    string state;
    int g, h;
    int zeroPos;

    Node(const string& s, int g, int h, int zeroPos)
        : state(s), g(g), h(h), zeroPos(zeroPos) {}

    int f() const {
        return g + h;
    }

    bool operator>(const Node& other) const {
        return f() > other.f();
    }
};

vector<string> solveAStar(const string& start) {
    if (start == GOAL) return { GOAL };

    priority_queue<Node, vector<Node>, greater<Node>> openList;
    unordered_set<string> closedList;
    int zeroPos = start.find('0');

    openList.push(Node(start, 0, manhattanDistance(start), zeroPos));

    while (!openList.empty()) {
        Node current = openList.top();
        openList.pop();

        if (current.state == GOAL) {
            vector<string> path;
            path.push_back(GOAL);
            return path;
        }

        closedList.insert(current.state);

        for (auto& move : moves) {
            int newZeroX = current.zeroPos / N + move.first;
            int newZeroY = current.zeroPos % N + move.second;

            if (newZeroX >= 0 && newZeroX < N && newZeroY >= 0 && newZeroY < N) {
                int newZeroPos = newZeroX * N + newZeroY;
                string newState = moveTile(current.state, current.zeroPos, newZeroPos);

                if (closedList.find(newState) == closedList.end()) {
                    int h = manhattanDistance(newState);
                    openList.push(Node(newState, current.g + 1, h, newZeroPos));
                }
            }
        }
    }

    return {}; // Не решаемо
}

// Проверка позиций
void checkPositions(const vector<string>& positions) {
    for (const auto& pos : positions) {
        auto start = chrono::high_resolution_clock::now();

        cout << "Позиция: " << pos << endl;
        if (!isSolvable(pos)) {
            cout << "Нет решения" << endl;
        }
        else {
            vector<string> solution = solveAStar(pos);
            if (solution.empty()) {
                cout << "Нет решения" << endl;
            }
            else {
                //cout << "Количество ходов: " << solution.size() - 1 << endl;
            }
        }

        auto end = chrono::high_resolution_clock::now();
        chrono::duration<double> elapsed = end - start;
        cout << "Время работы: " << elapsed.count() << " секунд\n\n";
    }
}

int main() {
    setlocale(LC_ALL, "ru");
    vector<string> positions = {
        "123456789AFB0EDC", // нет решения
        "F2345678A0BE91CD", // нет решения
        "123456789ABCDEF0", // 0
        "1234067859ACDEBF", // 5
        "5134207896ACDEBF", // 8
        "16245A3709C8DEBF", // 10
        "1723068459ACDEBF", // 13
        "12345678A0BE9FCD", // 19
        "51247308A6BE9FCD", // 27
        "F2345678A0BE91DC", // 33
        "75123804A6BE9FCD", // 35
        "75AB2C416D389F0E", // 45
        "04582E1DF79BCA36", // 48
        "FE169B4C0A73D852", // 52
        "D79F2E8A45106C3B", // 55
        "DBE87A2C91F65034", // 58
        "BAC0F478E19623D5"  // 61
    };

    checkPositions(positions);

    return 0;
}

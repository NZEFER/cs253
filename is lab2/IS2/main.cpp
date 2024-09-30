#include <iostream>
#include <vector>
#include <queue>
#include <stack>
#include <unordered_set>
#include <algorithm>
#include <cmath>
#include <chrono>
#include <unordered_map>

using namespace std;

const int N = 4;
const int SIZE = N * N;
const vector<pair<int, int>> moves = { {-1, 0}, {1, 0}, {0, -1}, {0, 1} };
const string GOAL = "123456789ABCDEF0";


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

// Проверка решаемости пятнашек
bool isSolvable(const string& state) {
    int inversions = 0;
    int zeroRow = 0;

    for (int i = 0; i < SIZE; ++i) {
        if (state[i] == '0') {
            zeroRow = i / N;
            continue;
        }
        for (int j = i + 1; j < SIZE; ++j) {
            if (state[j] != '0' && state[i] > state[j]) {
                inversions++;
            }
        }
    }
    return (N % 2 == 1 && inversions % 2 == 0) || (N % 2 == 0 && (inversions + zeroRow) % 2 == 1);
}

// Перемещение плиток
string moveTile(const string& state, int zeroPos, int newPos) {
    string newState = state;
    swap(newState[zeroPos], newState[newPos]);
    return newState;
}

// Структура узла для поиска
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

// Алгоритм A*
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

    return {};
}

// Алгоритм BFS
vector<string> solveBFS(const string& start) {
    if (start == GOAL) return { GOAL };

    queue<Node> openList;
    unordered_set<string> closedList;
    int zeroPos = start.find('0');

    openList.push(Node(start, 0, 0, zeroPos));

    while (!openList.empty()) {
        Node current = openList.front();
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
                    openList.push(Node(newState, current.g + 1, 0, newZeroPos));
                }
            }
        }
    }

    return {};
}

// Алгоритм DFS
vector<string> solveDFS(const string& start, int maxDepth = 20) {
    if (start == GOAL) return { GOAL };

    stack<Node> openList;
    unordered_set<string> closedList;
    int zeroPos = start.find('0');

    openList.push(Node(start, 0, 0, zeroPos));

    while (!openList.empty()) {
        Node current = openList.top();
        openList.pop();

       
        if (current.g > maxDepth) {
            continue;
        }

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
                    openList.push(Node(newState, current.g + 1, 0, newZeroPos));
                }
            }
        }
    }

    return {};
}

// Вспомогательная функция для IDS
bool depthLimitedSearch(const string& start, int limit, unordered_set<string>& visited, unordered_map<string, int>& cache) {
    stack<Node> openList;
    int zeroPos = start.find('0');

    openList.push(Node(start, 0, 0, zeroPos));

    while (!openList.empty()) {
        Node current = openList.top();
        openList.pop();

        if (current.state == GOAL) {
            return true;
        }

        
        if (current.g < limit) {
            visited.insert(current.state);

            for (auto& move : moves) {
                int newZeroX = current.zeroPos / N + move.first;
                int newZeroY = current.zeroPos % N + move.second;

                if (newZeroX >= 0 && newZeroX < N && newZeroY >= 0 && newZeroY < N) {
                    int newZeroPos = newZeroX * N + newZeroY;
                    string newState = moveTile(current.state, current.zeroPos, newZeroPos);

                    // Проверяем, не были ли уже в этом состоянии с меньшей глубиной
                    if (visited.find(newState) == visited.end() || (cache.find(newState) != cache.end() && cache[newState] > current.g + 1)) {
                        cache[newState] = current.g + 1;
                        openList.push(Node(newState, current.g + 1, 0, newZeroPos));
                    }
                }
            }
        }
    }

    return false;
}

// Алгоритм IDS с кэшированием
vector<string> solveIDS(const string& start) {
    int limit = 1;
    unordered_map<string, int> cache;

    while (true) {
        unordered_set<string> visited;
        if (depthLimitedSearch(start, limit, visited, cache)) {
            return { GOAL }; 
        }
        limit++;
    }
}

unordered_set<string> visitedStates;  
const int MAX_DEPTH = 100;  

int idaStarSearch(const string& state, int g, int bound, int zeroPos, vector<string>& path) {
    if (visitedStates.find(state) != visitedStates.end()) return INT_MAX;  
    if (g > MAX_DEPTH) return INT_MAX;  

    visitedStates.insert(state);

    int h = manhattanDistance(state);
    int f = g + h;

    if (f > bound) return f;  
    if (state == GOAL) return -1;  

    int minThreshold = INT_MAX;

    for (auto& move : moves) {
        int newZeroX = zeroPos / N + move.first;
        int newZeroY = zeroPos % N + move.second;

        if (newZeroX >= 0 && newZeroX < N && newZeroY >= 0 && newZeroY < N) {
            int newZeroPos = newZeroX * N + newZeroY;
            string newState = moveTile(state, zeroPos, newZeroPos);

            int t = idaStarSearch(newState, g + 1, bound, newZeroPos, path);
            if (t == -1) {
                path.push_back(newState);
                return -1;
            }
            minThreshold = min(minThreshold, t);
        }
    }

    return minThreshold;
}

vector<string> solveIDAStar(const string& start) {
    vector<string> path;
    int bound = manhattanDistance(start);
    int zeroPos = start.find('0');

    while (true) {
        visitedStates.clear();  // Очищаем кэш перед каждой итерацией
        int t = idaStarSearch(start, 0, bound, zeroPos, path);
        if (t == -1) {
            path.push_back(GOAL);
            reverse(path.begin(), path.end());
            return path;
        }
        if (t == INT_MAX) return {};
        bound = t;
    }
}

void checkPositions(const vector<string>& positions, const string& method = "A*") {
    for (const auto& pos : positions) {
        cout << "Позиция: " << pos << endl;

        if (!isSolvable(pos)) {
            cout << "Нет решения\n";
            continue;
        }

        vector<string> solution;
        visitedStates.clear();  // Очищаем кэш для каждой новой задачи
        auto start = chrono::high_resolution_clock::now();

        if (method == "A*") solution = solveAStar(pos);
        else if (method == "BFS") solution = solveBFS(pos);
        else if (method == "DFS") solution = solveDFS(pos);
        else if (method == "IDS") solution = solveIDS(pos);
        else if (method == "IDA*") solution = solveIDAStar(pos);

        auto end = chrono::high_resolution_clock::now();
        chrono::duration<double> elapsed = end - start;

        if (solution.empty()) {
            cout << "Нет решения\n";
        }
        else {
        //    cout << "Количество ходов: " << solution.size() - 1 << endl;
        }

        //cout << "Количество проверенных состояний: " << visitedStates.size() << endl;
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

    // "A*", "BFS", "DFS", "IDS", "IDA*"
    string method = "DFS";
    checkPositions(positions, method);

    return 0;
}

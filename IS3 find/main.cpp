#include <iostream>
#include <vector>
#include <queue>
#include <unordered_set>
#include <algorithm>
#include <tuple>
#include <chrono> // дл€ измерени€ времени
#include <string>

using namespace std;
using namespace std::chrono;

//  онстанты дл€ размера головоломки
const int NUM_DISKS = 5; // количество дисков
const int NUM_COLORS = 5; // количество цветов

// —труктура дл€ представлени€ состо€ни€ цилиндра
struct State {
    vector<vector<int>> cylinder; // состо€ние цилиндра, где cylinder[i][j] Ч шарик в прорези i на диске j
    int empty_disk; // индекс диска, где есть вырез
    int g; // стоимость пути до этого состо€ни€ (глубина поиска)
    int h; // эвристическа€ оценка
    vector<int> moves; // последовательность ходов

    // ќператор сравнени€ дл€ приоритетной очереди (нужно дл€ A*)
    bool operator>(const State& other) const {
        return g + h > other.g + other.h;
    }
};

// ÷елевое состо€ние: каждый диск содержит один цвет
vector<vector<int>> goal_state = {
    {0, 0, 0, 0, 0}, // диск 1
    {1, 1, 1, 1, 1}, // диск 2
    {2, 2, 2, 2, 2}, // диск 3
    {3, 3, 3, 3, 3}, // диск 4
    {4, 4, 4, 4, 4}  // диск 5
};

// Ёвристическа€ функци€: количество шариков, которые не на своих местах
int heuristic(const vector<vector<int>>& cylinder) {
    int misplaced = 0;
    for (int i = 0; i < NUM_DISKS; ++i) {
        for (int j = 0; j < NUM_COLORS; ++j) {
            if (cylinder[i][j] != goal_state[i][j]) {
                ++misplaced;
            }
        }
    }
    return misplaced;
}

// √енераци€ возможных соседних состо€ний
vector<State> get_neighbors(const State& state) {
    vector<State> neighbors;

    // ѕеремещение шариков между соседними дисками через вырез
    for (int shift = -1; shift <= 1; shift += 2) {
        int new_empty_disk = (state.empty_disk + shift + NUM_DISKS) % NUM_DISKS;

        //  опируем текущее состо€ние
        State new_state = state;

        // ѕеремещаем шарик с "нового" диска через вырез
        swap(new_state.cylinder[state.empty_disk][0], new_state.cylinder[new_empty_disk][0]);

        new_state.empty_disk = new_empty_disk; // обновл€ем индекс диска с вырезом
        new_state.g = state.g + 1; // увеличение стоимости
        new_state.moves.push_back(new_empty_disk); // запоминаем ход

        // ќбновл€ем эвристику
        new_state.h = heuristic(new_state.cylinder);

        neighbors.push_back(new_state);
    }

    return neighbors;
}

// ѕреобразуем состо€ние в строку дл€ хранени€ в set
string state_to_string(const vector<vector<int>>& cylinder) {
    string key;
    for (const auto& disk : cylinder) {
        for (int ball : disk) {
            key += to_string(ball) + ",";
        }
    }
    return key;
}

// A* дл€ поиска минимального пути
bool solve_astar(const vector<vector<int>>& start_state) {
    priority_queue<State, vector<State>, greater<State>> open_list;
    unordered_set<string> closed_set;

    State start = { start_state, 0, 0, heuristic(start_state), {} };
    open_list.push(start);

    auto start_time = high_resolution_clock::now(); // Ќачало измерени€ времени

    int max_depth = 0; // √лубина поиска

    while (!open_list.empty()) {
        State current = open_list.top();
        open_list.pop();

        // ѕроверка, если состо€ние Ч целевое
        if (current.cylinder == goal_state) {
            auto end_time = high_resolution_clock::now(); //  онец измерени€ времени
            auto duration = duration_cast<milliseconds>(end_time - start_time).count();

            cout << "–ешение найдено!\n";
            cout << " оличество ходов: " << current.g << endl;
            cout << "√лубина поиска: " << current.g << endl;
            cout << "ѕоследовательность ходов (индексы дисков): ";
            for (int move : current.moves) {
                cout << move << " ";
            }
            cout << endl;
            cout << "¬рем€ выполнени€: " << duration << " мс" << endl;
            return true;
        }

        // ќбновл€ем максимальную глубину
        max_depth = max(max_depth, current.g);

        // √енераци€ строкового ключа дл€ проверки уже посещенных состо€ний
        string key = state_to_string(current.cylinder);

        if (closed_set.find(key) != closed_set.end()) {
            continue; // если это состо€ние уже посещалось, пропускаем его
        }

        closed_set.insert(key);

        // ѕолучаем соседние состо€ни€
        vector<State> neighbors = get_neighbors(current);
        for (const State& neighbor : neighbors) {
            open_list.push(neighbor);
        }
    }

    cout << "–ешение не найдено!" << endl;
    return false;
}

int main() {
    setlocale(LC_ALL, "ru");
    // »сходное состо€ние цилиндра (пример)
    vector<vector<int>> start_state = {
        {0, 0, 0, 0, 0}, // диск 1
        {1, 1, 1, 1, 1}, // диск 2
        {2, 2, 2, 2, 2}, // диск 3
        {3, 3, 3, 3, 3}, // диск 4
        {4, 4, 4, 4, 4}  // диск 5
    };

    // –ешение головоломки
    solve_astar(start_state);

    return 0;
}

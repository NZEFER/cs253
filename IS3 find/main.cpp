#include <iostream>
#include <vector>
#include <queue>
#include <unordered_set>
#include <algorithm>
#include <tuple>
#include <chrono> // ��� ��������� �������
#include <string>

using namespace std;
using namespace std::chrono;

// ��������� ��� ������� �����������
const int NUM_DISKS = 5; // ���������� ������
const int NUM_COLORS = 5; // ���������� ������

// ��������� ��� ������������� ��������� ��������
struct State {
    vector<vector<int>> cylinder; // ��������� ��������, ��� cylinder[i][j] � ����� � ������� i �� ����� j
    int empty_disk; // ������ �����, ��� ���� �����
    int g; // ��������� ���� �� ����� ��������� (������� ������)
    int h; // ������������� ������
    vector<int> moves; // ������������������ �����

    // �������� ��������� ��� ������������ ������� (����� ��� A*)
    bool operator>(const State& other) const {
        return g + h > other.g + other.h;
    }
};

// ������� ���������: ������ ���� �������� ���� ����
vector<vector<int>> goal_state = {
    {0, 0, 0, 0, 0}, // ���� 1
    {1, 1, 1, 1, 1}, // ���� 2
    {2, 2, 2, 2, 2}, // ���� 3
    {3, 3, 3, 3, 3}, // ���� 4
    {4, 4, 4, 4, 4}  // ���� 5
};

// ������������� �������: ���������� �������, ������� �� �� ����� ������
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

// ��������� ��������� �������� ���������
vector<State> get_neighbors(const State& state) {
    vector<State> neighbors;

    // ����������� ������� ����� ��������� ������� ����� �����
    for (int shift = -1; shift <= 1; shift += 2) {
        int new_empty_disk = (state.empty_disk + shift + NUM_DISKS) % NUM_DISKS;

        // �������� ������� ���������
        State new_state = state;

        // ���������� ����� � "������" ����� ����� �����
        swap(new_state.cylinder[state.empty_disk][0], new_state.cylinder[new_empty_disk][0]);

        new_state.empty_disk = new_empty_disk; // ��������� ������ ����� � �������
        new_state.g = state.g + 1; // ���������� ���������
        new_state.moves.push_back(new_empty_disk); // ���������� ���

        // ��������� ���������
        new_state.h = heuristic(new_state.cylinder);

        neighbors.push_back(new_state);
    }

    return neighbors;
}

// ����������� ��������� � ������ ��� �������� � set
string state_to_string(const vector<vector<int>>& cylinder) {
    string key;
    for (const auto& disk : cylinder) {
        for (int ball : disk) {
            key += to_string(ball) + ",";
        }
    }
    return key;
}

// A* ��� ������ ������������ ����
bool solve_astar(const vector<vector<int>>& start_state) {
    priority_queue<State, vector<State>, greater<State>> open_list;
    unordered_set<string> closed_set;

    State start = { start_state, 0, 0, heuristic(start_state), {} };
    open_list.push(start);

    auto start_time = high_resolution_clock::now(); // ������ ��������� �������

    int max_depth = 0; // ������� ������

    while (!open_list.empty()) {
        State current = open_list.top();
        open_list.pop();

        // ��������, ���� ��������� � �������
        if (current.cylinder == goal_state) {
            auto end_time = high_resolution_clock::now(); // ����� ��������� �������
            auto duration = duration_cast<milliseconds>(end_time - start_time).count();

            cout << "������� �������!\n";
            cout << "���������� �����: " << current.g << endl;
            cout << "������� ������: " << current.g << endl;
            cout << "������������������ ����� (������� ������): ";
            for (int move : current.moves) {
                cout << move << " ";
            }
            cout << endl;
            cout << "����� ����������: " << duration << " ��" << endl;
            return true;
        }

        // ��������� ������������ �������
        max_depth = max(max_depth, current.g);

        // ��������� ���������� ����� ��� �������� ��� ���������� ���������
        string key = state_to_string(current.cylinder);

        if (closed_set.find(key) != closed_set.end()) {
            continue; // ���� ��� ��������� ��� ����������, ���������� ���
        }

        closed_set.insert(key);

        // �������� �������� ���������
        vector<State> neighbors = get_neighbors(current);
        for (const State& neighbor : neighbors) {
            open_list.push(neighbor);
        }
    }

    cout << "������� �� �������!" << endl;
    return false;
}

int main() {
    setlocale(LC_ALL, "ru");
    // �������� ��������� �������� (������)
    vector<vector<int>> start_state = {
        {0, 0, 0, 0, 0}, // ���� 1
        {1, 1, 1, 1, 1}, // ���� 2
        {2, 2, 2, 2, 2}, // ���� 3
        {3, 3, 3, 3, 3}, // ���� 4
        {4, 4, 4, 4, 4}  // ���� 5
    };

    // ������� �����������
    solve_astar(start_state);

    return 0;
}

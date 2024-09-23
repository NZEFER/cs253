#include <iostream>
#include <queue>
#include <unordered_map>
#include <vector>
#include <chrono>
#include <windows.h>

using namespace std;
using namespace std::chrono;


int task1(int start, int target) {
    if (start == target) return 0; 

    queue<pair<int, int>> q;
    unordered_map<int, int> visited; 

    q.push({ start, 0 });
    visited[start] = 0;

    while (!q.empty()) {
        pair<int, int> front = q.front(); 
        int current = front.first;        
        int steps = front.second;         
        q.pop();

        if (current == target) {
            return steps; // достигли целевого числа
        }

        int add3 = current + 3;
        if (add3 <= target + 3 && visited.find(add3) == visited.end()) {
            q.push({ add3, steps + 1 });
            visited[add3] = steps + 1;
        }

        int mul2 = current * 2;
        if (mul2 <= target * 2 && visited.find(mul2) == visited.end()) {
            q.push({ mul2, steps + 1 });
            visited[mul2] = steps + 1;
        }
    }

    return -1; //решения нет
}

int task2(int start, int target) {
    if (start == target) return 0; //начальное число равно целевому

    queue<pair<int, int>> q; 
    unordered_map<int, int> visited; 

    q.push({ start, 0 });
    visited[start] = 0;

    while (!q.empty()) {
        pair<int, int> front = q.front(); 
        int current = front.first;        
        int steps = front.second;         
        q.pop();

        if (current == target) {
            return steps; //достигли целевого числа
        }

        int add3 = current + 3;
        if (add3 <= target + 3 && visited.find(add3) == visited.end()) {
            q.push({ add3, steps + 1 });
            visited[add3] = steps + 1;
        }


        int mul2 = current * 2;
        if (mul2 <= target * 2 && visited.find(mul2) == visited.end()) {
            q.push({ mul2, steps + 1 });
            visited[mul2] = steps + 1;
        }


        int sub2 = current - 2;
        if (sub2 >= 0 && visited.find(sub2) == visited.end()) {
            q.push({ sub2, steps + 1 });
            visited[sub2] = steps + 1;
        }
    }

    return -1; // решения нет
}

int task3(int start, int target) {
    if (start == target) return 0; 

    queue<pair<int, int>> q;
    unordered_map<int, int> visited;

    q.push({ target, 0 });
    visited[target] = 0;

    while (!q.empty()) {
        pair<int, int> front = q.front(); 
        int current = front.first;        
        int steps = front.second;       
        q.pop();

        if (current == start) {
            return steps;
        }

        int sub3 = current - 3;
        if (sub3 >= start && visited.find(sub3) == visited.end()) {
            q.push({ sub3, steps + 1 });
            visited[sub3] = steps + 1;
        }

        if (current % 2 == 0) {
            int div2 = current / 2;
            if (div2 >= start && visited.find(div2) == visited.end()) {
                q.push({ div2, steps + 1 });
                visited[div2] = steps + 1;
            }
        }
    }

    return -1; //решения нет
}



int task4(int start, int target) {
    if (start == target) return 0; 

    queue<pair<int, int>> q_start, q_target;
    unordered_map<int, int> visited_start, visited_target;

    q_start.push({ start, 0 });
    visited_start[start] = 0;

    q_target.push({ target, 0 });
    visited_target[target] = 0;

    while (!q_start.empty() && !q_target.empty()) {
        // BFS от начального числа
        pair<int, int> current_pair_start = q_start.front();  
        int current_start = current_pair_start.first;          
        int steps_start = current_pair_start.second;           
        q_start.pop();

        // нашли пересечение с целевым числом
        if (visited_target.find(current_start) != visited_target.end()) {
            return steps_start + visited_target[current_start];
        }

        int add3_start = current_start + 3;
        if (add3_start <= target * 2 && visited_start.find(add3_start) == visited_start.end()) {
            q_start.push({ add3_start, steps_start + 1 });
            visited_start[add3_start] = steps_start + 1;
        }

        int mul2_start = current_start * 2;
        if (mul2_start <= target * 2 && visited_start.find(mul2_start) == visited_start.end()) {
            q_start.push({ mul2_start, steps_start + 1 });
            visited_start[mul2_start] = steps_start + 1;
        }

        // BFS от целевого числа
        pair<int, int> current_pair_target = q_target.front();  
        int current_target = current_pair_target.first;          
        int steps_target = current_pair_target.second;           
        q_target.pop();

        // нашли пересечение с начальным числом
        if (visited_start.find(current_target) != visited_start.end()) {
            return steps_target + visited_start[current_target];
        }

        int sub3_target = current_target - 3;
        if (sub3_target >= 0 && visited_target.find(sub3_target) == visited_target.end()) {
            q_target.push({ sub3_target, steps_target + 1 });
            visited_target[sub3_target] = steps_target + 1;
        }

        if (current_target % 2 == 0) {
            int div2_target = current_target / 2;
            if (visited_target.find(div2_target) == visited_target.end()) {
                q_target.push({ div2_target, steps_target + 1 });
                visited_target[div2_target] = steps_target + 1;
            }
        }
    }

    return -1; //решения нет
}



int main() {
    setlocale(LC_ALL, "RUSSIAN");
    int start = 2;
    int target = 10000001;

    auto start_time = high_resolution_clock::now();

    int result = task2(start, target);
    auto end_time = high_resolution_clock::now();

    cout << "Минимальное количество шагов: " << result << endl;

    //время работы
    auto duration = duration_cast<milliseconds>(end_time - start_time);
    cout << "Время работы: " << duration.count() << " мс" << endl;

    return 0;
}

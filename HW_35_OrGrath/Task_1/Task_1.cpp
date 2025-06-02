#include <iostream>
#include <fstream>
#include <string>

using namespace std;

int main() {
    setlocale(LC_ALL, "Russian");
    ifstream input("input.txt");
    if (!input.is_open()) {
        cerr << "Не удалось открыть файл input.txt" << endl;
        return 1;
    }

    int N;
    input >> N;

    // Динамический двумерный массив для матрицы смежности
    int** adjacencyMatrix = new int* [N];
    for (int i = 0; i < N; ++i) {
        adjacencyMatrix[i] = new int[N];
        for (int j = 0; j < N; ++j) {
            input >> adjacencyMatrix[i][j];
        }
    }
    input.close();

    cout << "Текстовый вид орграфа:" << endl;
    for (int i = 0; i < N; ++i) {
        int vertex = i + 1;  // Вершины нумеруются с 1
        int* targets = new int[N];  // Массив для хранения достижимых вершин
        int count = 0;  // Количество достижимых вершин

        // Проверяем все возможные целевые вершины
        for (int j = 0; j < N; ++j) {
            if (adjacencyMatrix[i][j] == 1) {
                targets[count++] = j + 1;  // Нумерация с 1
            }
        }

        // Формируем вывод
        cout << vertex << ": ";
        if (count == 0) {
            cout << "нет";
        }
        else {
            for (int k = 0; k < count; ++k) {
                if (k != 0) cout << " ";
                cout << targets[k];
            }
        }
        cout << endl;

        delete[] targets;  
    }

    for (int i = 0; i < N; ++i) {
        delete[] adjacencyMatrix[i];
    }
    delete[] adjacencyMatrix;

    return 0;
}
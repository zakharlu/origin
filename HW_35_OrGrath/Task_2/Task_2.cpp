#include <iostream>
#include <fstream>

using namespace std;

void topologicalSortUtil(int v, bool* visited, int* stack, int& stackIndex, int** adj, int N) {
    visited[v] = true;

    for (int i = 0; i < N; ++i) {
        if (adj[v][i] == 1 && !visited[i]) {
            topologicalSortUtil(i, visited, stack, stackIndex, adj, N);
        }
    }

    stack[stackIndex++] = v + 1; // +1 потому что вершины нумеруются с 1
}

void topologicalSort(int** adj, int N) {
    int* stack = new int[N];
    int stackIndex = 0;
    bool* visited = new bool[N];

    for (int i = 0; i < N; ++i) {
        visited[i] = false;
    }

    for (int i = 0; i < N; ++i) {
        if (!visited[i]) {
            topologicalSortUtil(i, visited, stack, stackIndex, adj, N);
        }
    }

    cout << "Топологический порядок вершин:";
    for (int i = stackIndex - 1; i >= 0; --i) {
        cout << " " << stack[i];
    }
    cout << endl;

    delete[] stack;
    delete[] visited;
}

int main() {
    setlocale(LC_ALL, "Russian");
    ifstream inputFile("input.txt");
    if (!inputFile.is_open()) {
        cerr << "Не удалось открыть файл input.txt" << endl;
        return 1;
    }

    int N;
    inputFile >> N;

    int** adj = new int* [N];
    for (int i = 0; i < N; ++i) {
        adj[i] = new int[N];
        for (int j = 0; j < N; ++j) {
            inputFile >> adj[i][j];
        }
    }

    topologicalSort(adj, N);

    for (int i = 0; i < N; ++i) {
        delete[] adj[i];
    }
    delete[] adj;

    return 0;
}
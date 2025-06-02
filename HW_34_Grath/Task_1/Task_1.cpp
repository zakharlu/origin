#include <iostream>
#include <fstream>

using namespace std;

void DFS(int** adjacencyMatrix, bool* visited, int N, int startVertex) {
    int* stack = new int[N];
    int stackSize = 0;

    stack[stackSize++] = startVertex;
    visited[startVertex - 1] = true;
    cout << startVertex << " ";

    while (stackSize > 0) {
        int currentVertex = stack[stackSize - 1];
        bool foundUnvisited = false;

        for (int neighbor = 0; neighbor < N; ++neighbor) {
            if (adjacencyMatrix[currentVertex - 1][neighbor] == 1 && !visited[neighbor]) {
                stack[stackSize++] = neighbor + 1;
                visited[neighbor] = true;
                cout << neighbor + 1 << " ";
                foundUnvisited = true;
                break;
            }
        }

        if (!foundUnvisited) {
            stackSize--;
        }
    }

    delete[] stack;
}

int main() {
    setlocale(LC_ALL, "Russian");
    ifstream inputFile("input.txt");
    if (!inputFile) {
        cerr << "Unable to open file input.txt";
        return 1;
    }

    int N;
    inputFile >> N;

    int** adjacencyMatrix = new int* [N];
    for (int i = 0; i < N; ++i) {
        adjacencyMatrix[i] = new int[N];
        for (int j = 0; j < N; ++j) {
            inputFile >> adjacencyMatrix[i][j];
        }
    }

    bool* visited = new bool[N];
    for (int i = 0; i < N; ++i) {
        visited[i] = false;
    }

    cout << "Порядок обхода вершин: ";
    DFS(adjacencyMatrix, visited, N, 1);

    for (int i = 0; i < N; ++i) {
        delete[] adjacencyMatrix[i];
    }
    delete[] adjacencyMatrix;
    delete[] visited;

    inputFile.close();
    return 0;
}

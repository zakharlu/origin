#include <iostream>
#include <map>
#include <vector>
#include <algorithm>
#include <string> 

using namespace std;

// Функция для сравнения пар (символ, частота) по убыванию частоты
bool comparePairs(const pair<char, int>& a, const pair<char, int>& b) {
    if (a.second == b.second) {
        return a.first < b.first;  // Если частоты равны, сортируем по символу
    }
    return a.second > b.second;
}

int main() {
    setlocale(LC_ALL, "Russian");
    string text;
    cout << "Введите текст: ";  
    getline(cin, text);        

    map<char, int> charCount;

    // Подсчёт количества каждого символа
    for (char c : text) {
        charCount[c]++;
    }

    // Переносим в вектор для сортировки
    vector<pair<char, int>> charVec(charCount.begin(), charCount.end());

       // Сортируем по убыванию частоты (и по символу, если частоты равны)
    sort(charVec.begin(), charVec.end(), comparePairs);

    // Выводим результат
    cout << "\nРезультат:\n";
    for (const auto& pair : charVec) {
        cout << "'" << pair.first << "': " << pair.second << endl;
    }

    return 0;
}
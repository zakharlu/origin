#include <iostream>

using namespace std;

void print_dynamic_array(int* arr, int logical_size, int actual_size) {
    cout << "Динамический массив: ";
    for (int i = 0; i < actual_size; ++i) {
        if (i < logical_size) {
            cout << arr[i];
        }
        else {
            cout << "_";
        }
        if (i != actual_size - 1) {
            cout << " ";
        }
    }
    cout << endl;
}

int main() {
    setlocale(LC_ALL, "Russian");
    int actual_size, logical_size;

    cout << "Введите фактический размер массива: ";
    cin >> actual_size;

    cout << "Введите логический размер массива: ";
    cin >> logical_size;

    if (logical_size > actual_size) {
        cout << "Ошибка! Логический размер массива не может превышать фактический!" << endl;
        return 1;
    }

    int* arr = new int[actual_size];

    for (int i = 0; i < logical_size; ++i) {
        cout << "Введите arr[" << i << "]: ";
        cin >> arr[i];
    }

    print_dynamic_array(arr, logical_size, actual_size);

    delete[] arr;

    return 0;
}
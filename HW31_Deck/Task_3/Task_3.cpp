#include <iostream>
#include <string>

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

int* remove_dynamic_array_head(int* arr, int& logical_size, int& actual_size) {
    if (logical_size == 0) {
        return arr;
    }

    int new_logical_size = logical_size - 1;

       // Проверяем, нужно ли уменьшать массив
    if (new_logical_size > actual_size / 3) {
        // Просто сдвигаем элементы
        for (int i = 0; i < new_logical_size; ++i) {
            arr[i] = arr[i + 1];
        }
        logical_size = new_logical_size;
        return arr;
    }
    else {
        // Уменьшаем массив в 3 раза
        int new_actual_size = actual_size / 3;
        if (new_actual_size < 1) new_actual_size = 1;

        int* new_arr = new int[new_actual_size];

        // Копируем элементы, пропуская первый
        for (int i = 0; i < new_logical_size; ++i) {
            new_arr[i] = arr[i + 1];
        }

        logical_size = new_logical_size;
        actual_size = new_actual_size;

        delete[] arr;
        return new_arr;
    }
}

int main() {
    int actual_size, logical_size;

    setlocale(LC_ALL, "Russian");
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

    // Уддаляем элементы
    while (true) {
        if (logical_size == 0) {
            cout << "Невозможно удалить первый элемент, так как массив пустой. До свидания!" << endl;
            break;
        }

        string answer;
        cout << "Удалить первый элемент? (yes/no): ";
        cin >> answer;

        if (answer == "no") {
            cout << "Спасибо! Ваш динамический массив: ";
            print_dynamic_array(arr, logical_size, actual_size);
            break;
        }
        else if (answer == "yes") {
            arr = remove_dynamic_array_head(arr, logical_size, actual_size);
            print_dynamic_array(arr, logical_size, actual_size);
        }
        else {
            cout << "Пожалуйста, введите 'yes' или 'no'." << endl;
        }
    }

    delete[] arr;

    return 0;
}
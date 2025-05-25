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

int* append_to_dynamic_array(int* arr, int& logical_size, int& actual_size, int new_element) {
    if (logical_size < actual_size) {
        // Есть место, просто добавляем элемент
        arr[logical_size] = new_element;
        logical_size++;
        return arr;
    }
    else {
        // Нужно расширить массив
        int new_actual_size = actual_size * 2;
        int* new_arr = new int[new_actual_size];

        // Копируем старые элементы
        for (int i = 0; i < logical_size; ++i) {
            new_arr[i] = arr[i];
        }

        // Добавляем новый элемент
        new_arr[logical_size] = new_element;
        logical_size++;
        actual_size = new_actual_size;

        // Удаляем старый массив
        delete[] arr;

        return new_arr;
    }
}

int main() {
    setlocale(LC_ALL, "Russian");
    int actual_size, logical_size;

    // Ввод размеров массива
    cout << "Введите фактический размер массива: ";
    cin >> actual_size;

    cout << "Введите логический размер массива: ";
    cin >> logical_size;

    if (logical_size > actual_size) {
        cout << "Ошибка! Логический размер массива не может превышать фактический!" << endl;
        return 1;
    }

    // Создание и заполнение массива
    int* arr = new int[actual_size];

    for (int i = 0; i < logical_size; ++i) {
        cout << "Введите arr[" << i << "]: ";
        cin >> arr[i];
    }

    // Вывод начального массива
    print_dynamic_array(arr, logical_size, actual_size);

    // Цикл добавления элементов
    while (true) {
        int new_element;
        cout << "Введите элемент для добавления (0 для выхода): ";
        cin >> new_element;

        if (new_element == 0) {
            break;
        }

        arr = append_to_dynamic_array(arr, logical_size, actual_size, new_element);
        print_dynamic_array(arr, logical_size, actual_size);
    }

    // Финальный вывод массива
    cout << "Спасибо! Ваш массив: ";
    print_dynamic_array(arr, logical_size, actual_size);

    // Освобождение памяти
    delete[] arr;

    return 0;
}
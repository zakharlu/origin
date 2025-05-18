#include <iostream>

void merge_sort(int* arr, int size);

int main() {
    int size;
    setlocale(LC_ALL, "Russian");

    std::cout << "Введите размер массива: ";
    std::cin >> size;


    int* arr = new int[size];


    std::cout << "Введите массив: ";
    for (int i = 0; i < size; i++) {
        std::cin >> arr[i];
    }

    std::cout << "Массив до сортировки: ";
    for (int i = 0; i < size; i++) {
        std::cout << arr[i] << " ";
    }

    std::cout << std::endl;

    merge_sort(arr, size);

    std::cout << "Отсортированный массив :";
    for (int i = 0; i < size; i++) {
        std::cout << arr[i] << " ";
    }
}


void merge_sort(int* arr, int size) {
    if (arr == nullptr || size <= 1) {
        return; // Нечего сортировать
    }

    // Временный буфер для слияния
    int* temp = new int[size];

    // Начинаем с сортировки подмассивов длины 1, затем удваиваем размер
    for (int block_size = 1; block_size < size; block_size *= 2) {
        for (int left = 0; left < size; left += 2 * block_size) {
            int mid = std::min(left + block_size - 1, size - 1);
            int right = std::min(left + 2 * block_size - 1, size - 1);

            // Слияние двух подмассивов arr[left..mid] и arr[mid+1..right]
            int i = left, j = mid + 1, k = left;
            while (i <= mid && j <= right) {
                if (arr[i] <= arr[j]) {
                    temp[k++] = arr[i++];
                }
                else {
                    temp[k++] = arr[j++];
                }
            }

            // Дописываем оставшиеся элементы (если есть)
            while (i <= mid) {
                temp[k++] = arr[i++];
            }
            while (j <= right) {
                temp[k++] = arr[j++];
            }

            // Копируем обратно в исходный массив
            for (int p = left; p <= right; p++) {
                arr[p] = temp[p];
            }
        }
    }

    delete[] temp; // Освобождаем память
}
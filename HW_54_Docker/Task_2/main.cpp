#include <iostream>
#include <vector>
#include <future>
#include <algorithm>

std::future<std::size_t> asyncFindMin(const std::vector<int>& arr, std::size_t start) {
    auto promise = std::make_shared<std::promise<std::size_t>>();
    std::future<std::size_t> future = promise->get_future();

    std::thread([promise, &arr, start]() {
        std::size_t minIndex = start;
        for (std::size_t i = start + 1; i < arr.size(); ++i) {
            if (arr[i] < arr[minIndex]) {
                minIndex = i;
            }
        }
        promise->set_value(minIndex);
        }).detach();

    return future;
}

void selectionSortWithAsync(std::vector<int>& arr) {
    for (std::size_t i = 0; i < arr.size() - 1; ++i) {
        std::future<std::size_t> minFuture = asyncFindMin(arr, i);
        std::size_t minIndex = minFuture.get();
        if (minIndex != i) {
            std::swap(arr[i], arr[minIndex]);
        }
    }
}

int main() {
    setlocale(LC_ALL, "Russian");
    std::vector<int> numbers = { 3, 7, 1, 9, 2, 8, 5, 6, 4 };

    std::cout << "До сортировки: ";
    for (int num : numbers) {
        std::cout << num << " ";
    }
    std::cout << std::endl;

    selectionSortWithAsync(numbers);

    std::cout << "После сортировки: ";
    for (int num : numbers) {
        std::cout << num << " ";
    }
    std::cout << std::endl;

    return 0;
}

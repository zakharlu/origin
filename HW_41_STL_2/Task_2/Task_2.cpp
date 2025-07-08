#include <iostream>
#include <set>
#include <vector>
#include <list>

template <typename Container>
void print_container(const Container& container) {
    bool is_first = true; 
    for (const auto& item : container) {
        if (!is_first) {
            std::cout << ", ";
        }
        std::cout << item;
        is_first = false;  
    }
    std::cout << std::endl;
}

int main() {
    std::set<std::string> test_set = { "one", "two", "three", "four" };
    print_container(test_set);

    std::vector<std::string> test_vector = { "one", "two", "three", "four" };
    print_container(test_vector);

    std::list<std::string> test_list = { "one", "two", "three", "four" };
    print_container(test_list);

    return 0;
}
#include <iostream>
#include <vector>
#include <set>
#include <algorithm>

using namespace std;

int main() {
    int n;
    cout << "[IN]:" << endl;
    cin >> n;  

    set<int> unique_numbers; 


    for (int i = 0; i < n; ++i) {
        int num;
        cin >> num;
        unique_numbers.insert(num);  // Дубликаты автоматически игнорируются
    }
    vector<int> sorted_numbers(unique_numbers.begin(), unique_numbers.end());

    sort(sorted_numbers.begin(), sorted_numbers.end(), greater<int>());
    
    cout << "[OUT]:" << endl;
    for (int num : sorted_numbers) {
        cout << num << endl;
    }

    return 0;
}
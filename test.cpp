#include <iostream>

int main() {
    std::cout << "How many fucks do you give? \n";
    int a;
    std::cin >> a;
    for (int i = 0; i < abs(a); ++i) {
        std::cout << " fuck ";
    }

    return 0; // code 0 => exit success, everything else not that good
};

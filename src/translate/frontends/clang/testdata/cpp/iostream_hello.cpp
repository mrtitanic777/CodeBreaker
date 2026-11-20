// Uses the C++ standard library's streams and containers: outside what the C++ front end carries yet,
// so the translator must refuse it and say why.
#include <iostream>
#include <vector>

int main() {
    std::vector<int> v;
    for (int i = 0; i < 5; i++) v.push_back(i * i);
    int s = 0;
    for (int x : v) s += x;
    std::cout << "sum " << s << std::endl;
    return 0;
}

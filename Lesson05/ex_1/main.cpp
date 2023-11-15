#include <iostream>

#include <vector>
#include <numeric>
#include <functional>

int main() {

    std::vector<int> a{ 1,2,3,4,5,6 };
    std::vector<int> b{ 6,2,4,3,5,1 };

    std::cout << std::inner_product(a.cbegin(), a.cend(), b.cbegin(), 0) << '\n';
    std::cout << std::inner_product(std::cbegin(a), std::cend(a), std::cbegin(b), 0, std::plus<>{}, std::equal_to<>{}) << '\n';
  
}

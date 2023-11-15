#include <iostream>

#include <vector>
#include <numeric>
#include <functional>
#include <algorithm>

int main() {

    // Makaronok
    // min { sum (2^i * Ci) } [km]
    std::vector<int> c{ 2,6,3,9,1,4 };
   
    //  127
    //  10^2 * 1 + 10^1 * 2 + 10^0 * 7
    //  10 * (10 * (1) + 2) + 7
    std::sort(std::begin(c), std::end(c));

    const auto km = std::accumulate(std::cbegin(c), std::cend(c), 0ll, [](long long res, int i) {
        return 2 * res + i;
    });
	
    std::cout << km << "\n";
    return 0;
}

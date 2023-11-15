#include <iostream>
#include <string>

#include <vector>
#include <numeric>
#include <functional>

// function object
class MyMultiplies {

    public:

        int operator()(int i, int j) const noexcept {
            return i * j;
        }
 
};


int main() {

    std::vector<int> v{ 1,2,3,4,5,6,7,8,9 };

    const auto sum   = std::accumulate(v.cbegin(), v.cend(), 0);
    const auto prod  = std::accumulate(std::cbegin(v), std::cend(v), 1, std::multiplies<>{});
    const auto prod2 = std::accumulate(std::cbegin(v), std::cend(v), 1, MyMultiplies{});

    // 1,2,3,4,5,6,7,8,9
    // 1 - 2 - 3 - ... - 9
    // v must not be empty
    const auto s = std::accumulate(std::next(std::cbegin(v)), std::cend(v), std::to_string(v[0]), [](std::string res, int i) {
                       return res + " - " + std::to_string(i);
                   });

    const auto res = [](std::string res, int i) {
        return res + " - " + std::to_string(i);
    } ("almafa", 12);

    std::cout << "Sum: "  << sum   << "\n";
    std::cout << "Prod: " << prod  << "\n";
    std::cout << "Prod: " << prod2 << "\n";
    std::cout << "S: "    << s     << "\n";
    std::cout << "Res: "  << res   << "\n";

}

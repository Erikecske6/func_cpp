#include <algorithm>
#include <atomic>
#include <cassert>
#include <execution>
#include <iostream>
#include <ranges>
#include <sstream>
#include <thread>
#include <vector>
#include <atomic>

int main() {
  
    constexpr auto n = 65536ULL;
    const auto numrange = std::views::iota(1) | std::views::take(n);
    std::vector<double> nums;
    std::ranges::copy(numrange.begin(), numrange.end(), std::back_inserter(nums));

    // Inducing side-effects with violiating contract on the function
    // to be parallelized
    //std::atomic<double> accu{};

    //std::for_each(std::execution::par_unseq, nums.begin(), nums.end(), [&accu](double v) { 
    //    std::atomic_fetch_add(&accu, v);
    //    //accu.atomic_fetch_add(v);
    //    //accu += v;
    //});

    //// No side effects when using pure functions
    const double accu = std::reduce(std::execution::par_unseq, nums.begin(), nums.end(), 0.0, [](double accu, double v) { return accu + v; });

    if (const auto expected = n * (n + 1) / 2.0; accu != expected) {
        std::cerr << "error " << accu << "!=" << expected << '\n';
    }

    // parallel sort can also be used
    std::sort(std::execution::par_unseq, nums.begin(), nums.end(), std::greater<>{});

    if (nums[0] != 65536.0)
        std::cerr << "error: nums are not sorted descending\n";

}

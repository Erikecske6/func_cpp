#include <string>
#include <iostream>
#include <functional>
#include <vector>


std::vector<int> zappImpl(std::vector<std::function<int(int)>>::const_iterator fB, std::vector<std::function<int(int)>>::const_iterator fE, std::vector<int>::const_iterator xB) {

    std::vector<int> a;
    a.push_back((*fB)(*xB));

    fB++; xB++;

    if (fB != fE) {
        auto b = zappImpl(fB, fE, xB);
        std::move(b.begin(), b.end(), std::back_inserter(a));
    }

    return a;
}


auto zapp(std::vector<const std::function<int(int)>> &f, const std::vector<int> &x) {
    return zappImpl(f.cbegin(), f.cend(), x.cbegin());
}


int main() {

    std::vector<std::function<int(int)>> f{
        [](int i){ return i + 1; },
        [](int i){ return i + 2; },
        [](int i){ return i + 3; }
    };
    const std::vector<int> x{1,2,3};

    for(const auto e : zapp(f, x)) {
        std::cout << e << ' ';
    }
}

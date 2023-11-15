#include <iostream>

// BoolPack encapsules the bool values in its type
template<bool ...B>struct BoolPack {};

// Here we compare two BoolPacks. The first BoolPack is created by adding
// an extra true at the end. The second one by rotating the pack of the
// first one. If the two BoolPack's type is same, then we have all trues.
template<bool ...B>
struct Conjuction {
    static constexpr bool value = std::is_same<BoolPack<B..., true>, BoolPack<true, B...>>::value;
};

int main() {
    std::cout << Conjuction<true, true, false, true, true, true>::value << '\n';
    std::cout << Conjuction<true, true, true, true, true>::value << '\n';
}

#include <iostream>
#include <string>

// "general" template
template<typename U, typename V>
struct is_same2 {
    static constexpr bool value = false;
};

// specialize the previous one for the case
// if the two parameters are the same
template<typename T>
struct is_same2<T, T> {
    static constexpr bool value = true;
};


template<typename U, typename V>struct is_same3 : public std::false_type {};
template<typename T>struct is_same3<T, T> : public std::true_type {};


template<typename T>
using is_int = std::is_same<T, int>;


int main() {
    std::cout << is_same3<int, double>::value << '\n';
    std::cout << is_same3<int, std::string>::value << '\n';
    std::cout << is_same3<int, int>::value << '\n';
  
    std::cout << is_int<double>::value << '\n';
}

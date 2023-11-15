#include <iostream>
#include <type_traits>

template<typename T>
bool allZeroes(T &&t) {
    return (t == 0);
}

template<typename T, typename ...Args>
bool allZeroes(T &&t, Args&& ...args) {
    return (t == 0) && allZeros(std::forward<Args>(args)...);
}

template<typename T>
auto squareSum(T t) {
    return t * t;
}

template<typename T, typename ...Args>
auto squareSum(T t, Args ...args) {
    return (t * t) + squareSum(args...);
}

int main() {
    std::cout << allZeros(0,0,0,0,0,0,0,0,1) << '\n';
    std::cout << squareSum(1,1,1,1,1,2,1)    << '\n';
}

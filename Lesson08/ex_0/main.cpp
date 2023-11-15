#include <iostream>

// C++17 fold expression

template<typename T> auto addAll(T t) {
    return t;
}

template<typename T, typename ...Args> auto addAll(T t, Args&&... args) {
    return t + addAll(std::forward<Args>(args)...);
}

template<typename ...Args> auto addAll2(Args&&... args) {
    return ((std::forward<Args>(args)+... +0);
 
    //return ((args*args)+... +0);
    //return (0+ ...+args);
}


int main() {
    std::cout << addAll(1,1,1,1,0,0,0,0,2,1,1);
    std::cout << addAll2(1, 1, 1, 1, 0, 0, 0, 0, 2, 1, 1);
}

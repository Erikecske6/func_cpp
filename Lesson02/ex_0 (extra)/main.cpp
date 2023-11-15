#include <cassert>
#include <iostream>
#include <stdarg.h>
#include <string_view>
#include <tuple>
#include <utility>

// Variadic function
int add(size_t numargs, ...) {
    va_list vl;
    va_start(vl, numargs);
    int accu = 0;
    for (size_t i = 0; i < numargs; ++i)
        accu += va_arg(vl, int);
    // NOTE: va_end must be called in the same function where
    // va_start has been called, i.e. RAII can't be used
    va_end(vl);
    return accu;
}

void test_variadic_func() {
    std::cout << add(3, 10, 11, 12) << '\n';
    std::cout << add(2, 10, 11, 12) << '\n'; // OK, caller cleans up the stack
    //std::cout << add(4, 10, 11, 12) << '\n'; // Runtime errror, less arguments!
    //std::cout << add(3, 10, 11, 12.3) << '\n'; // Runtime errror, type mismatch!
}

// Variadic macro function
#define PP_NUMT(...) (std::tuple_size_v<decltype(std::make_tuple(__VA_ARGS__))>)

#define PP_SUM(T, ...) ::add(PP_NUMT(__VA_ARGS__), __VA_ARGS__)

void test_pp_funcs() {
    std::cout << PP_SUM(int, 1, 2, 3, 4, 5) << '\n';
}


int main(int argc, char** argv) {
    test_variadic_func();
    test_pp_funcs();
}

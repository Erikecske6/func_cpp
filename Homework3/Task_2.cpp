#include "Task.h"

#include <string>
#include <utility>

using namespace std::literals;

void should_compile_with_meaningful_error_1() {
    auto bar = Bar{};
    bar.add("This should give a meaningful compiler error"s);
}

void should_compile_with_meaningful_error_2() {
    auto bar = Bar{};
    const auto i = 4;
    bar.add(4);
}

int main() {
    should_compile_with_meaningful_error_1();
    should_compile_with_meaningful_error_2();
}

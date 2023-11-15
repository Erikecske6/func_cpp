#include <utility>

#include "Task.h"

int main() {
    auto bar = Bar{};
    auto bp = BarProxy{bar};

    auto foo1 = Foo{};
    auto foo2 = Foo{};

    bp.add(Foo{});
    bp.add(foo1);
    bp.add(std::move(foo2));
}

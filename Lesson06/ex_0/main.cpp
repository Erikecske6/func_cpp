#include <variant>
#include <iostream>
#include <string>

using Unit    = std::monostate;
using Nothing = Unit;

// our maybe type
template<typename T>using Maybe = std::variant<Nothing, T>;


struct Visitor {
    auto operator()(int i) const {
        std::cout << "int: " << i << '\n';
    }

    auto operator()(double d) const {
        std::cout << "double: " << d << '\n';
    }

    template<typename T>
    auto operator()(const T &t) const {
        std::cout << "other\n";
    }
};

// we use this helper class to create a visitor class
// for the std::visit from lambda functions
template<typename ...F>struct Overload : public F... {	
 
    template<typename ...U>
    Overload(U&& ...u) : F{ std::forward<U>(u) }... {}
  
    using F::operator()...;
};

// deduction guide for the template parameters of the class
template<typename... T> Overload(T...)->Overload<T...>;


int main() {

    {
        std::variant<int, double, std::string> v;
        v = 12.5;

        std::visit(Visitor{}, v);
    }

    {
        auto visitor = Overload{
            [](Nothing) { std::cout << "Nothing\n"; },
	          [](int i) { std::cout << "int: " << i << '\n'; }
        };

        Maybe<int> i;
        std::visit(visitor, i);

        Maybe<int> j = 5;
        std::visit(visitor, j);
    }
}


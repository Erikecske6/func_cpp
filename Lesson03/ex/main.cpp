#include <iostream>

int main() {

    ///  true  = λxy.x
    ///  false = λxy.x
    ///  not   = λx.x false true

    auto TRUE = [=](auto x) {
        return [=](auto y) {
            return x;
        };
    };

    auto FALSE = [=](auto x) {
        return [=](auto y) {
            return y;
        };
    };

    auto NOT = [=](auto x) {
        return (x(FALSE)) (TRUE);
    };

    auto q = NOT(FALSE);

    static_assert(std::is_same<decltype(q), decltype(TRUE)>::value, "");

    ///  pair   = λxyz.zxy
    ///  first  = λx.xtrue ≡ λx.x(λyz.y)
    ///  second = λx.xfalse ≡ λx.x(λyz.z)

    auto PAIR = [=](auto x) {
        return [=](auto y) {
            return [=](auto z) {
                return (z(x))(y);
            };
        };
    };

    auto FIRST = [=](auto x) {
        return x(
            [=](auto y) {
                return [=](auto z) {
                    return y;
                };
            }
        );
    };

    auto SECOND = [=](auto x) {
        return x(
            [=](auto y) {
                return [=](auto z) {
                    return z;
                };
            }
        );
    };


    std::cout << FIRST((PAIR(12))((PAIR(13))(2)));
    std::cout << "\n";
    std::cout << FIRST(SECOND((PAIR(12))((PAIR(13))(2))));
    std::cout << "\n";
    std::cout << SECOND(SECOND((PAIR(12))((PAIR(13))(2))));
    std::cout << "\n";

    ///  cons  = λxy.pair false (pair x y)
    ///  nil   = pair true true
    ///  head  = λx.first(second x)
    ///  tail  = λx.second(second x)

    auto CONS = [=](auto x) {
        return [=](auto y) {
            return (PAIR(FALSE))((PAIR(x))(y));
        };
    };

    auto HEAD = [=](auto x) {
        return FIRST(SECOND(x));
    };

    auto TAIL = [=](auto x) {
        return SECOND(SECOND(x));
    };

    auto NIL = (PAIR(TRUE))(TRUE);


    auto e1 = (CONS(1))(NIL);
    auto e2 = (CONS(2))(e1);
    auto e3 = (CONS(3))(e2);

    std::cout << HEAD(e3) << HEAD(TAIL(e3)) << HEAD(TAIL(TAIL(e3)));

}

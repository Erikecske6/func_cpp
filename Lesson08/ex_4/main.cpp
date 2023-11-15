#include <iostream>
#include <optional>  /* <--- C++17 */

struct Pierre {
    int left  = 0;
    int right = 0;
};

auto banana(Pierre p) {
    return std::optional<Pierre>{};
}

std::optional<Pierre> addBirdLeft(Pierre p, int birds) {
    if (p.left + birds > p.right + 4)
        return {};
    return Pierre{ p.left + birds, p.right };
}

std::optional<Pierre> addBirdRight(Pierre p, int birds) {
    if (p.right + birds > p.left + 4)
        return {};
    return Pierre{ p.left, p.right + birds };
}

std::ostream& operator<<(std::ostream &out, const Pierre &p) {
    out << "[" << p.left << ", " << p.right << "]";
    return out;
}

std::ostream& operator<<(std::ostream &out, const std::optional<Pierre> &p) {
    if (p.has_value())
        out << p.value();
    else
        out << "RIP";
    return out;
}

template<typename F>std::optional<Pierre> mbind(F &&f, std::optional<Pierre> p) {
    if (p.has_value())
        return std::invoke(std::forward<F>(f), p.value());
    return {};
}

template<typename F, typename G> auto operator*(F &&f, G &&g) {
    return[f, g](std::optional<Pierre> p) {
        return mbind(f, mbind(g, p));
    };
}


int main() {

    auto addLeft = [](int birds) {
        return [birds](Pierre p) { return addBirdLeft(p, birds); };
    };

    auto addRight = [](int birds) {
        return [birds](Pierre p) { return addBirdRight(p, birds); };
    };

    auto p = mbind(addRight(3),
                               mbind(addLeft(1),
                                     mbind(addLeft(2), Pierre{})));
    std::cout << p << '\n';

    auto pp = (addRight(3) * addLeft(2))(Pierre{});
    std::cout << pp << '\n';
}

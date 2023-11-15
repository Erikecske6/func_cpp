#include <iostream>
#include <map>

struct fibonacci_r {
    template <typename F> size_t operator()(F &&f, size_t v) const {
        std::cout << "called fibo with " << v << '\n';
        return v < 2 ? 1 : std::invoke(f, v - 1) + std::invoke(f, v - 2);
    }
};

template <typename F, typename T> struct memoized_r;

template <typename F, typename R, typename... Ts>
struct memoized_r<F, R(Ts...)> {

    private:
        F f;
        mutable std::map<std::tuple<std::decay_t<Ts>...>, R> m_cache;

    public:
        explicit memoized_r(F f) : f{std::move(f)} {}

        // using R = std::invoke_result_t<F, memoized_r<F, Ts...>, Ts...>;

        template <typename... Us> R operator()(Us &&...u) const {
            // if init-list: https://en.cppreference.com/w/cpp/language/if    init-statement
            if (auto it = m_cache.find(std::make_tuple(std::ref(u)...)); it != m_cache.end())
                return it->second;
            else {
                // structured binding
                auto &&[iit, inserted] = m_cache.emplace(std::make_tuple(std::ref(u)...), std::invoke(f, *this, std::forward<Us>(u)...));
                return iit->second;
            }
        }
};

template <typename Signature, typename F> auto memoize_r(F f) {
    return memoized_r<F, Signature>(std::move(f));
}


bool is_prime(size_t x) {
    std::cerr << "is_prime called with " << x << '\n';
    for (size_t i = 2; i * i <= x; ++i) {
        if (x % i == 0)
            return false;
    }
    return true;
}

struct is_prime_t {
    template <typename F> bool operator()(F &&, size_t x) const {
        std::cerr << "is_prime called with " << x << '\n';
        for (size_t i = 2; i * i <= x; ++i) {
            if (x % i == 0)
                return false;
        }
        return true;
    }

    bool operator()(size_t x) const {
        return (*this)([]() {}, x);
    }
};

int main() {

    auto is_prime_m = memoize_r<size_t(size_t)>(is_prime_t{});
    auto m_fibo = memoize_r<size_t(size_t)>(fibonacci_r{});

    is_prime_m(100000015333);
    is_prime_m(100000015333);

    std::cout << m_fibo(4) << '\n';
    std::cout << m_fibo(5) << '\n';
    std::cout << m_fibo(6) << '\n';

    return 0;
}

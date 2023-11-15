#include <future>
#include <iostream>
#include <map>
#include <optional>
#include <string>
#include <type_traits>

template <typename T> struct Lazy {
    private:
        mutable std::future<T> m_fut;
        mutable std::optional<T> m_val;
  
    public:
  
        template <typename F, typename... Args>
        Lazy(F f, Args &&...args)
            : m_fut{std::async(std::launch::deferred, std::move(f), std::forward<Args>(args)...)} {}

        const auto &get() const {
            if (!m_val) {
                m_val = m_fut.get();
            }
            return *m_val;
        }
};

// Deduction guide
template <typename F, typename... Args>
Lazy(F, Args...) -> Lazy<std::invoke_result_t<F, Args...>>;

// From previous lesson
template <typename F> struct FunctionHolder : public F {
    auto &func() { return static_cast<F &>(*this); }
    auto &func() const { return static_cast<const F &>(*this); }
};

template <typename F> struct FunctionHolder<F *> {
    private:
        F *m_func{};
    public:
        FunctionHolder(F *f) : m_func{f} {}
        auto &func() { return m_func; }
        auto &func() const { return m_func; }
};

template <typename F> struct fmap : public FunctionHolder<F> {
  
    template <typename U>
    explicit fmap(U &&u) : FunctionHolder<F>{std::forward<U>(u)} {}

    template <typename T>
    requires std::invocable<F, T>        // <-- C++20
    auto operator()(Lazy<T> l) const {
        // using RT = std::invoke_result_t<F, T>;  // <-- we could guess the return type too
        return Lazy([](auto _this, auto &&l) { return _this.func()(l.get()); }, std::move(*this), std::move(l));
    }
};

template<typename F> fmap(F f) -> fmap<F>;


bool is_prime(size_t x) {
    std::cerr << "is_prime called with " << x << '\n';
    for (size_t i = 2; i * i <= x; ++i) {
        if (x % i == 0)
            return false;
    }
    return true;
}

bool is_prime_memoized(size_t x) {
    static std::map<size_t, bool> _cache;
    // if init-list
    if (auto it = _cache.find(x); it != _cache.end())
        return it->second;
  
    const auto res = is_prime(x);
    _cache.emplace(x, res);
    return res;
}

std::string toStr(double d) { return std::to_string(d); }

int main() try {

    auto f1 = std::async([]() { return 42; });
    f1.get();
    // f1.get();// would throw..
  

    // Lazy
    auto func = [](int a, double d) { return (42 + a) * d; };
    Lazy l1(func, 10, 3.14);
    std::cout << l1.get() << '\n';
    std::cout << l1.get() << '\n';

    // Lazy functor
    auto l2 = fmap(toStr)(Lazy(func, 6, 2.17));
    std::cout << l2.get() << '\n';

    // lazy is_prime
    Lazy isp(is_prime, 100000015333);
    //...
    //...
    Lazy isp2(is_prime, 100000015333);
    std::cout << isp.get() << ' ' << isp2.get() << '\n';

    // lazy is_prime memoized
    std::cout << "--------------------\n";
    Lazy isp_m(is_prime_memoized, 100000015333);
    //...
    //...
    Lazy isp2_m(is_prime_memoized, 100000015333);
    std::cout << isp_m.get() << ' ' << isp2_m.get() << '\n';

} catch (const std::exception &e) {
    std::cerr << e.what() << '\n';
    return 1;
}

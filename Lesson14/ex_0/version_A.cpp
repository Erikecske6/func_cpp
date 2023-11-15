#include <vector>
#include <functional>
#include <type_traits>
#include <future>
#include <optional>
#include <iostream>
#include <initializer_list>

// loeb
// loeb fs  = fmap (\f -> f (loeb fs)) fs
// or
// loeb fs = [ f (loeb fs) | f <- fs ]

template <typename T>
struct Lazy {

    private:

        mutable std::future<T>   fut;
        mutable std::optional<T> val;

    public:

        template <typename F, typename... Args>Lazy(F f, Args&&...args)
            : fut{ std::async(std::launch::deferred, std::move(f), std::forward<Args>(args)...) } {}

        const auto &get() const {
            if (!val)
                val = fut.get();
            return *val;
        }

}; // struct Lazy

template<typename T>
struct Loeb {

    private:
  
        std::vector<Lazy<T>> lazy_res;

    public:

        Loeb(std::initializer_list<std::function<T(const Loeb<T>&)>> loeb) {
            lazy_res.reserve(loeb.size());
            for (const auto &f : loeb) {
                lazy_res.push_back(Lazy<T>{ f, std::cref(*this) });
            }
        }

        const T& at(std::size_t i) const {
            return lazy_res.at(i).get();
        }

        const T& operator[](std::size_t i) const {
            return lazy_res[i].get();
        }

        auto size() const noexcept {
            return lazy_res.size();
        }

        std::vector<T> operator()() {
            auto res = std::vector<T>{};
            res.reserve(lazy_res.size());

            for (const auto &r : lazy_res)
                res.push_back(r.get());

            return res;
        }

};  // struct Loeb


int length(const Loeb<int> &loeb) {
    return static_cast<int>(loeb.size());
}

int main() {

    {
        auto res = Loeb<int>{
             length,
             [](const auto &loeb) { return 1; },
             [](const auto &loeb) { std::cout << "in loeb: lambda called\n"; return loeb[0] + 1; },
             [](const auto &loeb) { std::cout << "in loeb: lambda called\n"; return loeb[1] + 1; }
        };

        std::cout << "loeb wasn't evaluated yet\n";

        for (const auto &r : res()) {
            std::cout << r << ' ';
        }
    }

    std::cout << '\n';

    {
		    const auto res = Loeb<int>{
            length,
            [](const auto &loeb) { return 1; },
            [](const auto &loeb) { return loeb[0] + 1; },
            [](const auto &loeb) { return loeb[1] + 1; }
        }();

        for (const auto &r : res)
            std::cout << r << ' ';
    }

    std::cout << '\n';

    {
        const auto res = Loeb<int>{
            length,
            [](const auto &loeb) { return loeb[2] + 1; },
            [](const auto &loeb) { return loeb[3] + 1; },
            [](const auto &loeb) { return 1; }
        }();

        for (const auto &r : res)
            std::cout << r << ' ';
    }

    std::cout << '\n';

    {
        const auto res = Loeb<int>{
            length,
            [](const auto &loeb) { return loeb[3] + 1; },
            [](const auto &loeb) { return loeb[0] + 1; },
            [](const auto &loeb) { return 1; }
        }();

        for (const auto &r : res)
            std::cout << r << ' ';
    }
}

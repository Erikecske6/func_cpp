#include <vector>
#include <functional>
#include <type_traits>
#include <future>
#include <optional>

#include <iostream>

// loeb
// loeb fs  = fmap (\f -> f (loeb fs)) fs
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

        template<typename ...Fs>
        Loeb(Fs ...fs) {

            // assert the type of the functions
            static_assert(std::conjunction_v<std::is_invocable<Fs, const Loeb<T>&>...>, "At least one of the functions cannot be called const Loeb&.");
            static_assert(std::conjunction_v<std::is_same<std::invoke_result_t<Fs, const Loeb<T>&>, T>...>, "At least one of the given functions return wrong type.");

            // reserver the place
            lazy_res.reserve(sizeof...(fs));

            /////// we want to use auto in the lambdas, with this solutions the parameter would be reference_wrapper
            /////(lazy_res.push_back(Lazy<T>{ std::move(fs), std::cref(*this) }), ...);
          
            // ensure that we call the function with const Loeb&
            (lazy_res.push_back(Lazy<T>{ [ffs = std::move(fs)](const Loeb<T> &loeb) mutable { return std::invoke(std::move(ffs), loeb); }, std::cref(*this) }), ...);
        }

        const T& at(std::size_t i) const {
            return lazy_res.at(i).get();
        }

        const T& operator[](std::size_t i) const {
            return lazy_res[i].get();
        }

        auto size() const {
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

        std::cout << "loeb wasn't evaluated yet (nothing is printed before this line)\n";

        for (const auto &r : res())
            std::cout << r << ' ';
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

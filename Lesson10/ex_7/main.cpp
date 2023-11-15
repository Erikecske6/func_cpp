#include <iostream>
#include <vector>

#include <fucntional>
#include <iterator>
#include <ranges>
#include <utility>
#include <type_traits>

namespace details {

    namespace fmap_iterator_tags {
        class begin_t {};
        class end_t {};

        static constexpr begin_t begin;
        static constexpr end_t   end;
    }


    template<typename R1, typename R2>class fmap_iterator {

        private:

            using R1It = decltype(std::cbegin(std::declval<R1>()));  ///< The type of the R1's iterator.
            using R2It = decltype(std::cbegin(std::declval<R2>()));  ///< The type of the R2's iterator.

            /// Deduce the return type.
            using ResultType = std::invoke_result_t<decltype(*std::cbegin(std::declval<R1>())), decltype(*std::cbegin(std::declval<R2>()))>;

            std::pair<R1It, R2It> it;  ///< The current positions.
            std::pair<R1It, R2It> b;   ///< All the begin iterators.
            std::pair<R1It, R2It> e;   ///< All the end iterators.

        public:

            /// Some common iterator typedefs.
            using iterator_category = std::input_iterator_tag;
            using value_type = ResultType;
            using difference_type = std::ptrdiff_t;
            using pointer = ResultType*;
            using reference = ResultType&;

            fmap_iterator() = default;

            // We use tag dispatch to differenctiate the two constructors.
            fmap_iterator(fmap_iterator_tags::begin_t, const R1 &r1, const R2 &r2)
                : it{ std::cbegin(r1), std::cbegin(r2) }
                , b{ std::cbegin(r1), std::cbegin(r2) }
                , e{ std::cend(r1), std::cend(r2) }
            {}

            fmap_iterator(fmap_iterator_tags::end_t, const R1 &r1, const R2 &r2)
                : it{ std::cend(r1), std::cend(r2) }
                , b{ std::cbegin(r1), std::cbegin(r2) }
                , e{ std::cend(r1), std::cend(r2) }
            {}

            fmap_iterator& operator++() {
                if (it.second == e.second && it.first == e.first)
                    return *this;
                ++it.second;
                if (it.second == e.second) {
                    ++it.first;
                    if (it.first != e.first) {
                        it.second = b.second;
                    }
                }
                return *this;
            }

            fmap_iterator operator++(int) {
                const auto tmp = *this;
                this->operator++();
                return tmp;
            }

            auto operator*() const { return std::invoke(*it.first, *it.second); }
            bool operator==(const fmap_iterator& other) const { return it == other.it; }
            bool operator!=(const fmap_iterator& other) const { return !(*this == other); }
    };
}

namespace views {

    template<typename R1, typename R2>auto fmap_product(const R1 &r1, const R2 &r2) {
        return std::ranges::subrange(
            details::fmap_iterator<R1, R2>(details::fmap_iterator_tags::begin, r1, r2),
            details::fmap_iterator<R1, R2>(details::fmap_iterator_tags::end, r1, r2)
        );
    }

}


int main() {

    std::vector<int> v{ 5,6,7,8 };

    auto app = std::views::iota(10, 12) | std::views::transform([](int i) {
        return [i](int j) { return std::make_pair(i, j); };
    });

    for (const auto x : views::fmap_product(app, std::views::iota(100, 102))) {
        std::cout << x.first << ' ' << x.second << '\n';
    }

    for (const auto& f : views::fmap_product(app, v) | std::views::drop(2) | std::views::take(3)) {
        std::cout << f.first << ", " << f.second << '\n';
    }

}

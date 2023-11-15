#pragma once
#include "class_utils.hpp"

#include <algorithm>
#include <functional>
#include <memory>

namespace hw_II {

namespace implemenatation {

// The recursive function for the solution

template <typename Cont> auto tail(Cont const &v) {
  return Cont(std::next(v.begin()), v.end());
}

template <typename Cont> auto r_tail(Cont const &v) {
  return Cont(v.begin(), std::prev(v.end()));
}

template <typename Cont> struct calc_max_score {
  using container = Cont;
  template <typename F> int operator()(F &&f, Cont const &v) const {
    if (v.empty())
      return 0;
    else if (v.size() == 1)
      return v.front();
    else if (v.size() == 2)
      return std::max(v.front(), v.back());
    else {
      const auto x = std::invoke(std::forward<F>(f), r_tail(r_tail(v)));
      const auto y = std::invoke(std::forward<F>(f), r_tail(tail(v)));
      const auto z = std::invoke(std::forward<F>(f), tail(tail(v)));
      return std::max(
          {v.back() + x, v.back() + y, v.front() + y, v.front() + z});
    }
  }
};
} // namespace implemenatation

auto calc_max_score(std::vector<int> const &in) {
  // Create a memoized version of the recursive definition of the solution
  // using calc = implemenatation::calc_max_score<std::vector< const int>>;
  // using calc = implemenatation::calc_max_score<cache_utils::vector_view<const int>>;
  using calc = implemenatation::calc_max_score<
      cache_utils::light_vector_view<const int>>;
  auto memoized = from_class::memoize_r<int(calc::container const &)>(calc{});
  // call the memoized version of the function with the initial conditions
  auto begin = !in.empty() ? std::addressof(*in.begin()) : nullptr;
  return memoized(calc::container(begin, begin + in.size()));
}

} // namespace hw_II

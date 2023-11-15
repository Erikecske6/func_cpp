#pragma once

#include <functional>
#include <map>
#include <memory_resource>
#include <numeric>
#include <span>
#include <string_view>
#include <tuple>
#include <unordered_map>
#include <utility>
#include <vector>

namespace cache_utils {

/* Simple flat map implementation for demonstration purposes */
template <typename KeyT, typename ValueT, typename Less = std::less<KeyT>,
          typename Allocator = std::allocator<std::pair<KeyT, ValueT>>>
struct flat_map {
  using allocator_type = Allocator;
  flat_map() = default;
  explicit flat_map(Allocator const &allocator) : m_cont(allocator) {}
  template <typename T, typename U>
  static bool equals(T const &lhs, U const &rhs) {
    return !Less{}(lhs, rhs) && !Less{}(rhs, lhs);
  }
  template <typename UKeyT> auto find(const UKeyT &key) const {
    auto lb = lbound(key);
    return lb != end() && equals(key, lb->first) ? lb : end();
  }

  auto begin() { return m_cont.begin(); }
  auto end() { return m_cont.end(); }
  auto begin() const { return m_cont.begin(); }
  auto end() const { return m_cont.end(); }

  template <typename Key, typename... Args>
  auto emplace(Key &&key, Args &&...args) {
    auto lb = lbound(key);
    if (lb != end() && equals(lb->first, key))
      return std::make_pair(lb, false);
    return std::make_pair(
        m_cont.emplace(lb, std::forward<Key>(key), std::forward<Args>(args)...),
        true);
  }

  auto reserve(size_t count) { m_cont.reserve(count); }

private:
  template <typename UKeyT> auto lbound(const UKeyT &key) const {
    return std::lower_bound(
        m_cont.begin(), m_cont.end(), key,
        [](auto &lhs, auto &rhs) { return Less()(lhs.first, rhs); });
  }
  template <typename UKeyT> auto lbound(const UKeyT &key) {
    return std::lower_bound(
        m_cont.begin(), m_cont.end(), key,
        [](auto &lhs, auto &rhs) { return Less()(lhs.first, rhs); });
  }

  std::vector<std::pair<KeyT, ValueT>, Allocator> m_cont;
};

// helper class for capturing compile time indexes
template <size_t Idx> using c_size_t = std::integral_constant<size_t, Idx>;

// Transparent tuple less, to support transparent comparison
// of tuples of compatible types
template <typename Less = std::less<>> struct tuple_less {

  // marking compare class as transparent for enabling find() overloads in the
  // cache
  using is_transparent = std::true_type;

  template <typename... Ts, typename... Us>
  std::enable_if_t<sizeof...(Ts) == sizeof...(Us), bool>
  operator()(std::tuple<Ts...> const &lhs, std::tuple<Us...> const &rhs) const {
    return Less()(lhs, rhs);
  }
};

template <typename Equals = std::equal_to<>> struct tuple_equal_to {
  // marking compare class as transparent for enabling find() overloads in the
  // cache
  using is_transparent = std::true_type;
  template <typename... Ts, typename... Us>
  std::enable_if_t<sizeof...(Ts) == sizeof...(Us), bool>
  operator()(std::tuple<Ts...> const &lhs, std::tuple<Us...> const &rhs) const {
    return Equals()(lhs, rhs);
  }
};

// This is a default implementation to use
// for the ADL triggered marked with BOOKMARK_1
namespace adl_scope {
template <typename T> auto hash(T const &t) {
  std::hash<T> hasher;
  return hasher(t);
}
} // namespace adl_scope

// Reference implementation based on the one from boost libraries for combining
// hash values
template <class T>
inline std::size_t hash_combine(std::size_t seed, const T &v) {
  using adl_scope::hash;
  seed ^= /*BOOKMARK_1*/ hash(v) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
  return seed;
}

// Class for implementing hash calculation for std::tuple type
struct tuple_hash {
  template <typename... Ts>
  size_t operator()(const std::tuple<Ts...> &t) const {
    return impl(0, t, c_size_t<0>{});
  }

  // Recursive implementation of hash calculation: combine the hash values
  // one-by-one until the tuple is exhausted
  template <typename... Ts, size_t Idx>
  static size_t impl(size_t val, const std::tuple<Ts...> &t, c_size_t<Idx>) {
    if constexpr (sizeof...(Ts) < Idx)
      return impl(hash_combine(val, std::get<Idx>(t)), t, c_size_t<Idx + 1>{});
    else
      return val;
  }
};

/*****************************/
/* various cache definitions */
/*****************************/
 template <typename K, typename V, typename Less = tuple_less<>> struct Cache
 {
  using cache_t = std::map<K, V, Less>;
  cache_t cache;
};

//template <typename K, typename V, typename Hash = tuple_hash,
//          typename Equals = tuple_equal_to<>>
//struct Cache {
//  Cache() : buffer(16 * 1024 * 1024), pa{&buffer}, cache(1024, pa) {
//    cache.max_load_factor(0.68f);
//  }
//  using cache_t = std::pmr::unordered_map<K, V, Hash, Equals>;
//  std::pmr::monotonic_buffer_resource buffer;
//  typename cache_t::allocator_type pa;
//  cache_t cache;
//};

// template <typename K, typename V, typename Less = tuple_less<>> struct Cache
// {
//  Cache() { cache.reserve(16 * 1024 * 1024); }
//  using cache_t = flat_map<K, V, Less>;
//  cache_t cache;
//};

//  Helper class to wrap setting up memory resources that enables fast
// computation not dominated by memory allocations
// template <typename K, typename V> struct Cache {
//  Cache() : buffer(16* 1024 * 1024), pa{&buffer}, cache{pa} {}
//  using cache_t = std::pmr::map<K, V, tuple_less<>>;
//  std::pmr::monotonic_buffer_resource buffer;
//  typename cache_t::allocator_type pa;
//  cache_t cache;
//};
// Re-using basic_string view for creating a vector view
template <typename T> using vector_view = std::basic_string_view<T>;
template <typename T> struct light_vector_view : public std::span<T> {
  using std::span<T>::span;
};

template <typename T>
bool operator==(light_vector_view<T> lhs, light_vector_view<T> rhs) {
  return std::tuple(lhs.data(), lhs.size()) ==
         std::tuple(rhs.data(), rhs.size());
}
template <typename T>
bool operator<(light_vector_view<T> lhs, light_vector_view<T> rhs) {
  return std::tuple(lhs.data(), lhs.size()) <
         std::tuple(rhs.data(), rhs.size());
}
template <typename T> size_t hash(light_vector_view<T> val) {
  return cache_utils::hash_combine(cache_utils::hash_combine(0, val.data()),
                                   val.size());
}
} // namespace cache_utils

// This is the memoiziation implementation
// as we learnt during the class, with a slight modification on
// what is the cache type
namespace from_class {
template <typename F, typename T> struct memoized_r;

template <typename F, typename R, typename... Ts>
struct memoized_r<F, R(Ts...)> {
  template <typename U> explicit memoized_r(U &&f) : f{std::forward<U>(f)} {}
  template <typename... Us> R operator()(Us &&...u) const {
    if (auto it = cache().find(std::make_tuple(std::ref(u)...));
        it != cache().end())
      return it->second;
    else {
      auto &&[iit, inserted] = cache().emplace(
          std::make_tuple(u...), std::invoke(f, *this, std::forward<Us>(u)...));
      return iit->second;
    }
  }

private:
  auto &cache() const { return m_cache.cache; }

  F f;
  using key_t = std::tuple<std::decay_t<Ts>...>;
  using result_t = R;
  mutable cache_utils::Cache<key_t, result_t> m_cache;
};

template <typename Signature, typename F> auto memoize_r(F &&f) {
  return memoized_r<std::decay_t<F>, Signature>(std::forward<F>(f));
}

}; // namespace from_class

#include <iostream>
#include <string_view>
#include <utility>

#include <algorithm>
#include <map>
#include <tuple>
#include <unordered_set>

constexpr size_t indicator(char l, char r) { return l == r ? 0 : 1; }

size_t levenshtein_dist(std::string_view lhs, std::string_view rhs) {
    // std::cerr << "lhs=" << lhs << ",rhs=" << rhs << '\n';
    if (std::min(lhs.size(), rhs.size()) == 0)
        return std::max(lhs.size(), rhs.size());
    else {
        return std::min(
            std::min(levenshtein_dist(lhs.substr(0, lhs.size() - 1), rhs) + 1, 
                     levenshtein_dist(lhs, rhs.substr(0, rhs.size() - 1)) + 1),
            levenshtein_dist(lhs.substr(0, lhs.size() - 1) , rhs.substr(0, rhs.size() - 1)) + indicator(lhs[lhs.size() - 1], rhs[rhs.size() - 1])
        );
    }
}

template <typename RF>
size_t levenshtein_dist_r(RF &&rf, std::string_view lhs, std::string_view rhs) {
    // std::cerr << "lhs=" << lhs << ",rhs=" << rhs << '\n';

    if (std::min(lhs.size(), rhs.size()) == 0)
        return std::max(lhs.size(), rhs.size());
    else {
        return std::min(
            std::min(rf(lhs.substr(0, lhs.size() - 1), rhs) + 1, rf(lhs, rhs.substr(0, rhs.size() - 1)) + 1),
            rf(lhs.substr(0, lhs.size() - 1), rhs.substr(0, rhs.size() - 1)) + indicator(lhs[lhs.size() - 1], rhs[rhs.size() - 1])
        );
    }
}

size_t lev_origi(std::string_view lhs, std::string_view rhs) {
    return levenshtein_dist_r(lev_origi, lhs, rhs);
}

size_t lev_memoized(std::string_view lhs, std::string_view rhs) {
    static std::map<std::tuple<std::string_view, std::string_view>, size_t> _cache;
    
    // if init-list
    if (auto it = _cache.find(std::make_tuple(lhs, rhs)); it != _cache.end()) {
        return it->second;
    }
    else {
        const auto res = levenshtein_dist_r(lev_memoized, lhs, rhs);
        _cache.emplace(std::make_tuple(lhs, rhs), res);
        return res;
    }
}

auto suggest(std::string_view word) {

    using Dictionary = std::unordered_set<std::string>;

    static const Dictionary dict{
        "time",    "person", "year",  "way",     "day",   "thing", "lifetime",
        "for",     "man",    "learn", "hard",    "go",    "world", "life",
        "to",      "hand",   "part",  "child",   "long",  "eye",   "woman",
        "place",   "work",   "week",  "case",    "point", "ago",   "government",
        "company", "number", "group", "problem", "fact",  "almost"};

    auto m = std::min_element(dict.begin(), dict.end(), [=](const auto &lhs, const auto &rhs) {
        return lev_memoized(lhs, word) < lev_memoized(rhs, word);
    });
    return std::pair(*m, lev_memoized(*m, word));
}

int main() {

    std::cout << levenshtein_dist("book", "back") << '\n';
    std::cerr << "===========\n";
    std::cout << lev_memoized("sor", "mama") << '\n';
    std::cerr << "===========\n";
    std::cout << lev_memoized("so", "mam") << '\n';
    std::cerr << "===========\n";
    std::cout << lev_origi("sor", "mama") << '\n';
    std::cerr << "===========\n";
    std::cout << suggest("lifetiem").first << '\n';
    std::cout << suggest("placce").first << '\n';
}

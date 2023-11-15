#include <iostream>
#include <memory>
#include <type_traits>
#include <functional>
#include <cassert>

template<typename T>class List {
    private:

        struct Item {
            T val;
            std::shared_ptr<const Item> next;
            Item(T v, const std::shared_ptr<const Item>& tail) : val{ std::move(v) }, next{ tail } {}
        };

        std::shared_ptr<const Item> head;
    
        explicit List(const std::shared_ptr<const Item> &tail) : head{ tail } {}

    public:

        List() : head{ nullptr } {}
        List(T val, List tail) : head{ std::make_shared<Item>(std::move(val), tail.head) } {}

        bool empty() const {
            return !head;
        }

        T front() const {
            assert(!empty());
            return head->val;
        }

        List pop_front() const {
            assert(!empty());
            return List{ head->next };
        }

        List push_front(T v) const {
            return List{ v, *this };
        }
  
};  // class List

template<typename T, typename F>
auto fmap(F f, List<T> lst) {
    static_assert(std::is_invocable<F, T>::value, "Cannot call F with T.");

    using R = typename std::invoke_result<F, T>::type;

    if (lst.empty())
        return List<R>{};

    return List<R>{ f(lst.front()), fmap(f, lst.pop_front()) };
}

template<typename T, class P>
auto filter(List<T> lst, P p) {
    if (lst.empty())
        return List<T>{};

    if (p(lst.front()))
        return List<T>{ lst.front(), filter(lst.pop_front(), p) };
    else
        return filter(lst.pop_front(), p);
}

template<typename T, class F>
void for_each(List<T> lst, F f) {
    if (!lst.empty()) {
        f(lst.front());
        for_each(lst.pop_front(), std::move(f));
    }
}

template<typename T>
void print(List<T> lst) {
    for_each(lst, [](T v) {
        std::cout << "(" << v << ") ";
    });
    std::cout << '\n';
}

template<typename T, typename A, typename F>
A foldr(F f, A acc, List<T> lst) {

    if (lst.empty())
        return acc;

    return f(lst.front(), foldr(f, acc, lst.pop_front()));
}

template<typename T, typename A, typename F>
A foldl(F f, A acc, List<T> lst) {

    if (lst.empty())
        return acc;

    return f(f, f(acc, lst.front()), lst.pop_front());
}

int main() {

    auto l1 = List<int>{}.push_front(12).push_front(13).push_front(14);

    std::cout << "The list is " << (l1.empty() ? "empty" : "not empty") << ".\n";
    print(l1);

    auto l2 = fmap<int>([](int i) { return 2 * i; }, l1);
    print(l2);

    auto l3 = filter(l1, [](int i) { return i % 2 == 0; });
    print(l3);
}

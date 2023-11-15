#include <iostream>
#include <string>

template<typename T>
class Maybe {

    private:

        T *data;

    public:

        Maybe(T *data) : data{ data } {}

        template<typename F>auto with(F f);

        template<typename F>Maybe<T>& run(F f) {
            if (data)
                std::invoke(std::move(f), data);
            return *this;
        }
};

template<typename T>auto maybe(T *t) {
    return Maybe<T>{ t };
}

template<typename T> template<typename F> auto Maybe<T>::with(F f) {
    return maybe(data ? std::invoke(std::move(f), data) : nullptr);
}


// for testing
struct Address {
    std::string *street;
};

struct Person {
    Address *address;
};


int main() {

    Person *p = new Person;
    p->address = new Address;
    p->address->street = new std::string{ "Boraros ter" };

    maybe(p).with([](auto *p) { return p->address; })
            //.run([](auto *a) { std::cout << "here, in the middle of the chain, we could do something with the address too"; });
            .with([](auto *a) { return a->street; })
            .run([](auto *s) { std::cout << *s; });

    delete p->address->street;
    delete p->address;
    delete p;
}

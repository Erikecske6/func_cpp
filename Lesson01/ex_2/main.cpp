#include <iostream>
#include <string>

// Our simple type detector
template<typename V>struct TD;


class Foo {

  private:

        std::string name;
        std::size_t size;
        double *data;

    public:

        Foo(const std::string &name, std::size_t size) 
            : name{ name }
            , size{ size }
            , data{ new double[size] }
        {
            for (int i = 0; i < size; i++)
                data[i] = 0;
            std::cout << "~~ ctor ~~\n";
        }

        ~Foo() {
            std::cout << "~~ dtor ~~\n";
            delete[] data;
        }

        Foo(const Foo &other)
            : name{ other.name }
            , size{ other.size }
            , data{ new double[size]}
        {
            for (int i = 0; i < size; i++)
                data[i] = other.data[i];
            std::cout << "~~ copy ctor ~~\n";
        }

        Foo& operator=(const Foo &other) {

            if (this != std::addressof(other)) {
                delete[] data;

                name = other.name;
                size = other.size;
                data = new double[size];

                for (int i = 0; i < size; i++)
                    data[i] = other.data[i];
            }
            std::cout << "~~ operator=(const&) ~~\n";

            return *this;
        }

        Foo(Foo &&other) noexcept
            : name{ std::move(other.name) }
            , size{ other.size }
            , data{ std::exchange(other.data, nullptr) }
        {
            std::cout << "~~ move ctor ~~\n";
        }

        Foo& operator=(Foo &&other) noexcept {

            std::swap(name, other.name);
            std::swap(size, other.size);
            std::swap(data, other.data);

            std::cout << "~~ operator=(&&) ~~\n";

            return *this;
        }

};

void process(const Foo &f) {
    std::cout << "~~ process(const &) ~~\n";
}

void process(Foo &&f) {
    std::cout << "~~ process(&&) ~~\n";
}

//void proxy(const Foo &f) {
//    std::cout << "~~ proxy(const &) ~~\n";
//    process(f);
//}

//void proxy(Foo &&f) {
//    std::cout << "~~ proxy(&&) ~~\n";
//    process(std::move(f));
//}

template<typename T>void proxy(T &&p) {
    // TD<T> t;                    // <--- uncomment this to see the of T for various cases
    // TD<decltype(p)> t_param;    // <--- uncomment this to see the type of f for various cases

    std::cout << "~~ template proxy(&&) ~~\n";
    process(std::forward<T>(f));  // <--- do not use, after forwarding
}

int main() {

    {
        Foo f1{ "almafa", 42 };
        Foo f2 = f1;
        Foo f3 = std::move(f1);
        Foo f4 = Foo{ Foo{ "kortefa", 55 } };

        std::cout << "=============================\n\n";

        proxy(f2);
        proxy(Foo{ "szilvafa", 14 });
        
        std::cout << "\n\n=============================\n\n";

    }
}

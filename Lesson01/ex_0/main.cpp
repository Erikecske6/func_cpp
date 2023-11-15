#include <iostream>
#include <vector>

#include <string>
#include <utility>

class Foo {

    private:

        std::size_t size;
        int         *data;
        std::string name;

    public:

        Foo(const std::string &name, std::size_t size)
            : size{ size }
            , data{ new int[size] }
            , name{ name }
        {
            std::cout << "~~ ctor ~~\n";
        }

        ~Foo() {
            std::cout << "~~ dtor ~~\n";

            delete[] data;
        }

        /// Copy ctor.
        Foo(const Foo &other)
            : size{ other.size }
            , data{ new int[size] }
            , name{ other.name }
        {
            for(std::size_t i = 0; i < size; i++)
                data[i] = other.data[i];

            std::cout << "~~ copy ctor ~~\n";
        }

        /// Copy assignment operator.
        Foo& operator=(const Foo &other) {
            std::cout << "~~ operator=(const&) ~~\n";

            if(this != &other) {
                delete[] data;

                size = other.size;
                data = new int[size];
                for(std::size_t i = 0; i < size; i++)
                    data[i] = other.data[i];
                name = other.name;
            }
            return *this;
        }

        /// Move ctor.
        /// Try it with and without noexcept.
        Foo(Foo &&other) noexcept
            : size{ other.size }
            , data{ std::exchange(other.data, nullptr) }   // grab the resource 
            , name{ std::move(other.name) }
        {
            std::cout << "~~ move ctor ~~\n";
        }

        /// Move asssignment operator.
        /// Try it with and without noexcept.
        Foo& operator=(Foo &&other) noxcept {
            std::cout << "~~ operator=(const&&) ~~\n";

            // We do not test this != &other. Is this a good solution?
            
            std::swap(size, other.size);  // Note that as soon
            std::swap(data, other.data);  // as other is destructed our
            std::swap(name, other.name);  // original resources are freed

            return *this;
        }

};  // class Foo

/// Move constructor exmaples
int main() {

    std::vector<Foo> v;

    Foo foo1{ "almafa", 42 };

    v.push_back(foo1);
    v.push_back(Foo{ "kortefa", 12 });
    v.push_back(std::move(foo1));

    v.emplace_back("szilvafa", 74);

    return 0;
}

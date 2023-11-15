#include <iostream>

struct Foo {

    private:

        bool x = false;
        bool y = false;
        bool z = false;

    public:

        // in the next three line we define the tags;
        // empty classes that can be used to differentiate
        // between functions later (with overloading)
        struct TagX {};
        struct TagY {};
        struct TagZ {};

        // we can even create static variables to make
        // the code look more beautiful
        constexpr static TagX tagX{};

        // the next three methods have the same name, but
        // the tag, the first parameter helps differenciate
        // them...
        
        void tune(TagX, bool val) { x = val; }
        void tune(TagY, bool val) { y = val; }
        void tune(TagZ, bool val) { z = val; }
};

int main() {
    Foo f;

    f.tune(Foo::TagY{}, true); //<--- a bit ugly
    f.tune(Foo::tagX, true);   //<--- but with the static
                               // variable is less obscure
}

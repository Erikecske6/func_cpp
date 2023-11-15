#include <type_traits>
#include <iostream>
#include <string>

template<typename T>
auto algo(T t) -> typename std::enable_if<!std::is_integral<T>::value, void>::type {
    std::cout << "Slow algorithm\n";
}

template<typename T>
auto algo(T t) -> typename std::enable_if<std::is_integral<T>::value, void>::type {
    std::cout << "Fast algorithm\n";
}


/// BAD solution; when instantiating the static functions the T is already set,
/// e.g., no SFINAE will take place
///template<typename T>
///struct Foo {
///
///	static auto algo(T t) -> typename std::enable_if<!std::is_integral<T>::value, void>::type {		
///		std::cout << "Slow algorithm\n";
///	}
///
///	static auto algo(T t) -> typename std::enable_if<std::is_integral<T>::value, void>::type {		
///		std::cout << "Fast algorithm\n";
///	}
///};


template<typename T>
struct Foo_AlmostOK {

    template<typename U = T>
    static auto algo(U t) -> typename std::enable_if<!std::is_integral<U>::value, void>::type {		
		    std::cout << "Slow algorithm\n";
    }

    template<typename U = T>
    static auto algo(U t) -> typename std::enable_if<std::is_integral<U>::value, void>::type {
        std::cout << "Fast algorithm\n";
    }
};

template<typename T>
struct Foo_OK {

    template<typename U = T>
    static auto algo(U t) -> typename std::enable_if<!std::is_integral<U>::value, void>::type {
        // first we prescribe that U and T are always the same
        static_assert(std::is_same<U, T>::value, "You shoudn't define the template parameter for this method.");
      
        std::cout << "Slow algorithm\n";
    }

    template<typename U = T>
    static auto algo(U t) -> typename std::enable_if<std::is_integral<U>::value, void>::type {
        // first we prescribe that U and T are always the same
        static_assert(std::is_same<U, T>::value, "You shoudn't define the template parameter for this method.");

        std::cout << "Fast algorithm\n";
    }
};

int main() {

    {
        algo(42);      //<--- calls the fast algorithm
        algo(3.1415);  //<--- calls the slow algorithm
    }

    {
        Foo_AlmostOK<int>::algo(42.5);  //<--- calls the slow algorithm
                                        // because we defined U as double
                                        // counterintuitive :D

        Foo_AlmostOK<int>::algo<double>(42);  //<--- calls the slow one
        Foo_AlmostOK<int>::algo(42);          //<--- calls the fast one
    }

    {
        //Foo_OK<int>::algo(42.5);   //<--- compile time error
        //                           // due to the static assert
        //Foo_OK<double>::algo(42);  //<--- same as before

        Foo_OK<double>::algo(42.5);        //<--- calls the slow one
        Foo_OK<double>::algo<double>(42);  //<--- calls the slow one
        Foo_OK<int>::algo(42);             //<--- calls the fast one
    }
}

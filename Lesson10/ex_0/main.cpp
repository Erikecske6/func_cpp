#include <vector>
#include <iostream>
#include <ranges>

bool isPrime(int i) {
    for(int j = 2; j * j <=i;++j)
        if(i % j == 0) return false;
    return true;
}


int main() {

    // print the square of the even number contained in a vector
    {
        std::vector<int> v {1,2,3,4,5,6,7,8,9};

        auto even   = [](int i) { return 0 == i % 2; };        
        auto square = [](int i) { return i * i; };

        for(int i : v | std::views::filter(even) | std::views::transform(square)) {
            std::cout << i << " ";
        }

    }

    // print the square of some even numbers; take the first 10
    {
        auto even   = [](int i) { return 0 == i % 2; };        
        auto square = [](int i) { return i * i; };

        for(int i : std::views::iota(1) | std::views::filter(even) | std::views::transform(square) | std::views::take(10) ) {
            std::cout << i << "\n";
        }


    }

    // number of word with ranges
    {
        auto text  = "Lorem ipsum dolor sit amet";
        auto count = std::ranges::distance(
             std::string_view(text) | std::views::split(' ')
        ); 
        std::cout << "Number of words: " << count << "\n";

    }

    // print the odd numbers from the given interval
    {
        auto odd = [](int i){ return i % 2 == 1; };
        std::cout << "Odd numbers from 1000000 to 1001000 (displayed each 100th): " << std::endl;
        for (int i: std::views::iota(1000000, 1001000) | std::views::filter(odd)) {
            if (i % 100 == 1)
                std::cout << i << " ";  
        }
    }

    // and now the primes and the first 20 primes after 1000000
    {
        std::cout << "\n";
        auto odd = [](int i){ return (i % 2 == 1); };
        for(int i : std::views::iota(1000000, 1001000) | std::views::filter(odd)  | std::views::filter(isPrime)) {
            std::cout << i << " ";
        }

        for(int i : std::views::iota(1000000) | std::views::filter(odd)  | std::views::filter(isPrime) | std::views::take(20)) {
            std::cout << i << " ";
        }
    }
}

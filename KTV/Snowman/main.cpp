#include <array>
#include <iostream>


// This will run at runtime, a simple write-out function
template<std::size_t W, std::size_t H>
void  print(const std::array<int, W * H> &arr) {
    for (int i = 0; i < H; i++) {
        for (int j = 0; j < W; j++) {
            std::cout << arr[i * W + j];
        }
        std::cout << '\n';
    }
}


// Out simple shape
template<int X0, int Y0, int R>
struct Circle {
    constexpr static inline auto draw(int x, int y) {
        return (x - X0) * (x - X0) + (y - Y0) * (y - Y0) < R * R;
    }
};


// Union of two shapes...
template<typename Shape1, typename Shape2>
struct Union {
    constexpr static inline auto draw(int x, int y) {
        return Shape1::draw(x, y) || Shape2::draw(x, y);
    }
};


// ...but we can do better. Here's the variadic version of it.
template<typename ...Shapes>
struct UnionV {
    constexpr static inline auto draw(int x, int y) {
        return (Shapes::draw(x, y) ||...);  // C++17 fold expression
    }
};


// Helper to put the scene into the std::array
template<std::size_t W, std::size_t H, typename Shape>
constexpr auto createScene() {
    std::array<int, W * H> arr{};
    for (int i = 0; i < H; i++) {
        for (int j = 0; j < W; j++) {
            arr[i * W + j] = Shape::draw(i, j);
        }
    }
    return arr;
}


int main() {

    {
        constexpr std::size_t W = 10;
        constexpr std::size_t H = 10;

        constexpr std::array<int, W * H> scene = createScene<W, H, Circle<0, 0, 5>>();
        print<W, H>(scene);
    }
    
    std::cout << '\n';
    
    {
        constexpr std::size_t W = 10;
        constexpr std::size_t H = 10;

        constexpr std::array<int, W * H> scene = createScene<W, H, Union<Circle<0, 0, 5>, Circle<10, 10, 3>>>();
        print<W, H>(scene);
    }

    std::cout << "\nAnd now, and upside down snowman:\n";
    
    {
        constexpr std::size_t W = 30;
        constexpr std::size_t H = 30;

        // we use the variadic version of the union
        constexpr std::array<int, W * H> scene = createScene<W, H, UnionV<
                                                                          Circle< 5, 15, 7>,
                                                                          Circle<15, 15, 5>,
                                                                          Circle<21, 15, 3>
	                                                                 >>();

        print<W, H>(scene);
    }

}

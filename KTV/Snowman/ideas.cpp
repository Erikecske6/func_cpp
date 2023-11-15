#include <array>

#include <iostream>

constexpr int pic(int x, int y) {
    return x + y < 7;
}

template<std::size_t W, std::size_t H>
constexpr auto picture() {
    std::array<int, W * H> arr{};
    for (int i = 0; i < H; i++) {
        for (int j = 0; j < W; j++) {
            arr[i * W + j] = pic(i, j);
        }
    }
    return arr;
}


template<std::size_t W, std::size_t H>
void  print(const std::array<int, W * H> &arr) {
    for (int i = 0; i < H; i++) {
        for (int j = 0; j < W; j++) {
            std::cout << arr[i * W + j];
        }
        std::cout << '\n';
    }
}


int main() {

    constexpr std::size_t W = 10;
    constexpr std::size_t H = 10;

    constexpr std::array<int, W * H> scene = picture<W, H>();

    print<W, H>(scene);
}

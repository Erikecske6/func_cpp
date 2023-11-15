#include <iostream>
#include <functional>

#include <thread>
#include <future>

// std::function<ret-val(parameters)> fun;

//  int fun(int, int);
//  std::thread th{ [](int i, int j){ return i * j; }, 5, 7 };
//
//  th.join();

// general
template<typename>class Task;

// and now we specialize it
template<typename R, typename ...Args>
class Task<R(Args...)> {

    private:

        std::function<R(Args...)> fun;
        std::promise<R>           prom;

    public:

        Task(const std::function<R(Args...)> &fun) : fun{ fun }{}
        Task(std::function<R(Args...)> &&fun) : fun{ std::move(fun) } {}

        std::future<R> get_future() {
            return prom.get_future();
        }

        template<typename ...T>
        void operator()(T&& ...t) {
            try {
                prom.set_value(fun(std::forward<T>(t)...));
            }
            catch (...) {
                prom.set_exception(std::current_exception());
            }
        }
};


int main() {

    Task<int(int, int)> ts{ [](int i, int j) { throw 42; return i * j; } };
    std::future<int> fut = ts.get_future();

    std::thread th{ std::move(ts), 5, 7 };
    try {
        std::cout << "Res: " << fut.get() << "\n";
    }
    catch (...) {
        std::cout << "exception...\n";
    }

    th.join();
}

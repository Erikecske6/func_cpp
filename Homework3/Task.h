#ifndef TASK_H
#define TASK_H

#include <vector>

struct Foo {};

class Bar {
    private:
        std::vector<Foo> storage;

    public:
        /* TASK: this function must be a function template! */
        /* TASK: complete signature declaration /
        /* TASK: provide meaningful compiler error/diagnostic if called with not a Foo object as argument */
        void add(/*TASK: complete parameter list*/) {
            /* TASK: efficiently add the parameter to the storage */
        }
};

class BarProxy {
    private:
        Bar &bar;

    public:
        BarProxy(Bar &bar) : bar{bar} {}

        void add(const Foo &foo) {
            /* TASK: use bar's add member func to add the elem to it efficiently */
        }

        void add(Foo &&foo) {
            /* TASK: use bar's add member func to add the elem to it efficiently */
        }
};

#endif  /* TASK_H */

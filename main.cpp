#include <any>
#include <iostream>

#include "Wrapper.h"
#include "Engine.h"
class Subject {
    public:
    int x;
    int y;
    Subject(int x, int y) : x(x), y(y) {}

    int f3 (int x1, int y1) {
        std::cout << "x = " << x << " x1 = " << x1 << " y = " << y << " y1 = " << y1 << std::endl;
        x += x1;
        y += y1;
        std::cout << x << ' ' << y << std::endl;
        return x + y;
    }

    void f4 (int x1, int y1) {
        std::cout << "x = " << x << " x1 = " << x1 << " y = " << y << std::endl;
        //return 11;
    }
};

int main()
{
    Subject subj = Subject(1, 2);
    Wrapper wrapper(&subj, &Subject::f3, {{"arg1", 0}, {"arg2", 0}});
    Engine engine;
    engine.registerCommand(&wrapper, "command1");
    engine.execute("command1", {{"arg1", 4}, {"arg2", 5}});
}


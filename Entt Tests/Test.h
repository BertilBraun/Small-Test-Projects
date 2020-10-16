#pragma once

#include "System.h"
#include <iostream>

struct TestComponent {

    int i = 10;

};


class Test : public System<TestComponent> { // Init with all Components needed
public:
    virtual void update(TestComponent& t) {
        std::cout << "CALLED! " << t.i << std::endl;
    }

};

//class TestInt : public System<int, int> { // Init with all Components needed
//public:
//    virtual void update(int& i, int& j) {
//        std::cout << "CALLED Int! " << i << " " << j << std::endl;
//    }
//
//};

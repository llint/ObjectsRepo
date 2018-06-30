//
//  main.cpp
//  ObjectsRepo
//
//  Created by Lin Luo on 6/29/18.
//  Copyright © 2018 Linfinity. All rights reserved.
//

#include <iostream>
#include <cassert>

#include <string>
#include <vector>

#include "ObjectsRepository.h"

struct S {
    using ObjRef = pi::ObjectsRepository<S>::ObjRef;

    std::vector<ObjRef> vors;

    int i = 42;
    std::string s = "hello";

    S() = default;
    S(int i, const std::string& s) : i(i), s(s) {}

    S(const S&) = delete;
    S(S&&) = delete;
};

void Test_Int() {
    auto objr = pi::ObjectsRepository<int>::Singleton().createObject(42);
    auto objr2 = pi::ObjectsRepository<int>::Singleton().createObject(*objr);
    *objr = 84;
    assert(*objr2 == 42);
    auto objr3 = objr;
    objr.release();
    assert(!objr);
}

void Test_Struct() {
    auto objr = pi::ObjectsRepository<S>::Singleton().createObject();
    auto objr2 = pi::ObjectsRepository<S>::Singleton().createObject(33, "world");
}

int main(int argc, const char * argv[]) {
    // insert code here...
    std::cout << "Hello, World!\n";
    Test_Int();
    Test_Struct();
    return 0;
}

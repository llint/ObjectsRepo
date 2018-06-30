//
//  main.cpp
//  ObjectsRepo
//
//  Created by Lin Luo on 6/29/18.
//  Copyright © 2018 Linfinity. All rights reserved.
//

#include <iostream>
#include <cassert>

#include "ObjectsRepository.h"

void Test_Int() {
    auto objr = pi::ObjectsRepository<int>::Singleton().createObject(42);
    auto objr2 = pi::ObjectsRepository<int>::Singleton().createObject(*objr);
    *objr = 84;
    assert(*objr2 == 42);
    objr.destroy();
    assert(!objr);
}

int main(int argc, const char * argv[]) {
    // insert code here...
    std::cout << "Hello, World!\n";
    Test_Int();
    return 0;
}

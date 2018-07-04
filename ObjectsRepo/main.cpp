//
//  main.cpp
//  ObjectsRepo
//
//  Created by Lin Luo on 6/29/18.
//  Copyright © 2018 Linfinity. All rights reserved.
//

#include <cassert>
#include <iostream>

#include <string>
#include <vector>

#include "ObjectsRepository.h"

void Test_Int() {
  auto objr = pi::ObjectsRepository<int>::Singleton().createObject(42);
  auto objr2 = pi::ObjectsRepository<int>::Singleton().createObject(*objr);
  *objr = 84;
  assert(*objr2 == 42);
  auto objr3 = objr;
  objr.release();
  assert(!objr);
  assert(objr3);
  auto objr4 = std::move(objr3);
  assert(!objr3);
}

struct S {
  using ObjRef = pi::ObjectsRepository<S>::ObjRef;

  std::vector<ObjRef> vors;

  int i = 42;
  std::string s = "hello";

  S() = default;
  S(int i, const std::string &s) : i(i), s(s) {}

  S(const S &) = delete;
  S(S &&) = delete;
};

void Test_Struct() {
  auto objr = pi::ObjectsRepository<S>::Singleton().createObject();
  auto objr2 = pi::ObjectsRepository<S>::Singleton().createObject(33, "world");

  std::vector<S::ObjRef> vors;
  vors.push_back(objr);
  vors.push_back(objr2);

  for (size_t i = 0; i < 100; ++i) {
    auto o = pi::ObjectsRepository<S>::Singleton().createObject();
    o->vors.push_back(o);
  }

  pi::ObjectsRepository<S>::Singleton().clear();

  auto o = pi::ObjectsRepository<S>::Singleton().createObject();
  o.destroy();
  o.release();
}

int main(int argc, const char *argv[]) {
  // insert code here...
  std::cout << "Hello, World!\n";
  Test_Int();
  Test_Struct();
  return 0;
}

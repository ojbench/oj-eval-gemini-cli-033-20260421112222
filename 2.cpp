// Provided by Lhtie 2022.2

#include "class-bint.hpp"
#include "class-integer.hpp"
#include "class-matrix.hpp"
#include "src.hpp"


#include <iostream>
#include <list>

const int N = 5e4;

int ansCounter = 0, myCounter = 0, noUseCounter = 0;
class DynamicType {
public:
  int *pct;
  int *data;
  DynamicType(int *p) : pct(p), data(new int[2]) { (*pct)++; }
  DynamicType(int *p, int x) : pct(p), data(new int[2]) {
    (*pct)++;
    data[0] = x;
  }
  DynamicType(const DynamicType &other) : pct(other.pct), data(new int[2]) {
    (*pct)++;
    data[0] = other.data[0];
  }
  DynamicType &operator=(const DynamicType &other) {
    if (this == &other)
      return *this;
    (*pct)--;
    pct = other.pct;
    (*pct)++;
    delete[] data;
    data = new int[2];
    data[0] = other.data[0];
    return *this;
  }
  ~DynamicType() {
    delete[] data;
    (*pct)--;
  }
  bool operator==(const DynamicType &rhs) const {
    return data[0] == rhs.data[0];
  }
  bool operator!=(const DynamicType &rhs) const {
    return data[0] != rhs.data[0];
  }
  bool operator<(const DynamicType &rhs) const { return data[0] < rhs.data[0]; }
};

template <typename T>
bool equal(const std::list<T> &x, const sjtu::list<T> &y) {
  if (x.empty() && y.empty()) {
    return true;
  }
  return x.front() == y.front() && x.back() == y.back() && x.size() == y.size();
}

bool testPush() {
  {
    std::list<DynamicType> ans;
    sjtu::list<DynamicType> myList;

    for (int i = 0; i < N; ++i) {
      if (rand() % 2) {
        ans.push_back(DynamicType(&ansCounter));
        myList.push_back((DynamicType(&myCounter)));
      } else {
        ans.push_front(DynamicType(&ansCounter));
        myList.push_front((DynamicType(&myCounter)));
      }
    }

    if (myCounter != ansCounter)
      return false;
  }
  return myCounter == ansCounter;
}

bool testPop() {
  {
    std::list<DynamicType> ans;
    sjtu::list<DynamicType> myList;
    for (int i = 0; i < N; ++i) {
      ans.push_back(DynamicType(&ansCounter));
      myList.push_back(DynamicType(&myCounter));
    }

    for (int i = 0; i < rand() % N; ++i) {
      if (rand() % 2) {
        ans.pop_back();
        myList.pop_back();
      } else {
        ans.pop_front();
        myList.pop_front();
      }
    }

    if (myCounter != ansCounter)
      return false;
  }
  return myCounter == ansCounter;
}

int main() {
  srand(time(NULL));
  bool (*testList[])() = {testPush, testPop};
  const char *Messages[] = {"Test 1: Testing push_front() & push_back() and "
                            "check number of live objects...",
                            "Test 2: Testing pop_front() & pop_back() and "
                            "check number of live objects..."};

  bool okay = true;
  for (int i = 0; i < sizeof(testList) / sizeof(testList[0]); ++i) {
    printf("%s", Messages[i]);
    if (testList[i]()) {
      printf("Passed\n");
    } else {
      okay = false;
      printf("Failed\n");
    }
  }

  if (okay)
    printf("Congratulations, you have passed all tests!\n");
  else
    printf("Unfortunately, you failed in some of the tests.\n");
  return 0;
}
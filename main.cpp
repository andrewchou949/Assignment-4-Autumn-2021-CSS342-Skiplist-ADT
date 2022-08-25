#include <cassert>
#include <iostream>

#include "skiplist.h"

using namespace std;

void test1() {
  // setting seed for easy testing
  srand(0);
  SkipList skp(5, 80);
  cout << boolalpha;
  skp.add(3);
  skp.add(5);
  // cannot add existing
  assert(!skp.add(vector<int>{1, 7}));
  skp.add(1);
  cout << skp << endl;
  cout << "7 ? " << skp.contains(7) << endl;
  cout << "3 ? " << skp.contains(3) << endl;
  cout << "-100 ? " << skp.contains(-100) << endl;
  cout << "100 ? " << skp.contains(100) << endl;
  bool result = skp.remove(5);
  cout << skp << endl;
  assert(result);
  result = skp.remove(5);
  assert(!result);
  cout << "1 ?" << skp.contains(1) << endl;
  cout << skp << endl;
}

int main() {

  test1();

  cout << "Done." << endl;
  return 0;
}
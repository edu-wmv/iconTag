#include <iostream>
using namespace std;

class Check_in {
  public: 
    int tag_code;
    int time;
};

Check_in check;

void checkTag() {
  check.tag_code = 4;
  check.time = 5;

  cout << "Ok"; 
}

int main() {
  checkTag();

  return 0;
}
#include <iostream>
#include <memory>
#include <string>

using namespace std;

class person {

   public:
      person(string name):name(name){}

      void tell() { 
          cout << "Hi, I am " << name << endl;
      }

      ~person() {
          cout << "bye!" << endl;
      }

private:
string name;

};


int main(void) {

     person* p = new person("Cici");
     p->tell();
     delete p;

     auto_ptr<person> q(new person("haha"));
     q->tell();
}



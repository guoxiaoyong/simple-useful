#include <iostream>
#include <type_traits>


struct Box {
  static int num_box;

  int id;

  Box() {
    num_box++;
    id = num_box;
    printf("Box (%d) created! num_box = %d\n", id, num_box);
  }

  ~Box() {
    num_box--;
    printf("Box (%d) destroyed! num_box = %d\n", id, num_box);
  }
};

int Box::num_box = 0;

template<typename T>
size_t GetSize(T) {
  return sizeof(T);
}

template<typename T>
size_t GetSize(T* t) {
  return sizeof(*t);
}

template<typename T, size_t N>
size_t GetSize(T t[N]) {
  return N;
}

// extern char a[];
// char a[] = "hello";

int main() {
  std::cout << a << std::endl;
  //std::unique_ptr<Box[]> ptr(new Box[5]);

  char str[10];
  printf("size = %lu\n", GetSize(str));
  printf("size = %lu\n", GetSize<char, 10>(str));

  int Box::*kk[10];
  return 0;
}

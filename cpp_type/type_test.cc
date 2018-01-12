#include <iostream>
#include <memory>
#include <type_traits>

typedef int function_type(int, int);
typedef char char_array_with_no_length[];

// char_array_with_no_length arr;  // Not allowed
char_array_with_no_length str = "hello world!";

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

template <typename T> size_t GetSize(T) { return sizeof(T); }

template <typename T> size_t GetSize(T *t) { return sizeof(*t); }

template <typename T, size_t N> size_t GetSize(T t[N]) { return N; }

template <typename T> struct Forwarder { typedef T type; };

int add(int a, int b) { return a + b; }

int main() {
  std::unique_ptr<Box[]> ptr(new Box[5]);
  char str[10];
  printf("size = %lu\n", GetSize(str));
  printf("size = %lu\n", GetSize<char, 10>(str));

  typename Forwarder<int>::type a;
  typename Forwarder<Box>::type b;
  typename Forwarder<char[]>::type c = "hello";

  typename Forwarder<int(int, int)>::type *d = add;
  printf("res = %d\n", d(3, 4));

  typename Forwarder<int(int, int)>::type e_add;
  printf("res = %d\n", e_add(3, 4));

  decltype(add) f_add;
  printf("res = %d\n", f_add(3, 4));

  return 0;
}

int e_add(int a, int b) { return a + b + 10; }

int f_add(int a, int b) { return a + b + 20; }

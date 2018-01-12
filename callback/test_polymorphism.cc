#include <iostream>
#include <memory>

struct Box {
  static int num_box_created;
  static int num_box_destroyed;
  int box_id;

  Box() {
    box_id = num_box_created++;
    std::cout << ToString() << " created!\n";
  }

  virtual ~Box() {
    num_box_destroyed++;
    std::cout << ToString() << " destroyed\n";
  }

  std::string ToString() {
    char result[1024];
    sprintf(result, "Box(id=%d, num_box_created=%d, num_box_destroyed=%d)",
            box_id, num_box_created, num_box_destroyed);
    return std::string(result);
  }

  int width;
  int height;
};
int Box::num_box_created = 0;
int Box::num_box_destroyed = 0;

struct Pencil {
  static int num_pencil_created;
  static int num_pencil_destroyed;
  int pencil_id;
  Pencil() {
    pencil_id = num_pencil_created++;
    std::cout << ToString() << " created!\n";
  }

  virtual ~Pencil() {
    num_pencil_destroyed++;
    std::cout << ToString() << " destroyed!\n";
  }

  std::string ToString() {
    char result[1024];
    sprintf(result,
            "Pencil(id=%d, num_pencil_created=%d, num_pencil_destroyed=%d)",
            pencil_id, num_pencil_created, num_pencil_destroyed);
    return std::string(result);
  }

  int length;
  std::string brand;
};
int Pencil::num_pencil_created = 0;
int Pencil::num_pencil_destroyed = 0;

struct PencilBox : public Box, public Pencil {
  PencilBox() { std::cout << "pencil box created!\n"; }

  virtual ~PencilBox() { std::cout << "pencil box destroyed\n"; }
};

void check_addr(const Pencil *pen) {
  std::cout << "Addr of Pencil object:" << pen << std::endl;
}

void delete_obj(Pencil *pen) {
  std::cout << "Addr of Pencil object:" << pen << std::endl;
  delete pen;
}

int main(void) {
  {
    std::cout << "=======\n";
    std::cout << "Test Box started.\n";
    std::unique_ptr<Box[]> boxes(new Box[5]);
  }
  std::cout << "Test Box ended.\n";
  std::cout << "=======\n";

  {
    std::cout << "=======\n";
    std::cout << "Test Pencil started.\n";
    std::unique_ptr<Pencil[]> pencils(new Pencil[5]);
  }
  std::cout << "Test Pencil ended.\n";
  std::cout << "=======\n";

  {
    std::cout << "=======\n";
    std::cout << "Test PencilBox started.\n";
    std::unique_ptr<PencilBox[]> pencilboxes(new PencilBox[5]);
  }
  std::cout << "Test PencilBox ended.\n";
  std::cout << "=======\n";

  {
    std::cout << "=======\n";
    std::cout << "Test Polymorphism started.\n";
    std::unique_ptr<Pencil> pencil(new PencilBox());
  }
  std::cout << "Test Polymorphism ended.\n";
  std::cout << "=======\n";

  PencilBox pb;
  std::cout << "Addr of PencilBox object: " << &pb << std::endl;
  check_addr(&pb);

  auto another_pb = new PencilBox();
  std::cout << "Addr of PencilBox object: " << another_pb << std::endl;
  delete_obj(another_pb);
  return 0;
}

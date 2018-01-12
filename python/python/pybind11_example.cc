#include <pybind11/pybind11.h>
#include <pybind11/eval.h>
#include <pybind11/embed.h>

// There are some error caused by the definition of the _POSIX_C_SOURCE.
// So I have to put iostream after above includes.
#include <iostream>

enum Side { BUY, SELL };

struct Order {
  std::string symbol;
  Side side;
  int size;
  double price;
  int order_id;
};

class OrderGateway;

class Strategy {
 public:
  Strategy(OrderGateway* order_gateway) : order_gateway_(order_gateway) {}
  virtual ~Strategy() = default;

  virtual void Eval() = 0;
  virtual void OnOrder(const Order&) = 0;
  void SendOrder(
      const std::string& symbol,
      Side side, int size, double price);

 private:
  OrderGateway* order_gateway_;
};

class OrderGateway {
 public:
  void SendOrder(
      Strategy& strategy,
      const std::string& symbol,
      Side side,
      int size,
      double price) {
    std::cout << "Sending order to the market......\n";
    Order order{symbol, side, size, price, ++next_order_id_};
    strategy.OnOrder(order);
  }

 private:
  int next_order_id_ = 0;
};

void Strategy::SendOrder(
    const std::string& symbol,
    Side side, int size, double price) {
  order_gateway_->SendOrder(*this, symbol, side, size, price);
}

namespace py = pybind11;

class PyStrategy final : public Strategy {
  using Strategy::Strategy;

  void Eval() override {
    PYBIND11_OVERLOAD_PURE(
        void,      // return type
        Strategy,  // parent class
        Eval       // function name
        );
  }

  void OnOrder(const Order& order) override {
    PYBIND11_OVERLOAD_PURE_NAME(
        void,       // return type
        Strategy,   // parent class
        "on_order", // python function name
        OnOrder,    // function name
        order       // args
        );
  }
};

PYBIND11_EMBEDDED_MODULE(StrategyFramework, m) {
  m.doc() = "Example strategy framework";
  py::enum_<Side>(m, "Side").value("BUY", BUY).value("SELL", SELL);

  py::class_<Order>(m, "Order")
      .def_readonly("symbol", &Order::symbol)
      .def_readonly("side", &Order::side)
      .def_readwrite("size", &Order::size)
      .def_readwrite("price", &Order::price)
      .def_readonly("order_id", &Order::order_id);

  py::class_<OrderGateway>(m, "OrderGateway");

  py::class_<Strategy, PyStrategy>(m, "Strategy")
      .def(py::init<OrderGateway*>())
      .def("send_order", &Strategy::SendOrder);
}

int main() {
  py::scoped_interpreter guard{};
  try {
    auto strategy_module =  py::module::import("strategy");
    py::object Strategy = strategy_module.attr("MyStrategy");
    OrderGateway order_gateway;
    //py::object strategy = Strategy(&order_gateway);
    //strategy.attr("eval")();
    return 0;
  } catch (const py::error_already_set&) {
    std::cerr << ">>> Error! Uncaught exception:\n";
    PyErr_Print();
    return 1;
  }
}

#include <iostream>
#include <boost/python.hpp>

enum Side {
  BUY,
  SELL,
};

struct Order {
  std::string symbol;
  Side side;
  int size;
  double price;
  int order_id;
};

// forward declaration
class StrategyInstance;

class StrategyServer {
 public:
  void sendOrder(
      StrategyInstance&,
      const std::string& symbol,
      Side, int size, double price);

 private:
  int _next_order_id = 0;
};

class StrategyInstance {
 public:
   StrategyInstance(StrategyServer&);
   virtual ~StrategyInstance() = default;
   virtual void eval() = 0;
   virtual void onOrder(const Order&) = 0;
   void sendOrder(
       const std::string& symbol, Side, int size, double price);

 private:
  StrategyServer& _server;
};

///////////////////////////////////
// implementation
void StrategyServer::sendOrder(
    StrategyInstance& instance,
    const std::string& symbol, Side side, int size, double price) {
  // simulate sending an order, receiving an acknowledgement
  // and calling back to the strategy instance
  std::cout << "sending order to market\n";
  Order order { symbol, side, size, price, ++_next_order_id };
  instance.onOrder(order);
}

///////////////////////////////////
StrategyInstance::StrategyInstance(StrategyServer& server)
: _server(server) {}

void StrategyInstance::sendOrder(
    const std::string& symbol, Side side, int size, double price) {
  _server.sendOrder(*this, symbol, side, size, price);
}

////////////////////////////////////
// export to python
namespace bp = boost::python;

class PyStrategyInstance final
    : public StrategyInstance,
      public bp::wrapper<StrategyInstance> {

  using StrategyInstance::StrategyInstance;

  void eval() override {
    get_override("eval")();
  }

  void onOrder(const Order& order) override {
    get_override("on_order")(order);
  }
};

BOOST_PYTHON_MODULE(StrategyFramework) {
    bp::enum_<Side>("Side")
        .value("BUY", BUY)
        .value("SELL", SELL)
        ;

    bp::class_<Order>("Order")
        .def_readonly("symbol",   &Order::symbol)
        .def_readonly("side",     &Order::side)
        .def_readwrite("size",    &Order::size)
        .def_readwrite("price",   &Order::price)
        .def_readonly("order_id", &Order::order_id)
        ;

    bp::class_<StrategyServer>("StrategyServer")
        ;

    bp::class_<PyStrategyInstance, boost::noncopyable>("StrategyInstance", bp::init<StrategyServer&>())
        .def("send_order", &StrategyInstance::sendOrder)
        ;
}

bp::object import(
    const std::string& module,
    const std::string& path,
    bp::object& globals) {

  bp::dict locals;
  locals["module_name"] = module;
  locals["path"] = path;
  bp::exec("import imp\n"
           "new_module = imp.load_module(module_name, open(path), path, ('py', 'U', imp.PY_SOURCE))\n",
           globals, locals);
  return locals["new_module"];
}

//////////////////////////////////
int main() {
  try {
    Py_Initialize();
    PyImport_AppendInittab("StrategyFramework", &initStrategyFramework);
    StrategyServer server;
    bp::object main = bp::import("__main__");
    bp::object globals = main.attr("__dict__");
    bp::object module = import("strategy", "strategy.py", globals);
    bp::object Strategy = module.attr("Strategy");
    bp::object strategy = Strategy(server);
    strategy.attr("eval")();
    return 0;
  } catch(const bp::error_already_set&) {
    std::cerr << ">>> Error! Uncaught exception:\n";
    PyErr_Print();
    return 1;
  }
}

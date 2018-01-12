from StrategyFramework import *

class Strategy(StrategyInstance):
  def __init__(self, server):
    StrategyInstance.__init__(self, server)

  def eval(self):
    print "strategy.eval"
    self.send_order("GOOG", Side.BUY, 100, 759.11)

  def on_order(self, o):
    buy_or_sell = "buy" if o.side == Side.BUY else "sell"
    print "order for {} {} {}@{} has order_id={}".format(
        o.symbol, buy_or_sell, o.size, o.price, o.order_id)

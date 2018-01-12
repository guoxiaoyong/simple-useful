# Copyright (c) 2017 Xiaoyong Guo
# Author: xguo

import types
from tornado.gen import coroutine
from tornado.concurrent import Future
from tornado.ioloop import IOLoop


class Singleton(object):
  def __init__(self, decorated):
    self._decorated = decorated

  def get_instance(self):
    try:
      return self._instance
    except AttributeError:
      self._instance = self._decorated()
      return self._instance

  def __call__(self):
    raise TypeError('Singletons must be accessed through `get_instance()`.')

  def __instancecheck__(self, instance):
    return isinstance(instance, self._decorated)


# This class is intended for use in a single-threaded coroutine program.
@Singleton
class FutureManager(object):
  def __init__(self):
    self._futures = {}

  def get_future(self, key, fresh=False):
    future_list = self.get_future_list(key)
    if future_list:
      assert all(future.done() for future in future_list)
      if fresh is True:
        future = future_list[-1]
        future_list[:] = []
      else:
        future = future_list.pop(0)
      assert future.done()
    else:
      future = Future()
      future_list.append(future)
    return future

  def set_future(self, key, value=None):
    future_list = self.get_future_list(key)
    if ((future_list and all(future.done() for future in future_list))
        or len(future_list) == 0):
      future = Future()
      future_list.append(future)
    else:
      assert len(future_list) == 1
      assert not future_list[0].done()
    future_list[-1].set_result(value)

  def get_future_list(self, key):
    if key not in self._futures:
      self._futures[key] = list()
    future_list = self._futures[key]
    return future_list


class FutureManagerUser(object):
  def get_future(self):
    return FutureManager.get_instance().get_future(id(self))

  def set_future(self, value=None, key=None):
    if key:
      FutureManager.get_instance().set_future(key, value)
    else:
      FutureManager.get_instance().set_future(id(self), value)


def entity_run_decorator(run):
  def wrapper(self):
    gen = run(self)
    if isinstance(gen, types.GeneratorType):
      value = None
      try:
        while True:
          value = yield gen.send(value)
          if isinstance(value, EntityQuitMessage):
            break
      except StopIteration:
        pass
    EntityManager.get_instance().remove(self)
    self.finalize()
  wrapper.func_name = run.func_name
  return wrapper


class EntityQuitMessage(object):
  pass


@Singleton
class EntityManager(object):
  def __init__(self):
    self._entities = {}
    IOLoop.current().add_callback(self.check_quit)

  def add(self, entity):
    self._entities[id(entity)] = entity

  def remove(self, entity):
    del self._entities[id(entity)]

  @property
  def entities(self):
    return self._entities

  def is_ongoing(self):
    for key in self.entities:
      future_list = FutureManager.get_instance().get_future_list(key)
      if future_list and future_list[0].done():
        return True
    return False

  def check_quit(self, countdown=None):
    if countdown is None and self.is_ongoing():
      IOLoop.current().add_callback(self.check_quit)
    else:
      for entity in self.entities.values():
        entity.set_future(EntityQuitMessage())
      countdown = 5 if countdown is None else countdown-1
      if countdown > 0:
        IOLoop.current().add_callback(self.check_quit, countdown)
      else:
        IOLoop.current().add_callback(IOLoop.current().stop)


class Entity(FutureManagerUser):
  def __init__(self, *args, **kwargs):
    IOLoop.current().add_callback(
        coroutine(entity_run_decorator(type(self).run)), self)
    EntityManager.get_instance().add(self)
    self.init(*args, **kwargs)

  # Any entity can terminate the simulation.
  def exit(self):
    IOLoop.current().add_callback(IOLoop.current().stop)

  def send(self, entity, message):
    self.set_future(value=message, key=id(entity))

  def receive(self):
    return self.get_future()

  def init(self, *args, **kwargs):
    pass

  def finalize(self):
    pass

  def run(self):
    pass


class DriverEntity(Entity):
  def __init__(self):
    super(DriverEntity, self).__init__()
    IOLoop.current().start()

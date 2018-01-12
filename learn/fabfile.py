from fabric.api import local

def hello(name="world"):
  print "hello %s!" % name

def ls():
  local('ls -lh')

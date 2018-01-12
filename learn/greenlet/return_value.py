import greenlet

def test1(x, y):
    z, zz = gr2.switch(x+y)
    print(z, zz)

def test2(u):
    print(u)
    gr1.switch(42, 44)

gr1 = greenlet.greenlet(test1)
gr2 = greenlet.greenlet(test2)
gr1.switch("hello", " world")

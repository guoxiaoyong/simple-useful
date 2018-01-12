# based on http://perl.plover.com/lambda/jfp/lambda-church.pl

true = lambda x: lambda y: x
false = lambda x: lambda y: y
if_then_else = lambda condition: lambda true_clause: lambda false_clause: (condition)(true_clause)(false_clause)

succ = lambda n: lambda f: lambda x: f(n(f)(x))
add = lambda m,n: lambda f: lambda x: (m(f))(n(f)(x))
mul = lambda m,n: lambda f: m(n(f))
pred = lambda n: lambda f: lambda x: n(lambda g: lambda h: h(g(f)))(lambda u: x)(lambda u: u)
fac = lambda n: n(lambda c: lambda q: q(succ(c(true)))(mul(c(true), c(false))))(lambda q: succ(zero))(false)

zero = false
n0 = zero
n1 = succ(n0)
n2 = succ(n1)
n3 = succ(n2)
n4 = succ(n3)
n5 = succ(n4)
is_zero = lambda n: n(lambda k: false)(true)

# Defined according to:
# http://www.inf.fu-berlin.de/lehre/WS03/alpi/lambda.pdf
logical_and = lambda x: lambda y: (x)(y)(false)
logical_or = lambda x: lambda y: (x)(true)(y)
logical_not = lambda x: (x)(false)(true)
cond_test = lambda x: (x)(false)(logical_not)(false)
pair = lambda a: lambda b: lambda z: (z)(a)(b)  # similar to if_then_else
phi = lambda p: pair(succ(p(true)))(p(true))
predx = lambda n: (n)(phi)(pair(n0)(n0))(false)
greater_or_equal = lambda x: lambda y: cond_test((x)(predx)(y))
is_equal = lambda x: lambda y: logical_and(greater_or_equal(x)(y))(greater_or_equal(y)(x))
Y = lambda y: (lambda x: (y)((x)(x)))(lambda x: (y)((x)(x)))
sum_n = lambda r: lambda n: cond_test(n)(n0)((n)(succ)((r)(predx(n))))

if_then_else(is_zero(n0))(1)(2)
if_then_else(is_zero(n1))(1)(2)
to_int = lambda n: n(lambda x:x+1)(0)
to_boolean = lambda b: b(True)(False)

print "recursion"
print to_int(n4(sum_n)(n4))

print "if_then_else"
print to_int(if_then_else(true)(n1)(n2))
print to_int(if_then_else(false)(n1)(n2))

print "number"
print to_int(n3)

print "add"
print to_int(add(n1,n1))
print to_int(add(n1,n2))
print to_int(add(n3,n2))
print to_int(add(n3,succ(n2)))

print "factorial"
print to_int(fac(n5))

print "multiplication"
print to_int(mul(n4,n5))

print "predecessor"
print to_int(pred(n0))
print to_int(pred(n5))
print to_int(predx(n5))

print "logical"
print to_boolean(logical_and(true)(true))
print to_boolean(logical_and(true)(false))
print to_boolean(logical_or(true)(false))
print to_boolean(logical_not(false))
print to_boolean(logical_not(true))

print "cond_test"
print to_boolean(cond_test(n0))
print to_boolean(cond_test(n1))
print to_boolean(cond_test(n2))
print to_boolean(cond_test(n3))

print "greater_or_equal"
print to_boolean(greater_or_equal(n5)(n4))
print to_boolean(greater_or_equal(n4)(n5))

print "is_equal"
print to_boolean(is_equal(n4)(predx(n5)))
print to_boolean(is_equal(n4)(succ(n3)))

print "succ"
print to_int((n3)(succ)(n2))

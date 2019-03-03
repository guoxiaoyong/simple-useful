def add(long n):
  cdef long sum = 0
  cdef long x
  for x in range(n):
    sum += x
  return sum

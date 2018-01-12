for n in range(1, 10):
  line = ""
  for m in range(1, 10):
    eq = "%dx%d=%2d" % (n, m, n*m)
    line = "%s  %6s" % (line, eq)
  print line

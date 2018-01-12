
n = 2 * 4 ^ 5 - 3 ^ 3;
echo n;

n = 3;
while (n < 1000) {

  f = 0;
  m = 2;

  while (m < n) {

    if (n % m == 0)
      f = 1;

    m = m + 1;
  }

  if (f == 0)
    echo n;

  n = n + 1;
}

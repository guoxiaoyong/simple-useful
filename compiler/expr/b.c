
a = 1;
b = 1;
n = 0;

echo a;
echo b;

while (n < 10) {

  c = a + b;
  echo c;

  a = b;
  b = c;
  n = n + 1;
}

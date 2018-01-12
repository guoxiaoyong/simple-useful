function factorial(num) {
  if (num <=1) {
    return 1;
  } else {
    // return num * factorial(num-1);
    return num * arguments.callee(num-1);
  }
}

fac = factorial;
factorial = 10;

console.log(fac(4))
console.log(fac.length)

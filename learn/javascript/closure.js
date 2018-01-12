function RetFun() {
  var x = [10, 13];
  return function () {
    console.log(x);
    x.push(123);
  }
}

var a = RetFun();
var b = RetFun();

a();
b();

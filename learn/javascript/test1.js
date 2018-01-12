Number.prototype.myMethod = function() {
    return this.valueOf() / 2;
};

Number.prototype.toString= function() {
    return "hello";
};



var n = new Number(55);
n.toString = function() { return "dddd"}
console.log(n.myMethod());
console.log(n+' world!')
console.log(n.valueOf())

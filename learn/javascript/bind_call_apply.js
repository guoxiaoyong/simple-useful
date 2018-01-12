env = {
  id: 0
};

function set_id(id) {
  this.id = id;
}

// Default this is the global object.
set_id(999)
console.log('global id:', id)

// Bind the function bind_set_id.
bind_set_id = set_id.bind(env, 123, 456)
bind_set_id()
console.log('env: ', env)
console.log(bind_set_id)  // Bound function has different string representation.

ctx = {
};

set_id.call(ctx, 1234);
console.log('ctx: ', ctx);

set_id.apply(ctx, [12, 34])
console.log('ctx: ', ctx);

set_id.call(ctx, 9 ,8)
console.log('ctx: ', ctx);

// Both bound functions and arrow functions do not have a prototype property.

var array1 = [12, "foo", {name: "Joe"} , -2458];
var array2 = ["Doe", 555, 100];
Array.prototype.push.apply(array1, array2);
console.log(array1);


var numbers = [5, 458, 120, -215];
var maxInNumbers = Math.max.apply(Math, numbers);   //458
var maxInNumbers = Math.max.call(Math, 5, 458 , 120 , -215); //458

function isArray(obj) {
  return Object.prototype.toString.call(obj) === '[object Array]' ;
}


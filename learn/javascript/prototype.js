function Person(first, last, age, eyecolor) {
    this.firstName = first;
    this.lastName = last;
    this.age = age;
    this.eyeColor = eyecolor;
}

Person.prototype.nationality = "English";
Person.prototype.name = function () {
    return this.firstName + " " + this.lastName;
};
console.log(Person.prototype)

var myFather = new Person("John", "Doe", 50, "blue");
var myMother = new Person("Sally", "Rally", 48, "green");

console.log(myFather.nationality)
myFather.nationality = 'China'
console.log(myMother.nationality)
console.log(myFather.nationality)

console.log(myFather.__proto__);

for (prop in myFather) {
  console.log(prop);
}

console.log(Object.keys(myFather))

/*
In javascript, when usig add sign (+) to concatenate a string and
another variable, that variable will implicitly call its toString method if
it is not a string. To verify this, I made an constructor called Apple.
*/

    function Apple(name) {
        this.name = name;
    }

    Apple.prototype.toString = function() {
        console.log('Apple.prototype.toString called.');
        return this.name;
    };

    var apple = new Apple('Thai apple');
    var msg = apple + ' tastes good.'
    console.log(msg)

/*
It works as I expected: when calculate apple + ' tastes good',
Apple.prototype.toString is called.
Then I did a similar experiment on Number type.
*/

    Number.prototype.num2str = Number.prototype.toString;
    Number.prototype.toString = function() {
      console.log('new Number.prototype.toString called.');
      return this.num2str();
    }

    var msg = 'num = ' + 123;
    console.log(msg);

/*
After running it, I noticed that Number.prototype.toString is not called.
*/

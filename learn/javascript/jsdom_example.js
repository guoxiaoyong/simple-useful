jsdom = require("jsdom");

jsdom.env("", function(err, window) {
  if (err) {
    console.error(err);
    return;
  }

  global.$ = require("jquery")(window);

});

function Check_jQuery() {
  if (typeof(global.$) === 'undefined') {
    console.log('jQuery is not defined.');
    setTimeout(Check_jQuery, 1000);
  } else {
    console.log('jQuery is ready.');
    run_test();
  }
}

Check_jQuery();

function run_test() {
  $("body").append("<div>TEST</div>");
  console.log($("body").html());

  a=['helo', 99, 1,2,3,4,5]
  $.each(a, function(n, value) {console.log(n, value)})
}

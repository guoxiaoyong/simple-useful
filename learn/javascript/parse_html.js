const fs = require("fs");
const JSDOM = require("jsdom").JSDOM;
const sprintf = require('sprintf-js').sprintf

const page = fs.readFileSync("page.html","utf-8");
const $ = require('jquery')((new JSDOM(page)).window);
elements = $('span.ytd-playlist-video-renderer')

function print(index, value) {
  let content = $(value).text().trim();
  let str = sprintf("%d: %s", index, content);
  console.log(str);
}

elements.each(print)

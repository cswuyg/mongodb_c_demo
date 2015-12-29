/*
* * 测试 mongoose 耗时
* * cswuyg @ 2015.12.29
* */
"use strict";

var mongoose = require('mongoose');
var fs = require('fs');
var connect = mongoose.createConnection('mongodb://xxxhost:27030, yyyhost:27030/myapp', {mongos: true});   //注意mongos

var schema = {name: {type: String}};
var colSchema = new mongoose.Schema(schema, {collection: 'myuser'});
var model = connect.model('myuser', colSchema);

setTimeout(function() {
    var s = new Date().getTime();
    var query = new mongoose.Query({'name': 'cswuyg'}).read('n', [{location:'nj'}]);  //Query层面设置读取优先级和tag
    model.find(query).exec(function(err, doc) {
        var writerStream = fs.createWriteStream('output.txt');
        writerStream.write(doc.toString(), 'UTF8');
        writerStream.end();
        var e = new Date().getTime();
        console.log(e - s);
    });

}, 1000);

setTimeout(function() {
    var s = new Date().getTime();
    var query = new mongoose.Query({'name': 'cswuyg'}).read('n', [{location:'nj'}]);
    model.find(query).exec(function(err, doc) {
        var writerStream = fs.createWriteStream('output.txt');
        writerStream.write(doc.toString(), 'UTF8');
        writerStream.end();
        var e = new Date().getTime();
        console.log(e - s);
    });

}, 2000);

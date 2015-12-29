/*
* 测试 mongodb 驱动 耗时
* cswuyg @ 2015.12.29
*/
"use strict";
var mongodb = require('mongodb');
var assert = require('assert');
var fs = require('fs');
var url = 'mongodb://xxxhost:27030/myapp?w=1&readPreference=nearest&readPreferenceTags=location:bj';   //在uri上设置读取优先级和tag

var findDocuments = function(collection, callback) {
    collection.find({'name': 'cswuyg'}).limit(1).toArray(function(err, docs) {
        callback(docs);
    });      
}

mongodb.MongoClient.connect(url, function(err, db) {
    assert.equal(null, err);
    console.log("Connected correctly to server");
    var col = db.collection('myuser');
    var s = new Date().getTime();
    findDocuments(col, function(docs) {
        var writerStream = fs.createWriteStream('tmp.txt');
        writerStream.write(JSON.stringify(docs[0]), 'UTF8');
        writerStream.end();
        var e = new Date().getTime();
        console.log(e-s);
    });
});

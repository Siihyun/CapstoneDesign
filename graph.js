
var express = require('express');
var app = express();
var moment = require('moment');
var now = moment();
fs = require('fs');
var sqlinfo = fs.readFileSync('sqlinfo.txt').toString().split("\n");
mysql = require('mysql');
var connection = mysql.createConnection({
    host: sqlinfo[0],
    user: sqlinfo[1],
    password: sqlinfo[2],
    database: sqlinfo[3],
})
connection.connect();

app.get("/graph", function(req, res) {
  var html = fs.readFile('./graph.html',function(err,html){
  var qstr = 'select * from temperature order by time';
  html = " "+html
  connection.query(qstr, function(err, rows, cols) {
    if (err) {
      throw err;
      res.send('query error: '+ qstr);
      return;
    }
    var data = "";
    var comma = "";
    for ( var i = 0 ; i < rows.length ; i++){
         r = rows[i];
         now = moment(rows[i].time).add(-1,'month');
         data += comma + "[new Date(" + now.format('YYYY,MM,DD,HH,mm') + ",)," + r.temperature + "]";
	 comma = ",";
    }
    var header = "data.addColumn('date', 'Date/Time');"
    header += "data.addColumn('number', 'Temperature');"
    html = html.replace("<%HEADER%>",header);
    html = html.replace("<%DATA%>",data);
    res.writeHeader(200, {"Content-Type": "text/html"});
    res.write(html);
    now = moment(rows[0].time).add(-1,'month');
    var s  = now.format('YYYY/MM/DD,HH:mm');
    now = moment(rows[rows.length-1].time).add(-1,'month');
    var e = now.format('YYYY/MM/DD,HH:mm');
    res.write('START TIME : ' + s + '<br>' +'END TIME : ' + e);
    res.write('<br><br>NODEJS github url : test <br>');
    res.write('ARDUINO github url : https://github.com/Siihyun/CapstoneDesign/blob/Siihyun-4th-assignment/send_temperature_to_web.ino');
    res.end();

    });
  });

});

var server = app.listen(8083, function () {
  var host = server.address().address
  var port = server.address().port
  console.log('listening at http://%s:%s', host, port)
});

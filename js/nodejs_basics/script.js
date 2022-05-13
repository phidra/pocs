var http = require('http');

const PORT = 8080;
console.log(`You can now request 'localhost:${PORT}' to use this node server`);
console.log("Ctrl+C to quit.");

http.createServer(function (req, res) {
  console.log(`Hey, I just received a new request on url : ${req.url}`);
  res.writeHead(200, {'Content-Type': 'text/plain'});
  res.end('Hello World!');
}).listen(PORT);

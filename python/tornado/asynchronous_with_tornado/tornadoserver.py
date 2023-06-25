#!/usr/bin/env python
# coding: utf-8

import requests
import datetime

from tornado import gen, httpserver, ioloop, web, httpclient

TORNADO_PORT = 8888
gethost = lambda port_index: "http://localhost:{port}".format(port=8080+port_index)


class AsynchronousHandler(web.RequestHandler):
    """ AsynchronousHandler -> all requests are made at the same time, this takes ~ 3 seconds."""
    @gen.coroutine
    def get(self):
        before = datetime.datetime.now()
        results = yield [self.long_request(port_index) for port_index in range(1,10)]
        after = datetime.datetime.now()
        duration = (after - before).total_seconds()
        self.write("(requests took {0:.2f} seconds)   ".format(duration) + " - ".join(results))
        self.finish()

    @gen.coroutine
    def long_request(self, port_index):
        client = httpclient.AsyncHTTPClient()
        host = gethost(port_index)
        response = yield client.fetch(host)
        raise gen.Return(response.body)


class SynchronousHandler(web.RequestHandler):
    """ SynchronousHandler -> all requests are made one after the other, this takes ~ 27 seconds."""
    @gen.coroutine
    def get(self):
        before = datetime.datetime.now()
        results = [self.long_request(port_index) for port_index in range(1,10)]
        after = datetime.datetime.now()
        duration = (after - before).total_seconds()
        self.write("(requests took {0:.2f} seconds)   ".format(duration) + " - ".join(results))
        self.finish()

    def long_request(self, port_index):
        host = gethost(port_index)
        response = requests.get(host)
        assert(response.status_code == 200)
        return response.text

class HelpHandler(web.RequestHandler):
    @gen.coroutine
    def get(self):
        self.write('Invalid URL ! Valid URLs are   "/slow"   or   "/fast"')
        self.finish()


def main():
    application = web.Application([
        (r"/slow", SynchronousHandler),
        (r"/fast", AsynchronousHandler),
        (r"/.*", HelpHandler),
    ])
    http_server = httpserver.HTTPServer(application)
    http_server.listen(TORNADO_PORT)
    ioloop.IOLoop.current().start()


if __name__ == "__main__":
    main()

import jsonrpclib

server = jsonrpclib.Server('http://localhost:8080')
print server.add(12345, 23456)
print server.ping('first string')
print server.ping_later('second string')

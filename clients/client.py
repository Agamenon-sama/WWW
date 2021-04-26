#! /usr/bin/python3

import socket
import time

start = time.time()

host = "128.199.49.46"
clientSocket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
clientSocket.connect(("localhost", 8080))
request = b"GET / HTTP/1.1\r\nConnection: close\r\n"
clientSocket.send(request)
clientSocket.recv(1024)
clientSocket.close()

end = time.time()
print("\033[32mTime required : \033[34m", end - start, "s\033[0m")


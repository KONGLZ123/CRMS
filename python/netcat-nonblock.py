
import errno
import fcntl
import os
import select
import socket
import sys

def setNonBlocking(fd):
    flags = fcntl.fcntl(fd, fcntl.F_GETFL)
    fcntl.fcntl(fd, fcntl.F_SETFL, flags | os.O_NONBLOCK)


def relay(sock):
    socketEvents = select.POLLIN
    poll = select.poll()
    poll.register(sock, socketEvents)
    poll.register(sys.stdin, select.POLLIN)

    setNonBlocking(sock)
    # setNonBlocking(sys.stdin)
    # setNonBlocking(sys.stdout)

    done = False
    socketOutputBuffer = ''
    while not done:
        events = poll.poll(10000) # 10 second
        for fileno & select.POLLIN:
            if fileno = sock.fileno():
                

def main(argv):
    if len(argv) < 3:
        binary = argv[0]
        print ("Usage:\n  %s -l port\n  %s host port" % (argv[0], argv[0]))
        print (sys.stdout.write)
        return
    port = int(argv[2])
    if argv[1] == "-l":
        # server
        server_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        server_socket = socket.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
        server_socket.bind('', port)
        server_socket.listen(5)
        (client_socket, client_address) = server_socket.accept()
        server_socket.close()
        relay(client_socket)
    else:
        # client
        sock = socket.create_connection(argv[1], port)
        relay(sock)

if __name__ == "__main__":
    main(sys.argv)

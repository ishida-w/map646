import os
import socket

SOCK_FILENAME = '/tmp/map646_stat'

def main():
   try:
      while True:
         s = socket.socket(socket.AF_UNIX, socket.SOCK_STREAM)
         s.connect(SOCK_FILENAME)

         command = raw_input('>> ')
         s.send(command)
         size = int(s.recv(1024))
         print 'size: ', size
         s.send("ok")
         msg = s.recv(size)
         print msg

   except KeyboardInterrupt:
      s.close()



if __name__ == "__main__":
   main()

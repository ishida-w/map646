import os
import socket
import sys
import time

SOCK_FILENAME = '/tmp/map646_stat'

def com(f, s, command):
   s.send(command)
   size = int(s.recv(1024))
   s.send("ok")
   msg = s.recv(size)
   f.write(msg+'\n')

def a0ifnec(s):
   if len(s) == 1:
      return '0'+s
   else:
      return s

def time_struct_to_str(tm):
   year = str(tm.tm_year)
   mon = a0ifnec(str(tm.tm_mon))
   mday = a0ifnec(str(tm.tm_mday))
   hour = a0ifnec(str(tm.tm_hour))
   mn = a0ifnec(str(tm.tm_min))
   return year+mon+mday+hour+mn

def main(argvs):
   argc = len(argvs)
   command_index = range(0,argc)

   try:
      i = argvs.index("-d")
      dirname = argvs[i+1]
      filename = str("/")+time_struct_to_str(time.localtime())+str(".stat")
      f = open(dirname+filename, 'w')
      command_index.remove(i)
      command_index.remove(i+1)
   except:
      f = sys.stdout

   s = socket.socket(socket.AF_UNIX, socket.SOCK_STREAM)
   s.connect(SOCK_FILENAME)

   try:
      command = argvs[command_index.pop()]
      com(f, s, command)
   except:
      try:
         while True:
            command = raw_input('>> ')
            com(f, s, command)
            s = socket.socket(socket.AF_UNIX, socket.SOCK_STREAM)
            s.connect(SOCK_FILENAME)

      except KeyboardInterrupt:
         s.close()

if __name__ == "__main__":
   argvs = sys.argv
   main(argvs)

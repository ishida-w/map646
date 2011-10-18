#!/usr/bin/python

import client
import sys
import os
import time

def main():
   argvs = sys.argv
   argc = len(argvs)

   try:
      i = argvs.index("-d")
      dirname = argvs[i+1]
      filename = str("/map646_") + client.time_struct_to_str(time.localtime()) + str(".stat")
      f = open(dirname+filename, 'a')
   except:
      print "exception caught: failed to open file"
      exit()

   try:
      client.command("time", f)
      client.command("show", f)
      client.command("flush")
   except:
      print "exception caught"

if __name__ == "__main__":
   main()

#!/usr/bin/python

import client
import sys

def main():
   argvs = sys.argv
   argc = len(argvs)

   try:
      i = argvs.index("-d")
      dirname = argvs[i+1]
      client.main(["-d", dirname, "show"])
      client.main(["flush"])
   except:
      print "exception caught"

if __name__ == "__main__":
   main()

import sys

argvs = sys.argv
argc = len(argvs)
command_index = range(1, argc)

try:
   i = argvs.index("-d")
   filename = argvs[i+1]
   command_index.remove(i)
   command_index.remove(i+1)
   print argvs[command_index.pop()]

except:
   print "caught exception"




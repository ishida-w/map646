OBJS = server.o

server: $(OBJS) 
	g++ $(OBJS) -o $@
	scp server wataru@192.168.1.191:~/

.cpp.o:
	g++ -c $<

clean:
	rm -f *.o server *~


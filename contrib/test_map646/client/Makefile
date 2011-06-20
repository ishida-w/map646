OBJS = client.o

client: $(OBJS)
	g++ $(OBJS) -o $@
	scp client wataru@192.168.1.194:~/

.cpp.o:
	g++ -c $<

clean:
	rm -f *.o client *~
